#include "rdepthbuffer.h"

RDepthBuffer::RDepthBuffer(const QSize &size, QPointF &topLeft, QPointF &downRight) {
    this->topLeft = topLeft;
    this->downRight = downRight;
    this->bufferSize = size;

}

QPoint convertWorldToPixel(QPointF worldPoint);
QPoint convertPixelToWorld(QPoint pixelPosition);

class ZBuffer {
    friend class RView;
private:
    QPointF topLeft, downRight;
    QSize bufferSize;
    QVector<QVector<double>> depthBuffer;
    QVector<QVector<int>> meshBuffer;
public:
    ZBuffer(const QSize &size, QPointF &tl, QPointF &dr): \
        bufferSize(size), topLeft(tl), downRight(dr) {
        this->depthBuffer = QVector<QVector<double>>(
                    bufferSize.height(),
                    QVector<double>(bufferSize.width(), INFINITY));
        this->meshBuffer = QVector<QVector<int>>(
                    bufferSize.height(),
                    QVector<int>(bufferSize.width(), -1));
    }
    ~ZBuffer();
};
