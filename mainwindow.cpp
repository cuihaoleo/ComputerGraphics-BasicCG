#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "worldmodel.h"
#include "rscene.h"
#include "rview.h"

#include "basic_cg.h"

#include <cmath>
#include <cstdint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new PixmapScene(QSize(256, 256)))
{
    ui->setupUi(this);
    scene->setDrawMode(DrawMode::NONE);
}

void MainWindow::showEvent(QShowEvent* event) {
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
    QWidget::showEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLine_triggered()
{
    scene->setDrawMode(DrawMode::DRAW_LINE);
    ui->labelTitle->setText("Bresenham's line-drawing algorithm");
}

void MainWindow::on_actionCircle_triggered()
{
    scene->setDrawMode(DrawMode::DRAW_CIRCLE);
    ui->labelTitle->setText("Midpoint circle algorithm");
}

void MainWindow::on_actionEllipse_triggered()
{
    scene->setDrawMode(DrawMode::DRAW_ELLIPSE);
    ui->labelTitle->setText("Midpoint ellipse algorithm");
}

void MainWindow::on_actionFilling_triggered()
{
    const int canvas_height = 256;
    const int canvas_width = 256;

    GrayImage im(canvas_width, canvas_height);
    int cx = canvas_height / 2, cy = canvas_width / 2;

    ellipseMidpoint(im, QPoint(cx, cy), 100, 64);
    fillingScanlineSeed(im, QPoint(cx, cy), 127);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();

    ui->labelTitle->setText("Scanline seed filling algorithm");
}

void MainWindow::on_action3rdBezier_triggered()
{
    scene->setDrawMode(DrawMode::CURVE_3RD_BEZIER);
}

void MainWindow::on_actionBspline_triggered()
{
    // bSpline

    const int canvas_height = 256;
    const int canvas_width = 256;

    GrayImage im(canvas_width, canvas_height);

    QList<QPoint> ctrl;
    ctrl.append(QPoint(40, 40));
    ctrl.append(QPoint(100, 120));
    ctrl.append(QPoint(120, 120));
    ctrl.append(QPoint(120, 100));
    ctrl.append(QPoint(160, 150));

    QList<double> ubreak({0, 0, 0, 0, 1, 2, 2, 2, 2});
    bSpline(im, ctrl, ubreak, 100);

    circleMidpoint(im, ctrl[0], 2);
    circleMidpoint(im, ctrl[1], 2);
    circleMidpoint(im, ctrl[2], 2);
    circleMidpoint(im, ctrl[3], 2);
    circleMidpoint(im, ctrl[4], 2);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}

void MainWindow::on_actionKoch_triggered()
{
    const int canvas_height = 512;
    const int canvas_width = 512;

    GrayImage im(canvas_width, canvas_height);

    snowflakeKoch(im, QPoint(100, 100), 250, 0);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}

void MainWindow::on_actionMandelbrot_triggered()
{
    const int canvas_height = 512;
    const int canvas_width = 512;

    GrayImage im(canvas_width, canvas_height);

    paintMandelbrot(im, std::complex<double>(0),
                        std::complex<double>(-2, -2),
                        std::complex<double>(2, 2), 255);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}

void MainWindow::on_actionJulia_triggered()
{
    const int canvas_height = 512;
    const int canvas_width = 512;

    GrayImage im(canvas_width, canvas_height);

    paintJulia(im, std::complex<double>(0.285, 0.01),
                   std::complex<double>(-1.5, -1.5),
                   std::complex<double>(1.5, 1.5), 100);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}

void MainWindow::on_actionFerns_triggered()
{
    const int canvas_height = 512;
    const int canvas_width = 512;

    GrayImage im(canvas_width, canvas_height);

    paintFern(im, QPoint(100, 100), 300, 50000);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}

void MainWindow::on_actionWorld_triggered()
{
    RScene mesh;
    GrayImage im(1024, 1024);

    mesh.addPoint(QVector3D(30, 10, 10));
    mesh.addPoint(QVector3D(20, 30, 10));
    mesh.addPoint(QVector3D(40, 30, 10));
    mesh.addPoint(QVector3D(40, 15, 24));
    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(0, 2, 3);
    mesh.addTriangle(0, 1, 3);
    mesh.addTriangle(1, 2, 3);

    RView view(0, 0, 0, 0);

    //mesh.toImage(im, QPointF(30, 30), QSizeF(50, 50));

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}
