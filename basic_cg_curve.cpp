#include "basic_cg.h"
#include <QPointF>
#include <QVector>
#include <QDebug>
#include <cmath>
#include <cassert>

static void _thirdOrderBezier(GrayImage &im,
                       const QPointF &p1,
                       const QPointF &p2,
                       const QPointF &p3,
                       const QPointF &p4) {
    QPointF diff = p1 - p4;
    if (diff.manhattanLength() < 1) {
        im.setPixel(std::round(p1.x()), std::round(p1.y()));
        im.setPixel(std::round(p4.x()), std::round(p4.y()));
        return;
    }

    QPointF l1 = p1;
    QPointF l2 = (p1 + p2) / 2.0;
    QPointF h = (p2 + p3) / 2.0;
    QPointF r3 = (p3 + p4) / 2.0;
    QPointF r4 = p4;
    QPointF l3 = (l2 + h) / 2.0;
    QPointF r2 = (r3 + h) / 2.0;
    QPointF l4 = (l3 + r2) / 2.0;
    QPointF r1 = l4;
    _thirdOrderBezier(im, l1, l2, l3, l4);
    _thirdOrderBezier(im, r1, r2, r3, r4);
}

void thirdOrderBezier(GrayImage &im,
                      const QPoint &p1,
                      const QPoint &p2,
                      const QPoint &p3,
                      const QPoint &p4) {
    QPointF p1f(p1.x(), p1.y());
    QPointF p2f(p2.x(), p2.y());
    QPointF p3f(p3.x(), p3.y());
    QPointF p4f(p4.x(), p4.y());
    _thirdOrderBezier(im, p1f, p2f, p3f, p4f);
}

void bSpline(GrayImage &im,
             QList<QPoint> &control, QList<double> &ubreak,
             int ndiv) {
    int n = control.size() - 1;
    int m = ubreak.size() - 1;
    int deg = m - n;
    assert (2*(deg-1) < m);

    double tmin = ubreak[deg-1];
    double tmax = ubreak[m-(deg-1)];
    double tstep = (tmax - tmin) / ndiv;
    QVector<double> bk(n+2, 0);
    QPoint pcur, plast(-1, -1);

    qDebug() << "Tmin:" << tmin << ", Tmax:" << tmax;
    qDebug() << "n =" << n << "m =" << m << "deg =" << deg;
    for (int i=0, j=0; i<ndiv; i++) {
        double u = tmin + tstep * (i + 0.5);
        if (u >= ubreak[j+1] && j < n)
            j++;

        bk.fill(0.0);
        bk[j] = 1.0;

        for (int d=2; d<=deg; d++) {
            for (int k=0; k<=n; k++) {
                double p1 = (u - ubreak[k]) / (ubreak[k+d-1] - ubreak[k]);
                double p2 = (ubreak[k+d] - u) / (ubreak[k+d] - ubreak[k+1]);

                if (!std::isnormal(p1)) p1 = 0.0;
                if (!std::isnormal(p2)) p2 = 0.0;

                bk[k] = p1 * bk[k] + p2 * bk[k+1];
            }
        }

        double sumx = 0.0, sumy = 0.0;
        bool invalid = false;

        for (int k=0; k<=n; k++) {
            if (bk[k] < 0) {
                invalid = true;
                break;
            }

            sumx += bk[k] * control[k].x();
            sumy += bk[k] * control[k].y();
        }

        qDebug() << Q_FUNC_INFO << u << j << sumx << sumy;
        qDebug() << Q_FUNC_INFO << bk;

        if (!invalid) {
            pcur = QPoint(std::round(sumx), std::round(sumy));
            if (plast.x() > 0) lineBresenham(im, plast, pcur);
            plast = pcur;
        }
    }
}
