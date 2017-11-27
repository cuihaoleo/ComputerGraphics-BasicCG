#ifndef RVIEW_H
#define RVIEW_H

#include "rscene.h"

#include <QVector3D>
#include <QMatrix4x4>

class RView
{
private:
    QVector3D viewPoint;
    // assume view direction is towards world origin point
    QVector3D viewUp;
    bool usePerspectiveProjection;

    QVector<QVector<int>> depthBuffer;
    QVector<QVector<int>> meshBuffer;

public:
    RView(double radius, double alpha, double beta, double gamma);
    void lookAt(RSence scene);
};

#endif // RVIEW_H
