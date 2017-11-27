#ifndef PIXMAPSCENE_H
#define PIXMAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QMap>

#include "basic_cg.h"

enum DrawMode {
    INUSE = -1,
    NONE = 0,
    DRAW_LINE = 1,
    DRAW_CIRCLE = 2,
    DRAW_ELLIPSE = 3,
    FILLING = 4,
    CURVE_3RD_BEZIER = 5,
    CURVE_4TH_BSPLINE = 6,
    FRACTAL_KOCH = 7,
    FRACTAL_FERN = 8,
};

class PixmapScene : public QGraphicsScene
{
    Q_OBJECT

private:
    QList<QPoint> mPoints;
    QSize mPixmapSize;
    QGraphicsPixmapItem *mPixmapItem;
    GrayImage mImage;
    DrawMode mDrawMode;

    static QMap<DrawMode, int> MODE_PARAM;
    static QMap<DrawMode, void (*)(GrayImage &image, QList <QPoint> points, void *args)> MODE_FUNC;

public:
    explicit PixmapScene(QSize pixmapSize, QObject *parent = 0);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void setDrawMode(DrawMode mode);

    GrayImage &getImage() { return mImage; }
    void update() {
        mPixmapItem->setPixmap(QPixmap::fromImage(mImage.toQImage()));
        mPoints.clear();
    }

    static void drawLine(GrayImage &image, QList <QPoint> points, void *args);
    static void drawCircle(GrayImage &image, QList <QPoint> points, void *args);
    static void drawEllipse(GrayImage &image, QList <QPoint> points, void *args);
    static void filling(GrayImage &image, QList <QPoint> points, void *args);
    static void curve3rdBezier(GrayImage &image, QList <QPoint> points, void *args);
    static void curve4rdBSpline(GrayImage &image, QList <QPoint> points, void *args);
    static void fractalKoch(GrayImage &image, QList <QPoint> points, void *args);
    static void fractalFern(GrayImage &image, QList <QPoint> points, void *args);
};


#endif // PIXMAPSCENE_H
