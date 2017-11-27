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

RView::RView(QVector3D viewPoint, double angle)
{
    static const QVector3D axisZ(0, 0, 1);
    QVector3D view = -viewPoint.normalized();
    QVector3D projZ = QVector3D::crossProduct(axisZ, view);
    double theta = std::atan2(projZ.length(), QVector3D::dotProduct(axisZ, view));

    QQuaternion qua(std::cos(theta/2), projZ * std::sin(theta/2));
    QMatrix4x4 rotate1 = QMatrix4x4(qua.toRotationMatrix());  // rotate1: view in world -> z'

    QMatrix4x4 rotate2;
    rotate2.rotate(-angle, axisZ);  // rotate2: x, y rotate

    QMatrix4x4 translate;
    translate.translate(0, 0, -1);  // move origin

    QMatrix4x4 combine = translate * rotate2 * rotate1;
    combine.optimize();  // combile: world -> view

    this->normalVector = view.normalized();
    this->viewTransform = combine;
    this->viewTransformR = combine.inverted();
}

void RView::lookAt(
        RScene scene,
        const QSize &bufferSize,
        double scale)
{
    QVector<QVector3D> mapPoints;
    this->buffer = new ZBuffer(bufferSize, scale);
    this->depthBuffer = QVector<double>(
                bufferSize.height(),
                QVector<double>(bufferSize.width(), INFINITY));

    for (const QVector3D &p: scene.points) {
        QVector3D moved = this->viewTransform.map(p);  // rotate
        mapPoints.append(moved);
    }

    for (int i=0; i<mapPoints.size(); i++) {
        const QVector3D &pa = mapPoints[polygons[i].ia];
        const QVector3D &pb = mapPoints[polygons[i].ib];
        const QVector3D &pc = mapPoints[polygons[i].ic];


        int ymin = std::min({pa.y(), pb.y(), pc.y()});
        int ymax = std::max({pa.y(), pb.y(), pc.y()});
        int xmin = std::min({pa.x(), pb.x(), pc.x()});
        int xmax = std::max({pa.x(), pb.x(), pc.x()});

        if (ymin >= 0 && ymin < height && xmax >= 0 && xmin < width)
            PT[ymin].append(PTItem{ i, ymax });
    }



     /*int width = im.W(), height = im.H();
     double w_scale = width / view_size.width();
     double h_scale = height / view_size.height();
     double x_shift = view_center.x() - view_size.width()/2;
     double y_shift = view_center.y() - view_size.height()/2;

     QVector<QVector3D> canvas_pts;
     for (QVector3D p: points) {
         double canvas_x = std::round((p.x() - x_shift) * w_scale);
         double canvas_y = std::round((p.y() - y_shift) * h_scale);
         canvas_pts.append(QVector3D(canvas_x, canvas_y, p.z()));
     }

     struct PTItem {
         int idx;
         int ymax;
     };
     QVector<QVector<PTItem>> PT(height);

     for (int i=0; i<polygons.size(); i++) {
         const QVector3D &pa = canvas_pts[polygons[i].ia];
         const QVector3D &pb = canvas_pts[polygons[i].ib];
         const QVector3D &pc = canvas_pts[polygons[i].ic];
         int ymin = std::min({pa.y(), pb.y(), pc.y()});
         int ymax = std::max({pa.y(), pb.y(), pc.y()});
         int xmin = std::min({pa.x(), pb.x(), pc.x()});
         int xmax = std::max({pa.x(), pb.x(), pc.x()});

         if (ymin >= 0 && ymin < height && xmax >= 0 && xmin < width)
             PT[ymin].append(PTItem{ i, ymax });
     }

     struct APTItem {
         double A, B, C, D;
         double pkab, pkac, pkbc;
         int ymax, yturn;
         double xstart, xend;
         int xturn;
     };
     QVector<APTItem> APT;

     for (int y=0; y<height; y++) {
         for (PTItem &store: PT[y]) {
             int poly_idx = store.idx;
             int ymax = store.ymax;

             const WorldObject::TriangleMesh &mesh = polygons[poly_idx];
             QVector3D pa = canvas_pts[mesh.ia];
             QVector3D pb = canvas_pts[mesh.ib];
             QVector3D pc = canvas_pts[mesh.ic];

             // sort
             if (pa.y() > pb.y()) std::swap(pa, pb);
             if (pa.y() > pc.y()) std::swap(pa, pc);
             if (pb.y() > pc.y()) std::swap(pb, pc);

             QVector4D solution = solvePlane(pa, pb, pc);

             APTItem item;
             item.A = solution[0];
             item.B = solution[1];
             item.C = solution[2];
             item.D = solution[3];
             item.ymax = ymax;
             item.yturn = std::round(pb.y());
             item.pkab = (pa.x() - pb.x()) / (pa.y() - pb.y());
             item.pkac = (pa.x() - pc.x()) / (pa.y() - pc.y());
             item.pkbc = (pb.x() - pc.x()) / (pb.y() - pc.y());
             item.xstart = item.xend = pa.x();
             item.xturn = std::round(pb.x());
             APT.append(item);
         }

         QVector<APTItem>::iterator iter = APT.begin();
         QVector<double> sbuf(width, INFINITY);

         while (iter != APT.end()) {
             if (y > iter->ymax) {
                 iter = APT.erase(iter);
                 continue;
             }

             double delta_yend = y < iter->yturn ? iter->pkab : iter->pkbc;
             int xstart = std::round(qMin(iter->xstart, iter->xend));
             int xend = std::round(qMax(iter->xstart, iter->xend));
             double A = iter->A, B = iter->B, C = iter->C, D = iter->D;

             double z = (-A*xstart - B*y - D) / C; // z at (xstart, y)
             for (int x=xstart; x<=xend; x++) {
                 z -= A / C;
                 if (z < sbuf[x]) sbuf[x] = z;
             }

             iter->xstart += iter->pkac;
             iter->xend += delta_yend;

             if (y == iter->yturn) iter->xend = iter->xturn;

             iter++;
         }

         for (int x=0; x<width; x++) {
             double zval = sbuf[x];
             if (zval < 25)
                 im.setPixel(QPoint(x, y), 255 - 10*zval);
             else
                 im.setPixel(QPoint(x, y), 0);
         }
     }
*/
}
