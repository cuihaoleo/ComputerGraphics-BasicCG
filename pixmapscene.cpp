#include "pixmapscene.h"

#include <QDebug>
#include <QGraphicsPixmapItem>

#include <cmath>

QMap<DrawMode, int> PixmapScene::MODE_PARAM = {
    { DrawMode::NONE, -1 },
    { DrawMode::DRAW_LINE, 2 },
    { DrawMode::DRAW_CIRCLE, 2 },
    { DrawMode::DRAW_ELLIPSE, 2 },
    { DrawMode::FILLING, 1 },
    { DrawMode::CURVE_3RD_BEZIER, 4 },
    { DrawMode::CURVE_4TH_BSPLINE, 5 },
    { DrawMode::FRACTAL_KOCH, 2 },
    { DrawMode::FRACTAL_FERN, 2 },
};

QMap<DrawMode, void (*)(GrayImage &image, QList <QPoint> points, void *args)> PixmapScene::MODE_FUNC = {
    { DrawMode::NONE, nullptr },
    { DrawMode::DRAW_LINE, PixmapScene::drawLine },
    { DrawMode::DRAW_CIRCLE, PixmapScene::drawCircle },
    { DrawMode::DRAW_ELLIPSE, PixmapScene::drawEllipse },
    { DrawMode::FILLING, PixmapScene::filling },
    { DrawMode::CURVE_3RD_BEZIER, PixmapScene::curve3rdBezier },
    { DrawMode::CURVE_4TH_BSPLINE, PixmapScene::curve4rdBSpline },
    { DrawMode::FRACTAL_KOCH, PixmapScene::fractalKoch },
    { DrawMode::FRACTAL_FERN, PixmapScene::fractalFern },
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
            this->update();
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
    QPoint diff = points[1] - points[0];
    double rx = std::abs(diff.x());
    double ry = std::abs(diff.y());
    ellipseMidpoint(image, points[0], rx, ry);
    (void)args;
}

void PixmapScene::filling(GrayImage &image, QList<QPoint> points, void *args) {
    fillingScanlineSeed(image, points[0], 127);
    (void)args;
}

void PixmapScene::curve3rdBezier(GrayImage &image, QList <QPoint> points, void *args) {
    (void)args;
    thirdOrderBezier(image, points[0], points[1], points[2], points[3]);
}

void PixmapScene::curve4rdBSpline(GrayImage &image, QList <QPoint> points, void *args) {
    (void)args;
    QList<double> ubreak({0, 0, 0, 0, 1, 2, 2, 2, 2});
    bSpline(image, points, ubreak, 100);
}

void PixmapScene::fractalKoch(GrayImage &image, QList <QPoint> points, void *args) {
    QPoint p1 = points[0];
    QPoint p2 = points[1];
    QPoint tl(qMin(p1.x(), p2.x()), qMin(p1.y(), p2.y()));
    QPoint dr(qMax(p1.x(), p2.x()), qMax(p1.y(), p2.y()));
    QPoint diff = dr - tl;
    snowflakeKoch(image, tl, diff.manhattanLength(), 0);
    (void)args;
}

void PixmapScene::fractalFern(GrayImage &image, QList <QPoint> points, void *args) {
    QPoint p1 = points[0];
    QPoint p2 = points[1];
    QPoint tl(qMin(p1.x(), p2.x()), qMin(p1.y(), p2.y()));
    QPoint dr(qMax(p1.x(), p2.x()), qMax(p1.y(), p2.y()));
    QPoint diff = dr - tl;
    double mlength = diff.manhattanLength();
    paintFern(image, tl, mlength, 100 * mlength);
    (void)args;
}
