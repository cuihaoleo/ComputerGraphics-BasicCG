#include "worldmodel.h"
#include <QPair>
#include <array>
#include <QVector4D>
#include <algorithm>
#include "basic_cg.h"

WorldObject::WorldObject()
{

}

bool WorldObject::addPoint(const QVector3D &point) {
    if (points.contains(point))
        return false;
    else {
        points.append(point);
        return true;
    }
}

void WorldObject::addPolygon(int ia, int ib, int ic){
    WorldObject::TriangleMesh mesh = { ia, ib, ic };
    polygons.append(mesh);
}

static QVector4D solvePlane(
        const QVector3D &p1, const QVector3D &p2, const QVector3D &p3 ) {
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p2;
    QVector3D n = QVector3D::crossProduct(v1, v2);
    double d = -QVector3D::dotProduct(n, p2);
    return QVector4D(n, d);
}

void WorldObject::toImage(GrayImage &im, const QPointF &view_center, const QSizeF &view_size) {
    int width = im.W(), height = im.H();
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
                im.setPixel(QPoint(x, y), 10*int(zval));
            else
                im.setPixel(QPoint(x, y), 255);
        }
    }
}
