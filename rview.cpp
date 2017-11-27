#include "rscene.h"
#include "rview.h"

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QSize>

#include <cmath>

static QMatrix4x4 eulerRotateZXZ(double alpha, double beta, double gamma) {
    using namespace std;
    double c1 = cos(alpha), s1 = sin(alpha);
    double c2 = cos(beta),  s2 = sin(beta);
    double c3 = cos(gamma), s3 = sin(gamma);
    QMatrix4x4 transform(
                c1*c3-c2*s1*s3, -c1*s3-c2*c3*s1, s1*s2,  0,
                c3*s1+c1*c2*s3, c1*c2*c3-s1*s3,  -c1*s2, 0,
                s2*s3,          c3*s2,           c2,     0,
                0,              0,               0,      1);
    return transform;
}

static QVector4D solvePlane(
        const QVector3D &p1, const QVector3D &p2, const QVector3D &p3 ) {
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p2;
    QVector3D n = QVector3D::crossProduct(v1, v2);
    double d = -QVector3D::dotProduct(n, p2);
    return QVector4D(n, d);
}

RView::RView(QVector3D viewPoint, double angle)
{
    static const QVector3D axisZ(0, 0, 1);
    double degree = angle * 180.0 / M_PI;
    QVector3D view = -viewPoint.normalized();
    QVector3D projZ = QVector3D::crossProduct(view, axisZ);
    double theta = std::atan2(projZ.length(), QVector3D::dotProduct(axisZ, view));

    QQuaternion qua(std::cos(theta/2), projZ.normalized() * std::sin(theta/2));
    QMatrix4x4 rotate1 = QMatrix4x4(qua.toRotationMatrix());  // rotate1: view in world -> z'

    QMatrix4x4 rotate2;
    rotate2.rotate(-degree, axisZ);  // rotate2: x, y rotate

    QMatrix4x4 translate;
    QVector3D newZ = rotate1.map(-viewPoint);
    translate.translate(0, 0, newZ.z());  // move origin

    QMatrix4x4 combine = translate * rotate2 * rotate1;
    combine.optimize();  // combile: world -> view

    this->normalVector = view.normalized();
    this->viewTransform = combine;
    this->viewTransformR = combine.inverted();
}

void RView::lookAt(RScene scene, const QSize &bufferSize)
{
    QVector<QVector3D> viewPoints;
    double maxX, maxY;
    double minX, minY;

    maxX = maxY = -INFINITY;
    minX = minY = INFINITY;

    for (const QVector3D &p: scene.points) {
        QVector3D moved = this->viewTransform.map(p);
        if (moved.x() > maxX) maxX = moved.x();
        if (moved.x() < minX) minX = moved.x();
        if (moved.y() > maxY) maxY = moved.y();
        if (moved.y() < minY) minY = moved.y();
        viewPoints.append(moved);
    }

    QPointF topLeft(minX - 1, minY - 1);
    QPointF downRight(maxX + 1, maxY + 1);
    this->buffer = new RDepthBuffer(bufferSize, topLeft, downRight);

    QVector<QPoint> drawPoints;
    QVector<double> drawZ;
    for (const QVector3D &p: viewPoints) {
        QPoint dp = this->buffer->convertWorldToPixel(p.toPointF());
        drawPoints.append(dp);
        drawZ.append(p.z());
    }

    struct PTItem {
        int idx;
        int ymax;
    };
    QVector<QVector<PTItem>> PT(bufferSize.height());

    for (int i=0; i<drawPoints.size(); i++) {
        const QPoint &pa = drawPoints[scene.mesh[i][0]];
        const QPoint &pb = drawPoints[scene.mesh[i][1]];
        const QPoint &pc = drawPoints[scene.mesh[i][2]];

        int ymin = std::min({pa.y(), pb.y(), pc.y()});
        int ymax = std::max({pa.y(), pb.y(), pc.y()});

        PT[ymin].append(PTItem{ i, ymax });
    }

    struct APTItem {
        int index;
        QVector4D factor;
        double kab, kac, kbc;
        int ymax, yturn;
        double xstart, xend;
        int xturn;
    };
    QVector<APTItem> APT;

    for (int y=0; y<bufferSize.height(); y++) {

        // Add new items to APT
        for (PTItem &store: PT[y]) {
            int idx = store.idx, ymax = store.ymax;
            const std::array<int, 3> &mesh = scene.mesh[idx];

            QVector3D pa(drawPoints[mesh[0]]);
            pa.setZ(drawZ[mesh[0]]);
            QVector3D pb(drawPoints[mesh[1]]);
            pb.setZ(drawZ[mesh[1]]);
            QVector3D pc(drawPoints[mesh[2]]);
            pc.setZ(drawZ[mesh[2]]);

            // sort
            if (pa.y() > pb.y()) std::swap(pa, pb);
            if (pa.y() > pc.y()) std::swap(pa, pc);
            if (pb.y() > pc.y()) std::swap(pb, pc);

            QVector4D solution = solvePlane(pa, pb, pc);

            APTItem item;
            item.index = idx;
            item.factor = solution;
            item.ymax = ymax;
            item.yturn = std::round(pb.y());
            item.kab = (pa.x() - pb.x()) / (pa.y() - pb.y());
            item.kac = (pa.x() - pc.x()) / (pa.y() - pc.y());
            item.kbc = (pb.x() - pc.x()) / (pb.y() - pc.y());
            item.xstart = item.xend = pa.x();
            item.xturn = std::round(pb.x());
            APT.append(item);
        }

        // scanline
        QVector<APTItem>::iterator iter = APT.begin();
        while (iter != APT.end()) {
            if (y > iter->ymax) {
                iter = APT.erase(iter);
                continue;
            }

            double delta_yend = y < iter->yturn ? iter->kab : iter->kbc;
            int xstart = std::round(qMin(iter->xstart, iter->xend));
            int xend = std::round(qMax(iter->xstart, iter->xend));
            double A = iter->factor[0];
            double B = iter->factor[1];
            double C = iter->factor[2];
            double D = iter->factor[3];

            double z = (-A*xstart - B*y - D) / C; // z at (xstart, y)
            for (int x=xstart; x<=xend; x++) {
                z -= A / C;
                this->buffer->update(QPoint(x, y), z, iter->index);
            }

            iter->xstart += iter->kac;
            iter->xend += delta_yend;
            if (y == iter->yturn) iter->xend = iter->xturn;
            iter++;
        }
    }
}

void RView::toDepthImage(GrayImage &im) {
    int height = im.H(), width = im.W();
    double scaleH = buffer->H() / double(height);
    double scaleW = buffer->W() / double(width);

    double maxZ = 1.0;
    for (int i=0; i<buffer->H(); i++)
        for (int j=0; j<buffer->W(); j++) {
            double z = buffer->getDepth(QPoint(j, i));
            if (std::isnormal(z) && z > maxZ)
                maxZ = z;
        }

    maxZ *= 1.1;
    for (int i=0; i<height; i++)
        for (int j=0; j<width; j++) {
            int mi = std::round(i * scaleH);
            int mj = std::round(j * scaleW);
            double zval = buffer->getDepth(QPoint(mj, mi));
            assert (zval > 0);

            if (std::isnormal(zval)) {
                im.setPixel(QPoint(j, i), 255 * (1 - zval / maxZ));
            } else {
                im.setPixel(QPoint(j, i), 0);
            }
        }
}
