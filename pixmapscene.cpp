#include "pixmapscene.h"

#include <QDebug>
#include <QGraphicsPixmapItem>

#include <cmath>

QMap<DrawMode, int> PixmapScene::MODE_PARAM = {
    { DrawMode::NONE, -1 },
    { DrawMode::DRAW_LINE, 2 },
    { DrawMode::DRAW_CIRCLE, 2 },
    { DrawMode::DRAW_ELLIPSE, 3 },
    { DrawMode::CURVE_3RD_BEZIER, 4 },
};

QMap<DrawMode, void (*)(GrayImage &image, QList <QPoint> points, void *args)> PixmapScene::MODE_FUNC = {
    { DrawMode::NONE, nullptr },
    { DrawMode::DRAW_LINE, PixmapScene::drawLine },
    { DrawMode::DRAW_CIRCLE, PixmapScene::drawCircle },
    { DrawMode::DRAW_ELLIPSE, PixmapScene::drawEllipse },
    { DrawMode::CURVE_3RD_BEZIER, PixmapScene::curve3rdBezier },
};


PixmapScene::PixmapScene(QSize pixmapSize, QObject *parent):
    QGraphicsScene(parent),
    mImage(pixmapSize.height(), pixmapSize.width())
{
    this->setBackgroundBrush(Qt::gray);
    mPixmapItem = addPixmap(QPixmap::fromImage(mImage.toQImage()));
    mPixmapSize = pixmapSize;
}

void PixmapScene::setDrawMode(DrawMode mode) {
    this->mDrawMode = mode;
}

void PixmapScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void PixmapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void PixmapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void PixmapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->scenePos();
    int x = std::round(pos.x());
    int y = std::round(pos.y());
    int n_param = MODE_PARAM[mDrawMode];

    qDebug() << Q_FUNC_INFO << pos;

    if (n_param < 0)
        qDebug() << Q_FUNC_INFO << "Nothing to draw";
    else if (x >= 0 && x < mPixmapSize.width() \
            && y >= 0 && y < mPixmapSize.height()) {
        mPoints.append(QPoint(x, y));
        if (mPoints.size() >= n_param) {
            qDebug() << Q_FUNC_INFO << "Draw!";
            MODE_FUNC[mDrawMode](mImage, mPoints, nullptr);
            mPixmapItem->setPixmap(QPixmap::fromImage(mImage.toQImage()));
            mPoints.clear();
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void PixmapScene::drawLine(GrayImage &image, QList<QPoint> points, void *args) {
    (void)args;
    lineBresenham(image, points[0], points[1]);
}

void PixmapScene::drawCircle(GrayImage &image, QList<QPoint> points, void *args) {
    int radius = std::hypot(points[1].x() - points[0].x(),
                            points[1].y() - points[0].y());
    (void)args;
    circleMidpoint(image, points[0], radius);
}

void PixmapScene::drawEllipse(GrayImage &image, QList<QPoint> points, void *args) {
    double cx = points[0].x(), cy = points[0].y();
    double pa = points[1].x() - cx, qa = points[1].y() - cy;
    double pb = points[2].x() - cx, qb = points[2].y() - cy;
    double pa2 = pa * pa, qa2 = qa * qa;
    double pb2 = pb * pb, qb2 = qb * qb;
    double up = pb2 * qa2 - pa2 * qb2;
    double rx = std::sqrt(up / (qa2 - qb2));
    double ry = std::sqrt(up / (pb2 - pa2));
    auto thresh = qMax(image.H(), image.W());

    if (rx >= 0 && ry >= 0 && rx < thresh && ry < thresh)
        ellipseMidpoint(image, points[0], rx, ry);

    (void)args;
}

void PixmapScene::curve3rdBezier(GrayImage &image, QList <QPoint> points, void *args) {
    (void)args;
    thirdOrderBezier(image, points[0], points[1], points[2], points[3]);
}
