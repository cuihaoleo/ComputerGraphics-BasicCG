#ifndef RVIEW_H
#define RVIEW_H

#include "rscene.h"

#include <QVector3D>
#include <QMatrix4x4>

class RView
{
private:
    QVector3D normalVector;
    QMatrix4x4 viewTransform;
    QMatrix4x4 viewTransformR;
    ZBuffer *buffer = nullptr;

public:
    RView(QVector3D viewPoint, double angle);
    ~RView() {
        if (buffer != nullptr) delete buffer;
    }
    void RView::lookAt(RScene scene, const QSize &bufferSize, double scale);
};



#endif // RVIEW_H
