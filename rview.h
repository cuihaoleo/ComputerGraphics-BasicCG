#ifndef RVIEW_H
#define RVIEW_H

#include "basic_cg.h"
#include "rscene.h"
#include "rdepthbuffer.h"

#include <QVector3D>
#include <QMatrix4x4>

class RView
{
private:
    QVector3D normalVector;
    QMatrix4x4 viewTransform;
    QMatrix4x4 viewTransformR;
    RDepthBuffer *buffer = nullptr;

public:
    RView(QVector3D viewPoint, double angle);
    ~RView() {
        if (buffer != nullptr) delete buffer;
    }
    void lookAt(RScene scene, const QSize &bufferSize);
    void toDepthImage(GrayImage &im);
};



#endif // RVIEW_H
