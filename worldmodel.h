#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include "basic_cg.h"

#include <QVector>
#include <QVector3D>
#include <QVector4D>

#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>

class WorldObject
{
private:
    struct TriangleMesh {
        int ia, ib, ic;
    };

    QVector<QVector3D> points;
    QVector<TriangleMesh> polygons;
public:
    WorldObject();
    bool addPoint(const QVector3D &point);
    void addPolygon(int ia, int ib, int ic);
    void toImage(GrayImage &im, const QPointF &view_center, const QSizeF &view_size);
};

#endif // WORLDMODEL_H
