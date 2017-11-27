#ifndef RDEPTHBUFFER_H
#define RDEPTHBUFFER_H


class RDepthBuffer
{
    QPointF topLeft, downRight;
    QSize bufferSize;
    double depthBuffer;
    int flagBuffer;

public:
    RDepthBuffer(const QSize &size, QPointF &topLeft, QPointF &downRight);
    QPoint convertWorldToPixel(QPointF worldPoint);
    QPoint convertPixelToWorld(QPoint pixelPosition);
};

#endif // RDEPTHBUFFER_H
