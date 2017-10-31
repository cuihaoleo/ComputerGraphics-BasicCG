#ifndef PIXMAPSCENE_H
#define PIXMAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMap>

#include "basic_cg.h"

enum DrawMode {
    NONE = 0,
    DRAW_LINE = 1,
    DRAW_CIRCLE = 2,
    DRAW_ELLIPSE = 3,
    FILLING = 4,
};

class PixmapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PixmapScene(QSize pixmapSize, QObject *parent = 0);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void setDrawMode(DrawMode mode);

    static void drawLine(GrayImage &image, QList <QPoint> points, void *args);
    static void drawCircle(GrayImage &image, QList <QPoint> points, void *args);
    static void drawEllipse(GrayImage &image, QList <QPoint> points, void *args);

private:
    QList<QPoint> mPoints;
    QSize mPixmapSize;
    QGraphicsPixmapItem *mPixmapItem;
    GrayImage mImage;
    DrawMode mDrawMode;

    static QMap<DrawMode, int> MODE_PARAM;
    static QMap<DrawMode, void (*)(GrayImage &image, QList <QPoint> points, void *args)> MODE_FUNC;
};


#endif // PIXMAPSCENE_H
