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
    scene(new PixmapScene(QSize(512, 512)))
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
    ui->labelMessage->setText("Click two endpoints to draw a line between them");
}

void MainWindow::on_actionCircle_triggered()
{
    scene->setDrawMode(DrawMode::DRAW_CIRCLE);
    ui->labelTitle->setText("Midpoint circle algorithm");
    ui->labelMessage->setText("First click for center. Second for radius.");
}

void MainWindow::on_actionEllipse_triggered()
{
    scene->setDrawMode(DrawMode::DRAW_ELLIPSE);
    ui->labelTitle->setText("Midpoint ellipse algorithm");
    ui->labelMessage->setText("First click for center. Second for bounding box.");
}

void MainWindow::on_actionFilling_triggered()
{
    scene->setDrawMode(DrawMode::FILLING);
    ui->labelTitle->setText("Scanline seed filling algorithm");
    ui->labelMessage->setText("Click a point inside a closed shape. I'll paint it gray.");
}

void MainWindow::on_action3rdBezier_triggered()
{
    scene->setDrawMode(DrawMode::CURVE_3RD_BEZIER);
    ui->labelTitle->setText("Bezier curve (3rd-order)");
    ui->labelMessage->setText("Click 4 control points!");
}

void MainWindow::on_actionBspline_triggered()
{
    scene->setDrawMode(DrawMode::CURVE_4TH_BSPLINE);
    ui->labelTitle->setText("B-spline curve (4th-order)");
    ui->labelMessage->setText("Click 5 control points!");
}

void MainWindow::on_actionKoch_triggered()
{
    scene->setDrawMode(DrawMode::FRACTAL_KOCH);
    ui->labelTitle->setText("Koch snowflower");
    ui->labelMessage->setText("Click two points. First for origin. Second for size.");
}

void MainWindow::on_actionMandelbrot_triggered()
{
    scene->setDrawMode(DrawMode::INUSE);
    paintMandelbrot(scene->getImage(),
                    std::complex<double>(0),
                    std::complex<double>(-2, -2),
                    std::complex<double>(2, 2), 255);
    scene->update();
    ui->labelTitle->setText("Mandelbrot set");
    ui->labelMessage->setText("z = 0");
}

void MainWindow::on_actionJulia_triggered()
{
    scene->setDrawMode(DrawMode::INUSE);
    paintJulia(scene->getImage(),
               std::complex<double>(0.285, 0.01),
               std::complex<double>(-1.5, -1.5),
               std::complex<double>(1.5, 1.5), 100);
    scene->update();
    ui->labelTitle->setText("Julia set");
    ui->labelMessage->setText("c = 0.285 + 0.01i");
}

void MainWindow::on_actionFerns_triggered()
{
    scene->setDrawMode(DrawMode::FRACTAL_FERN);
    ui->labelTitle->setText("Fern");
    ui->labelMessage->setText("Click two points. First for origin. Second for size.");
}

void MainWindow::on_actionWorld_triggered()
{
    static double mem = 0;
    RScene mesh;
    GrayImage im(1024, 1024);

    mesh.addPoint(QVector3D(0, 1, 0));
    mesh.addPoint(QVector3D(0, -1, 1));
    mesh.addPoint(QVector3D(1, -1, -1));
    mesh.addPoint(QVector3D(-1, -1, -1));
    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(0, 2, 3);
    mesh.addTriangle(0, 1, 3);
    mesh.addTriangle(1, 2, 3);

    //double r = 60;
    //RView view(QVector3D(r*std::sin(mem), 0, -r*std::cos(mem)), 0.0);
    mem += 0.1;
    RView view(QVector3D(2, -1, -2), 0.0);
    view.lookAt(mesh, QSize(1024, 1024));
    view.toDepthImage(im);

    //mesh.toImage(im, QPointF(30, 30), QSizeF(50, 50));

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}
