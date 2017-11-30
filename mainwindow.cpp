#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "basic_cg.h"

#include <QFileDialog>

#include <cmath>
#include <cstdint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new PixmapScene(QSize(600, 400)))
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
                    std::complex<double>(-2.1, -1.4),
                    std::complex<double>(2.1, 1.4), 255);
    scene->update();
    ui->labelTitle->setText("Mandelbrot set");
    ui->labelMessage->setText("z = 0");
}

void MainWindow::on_actionJulia_triggered()
{
    scene->setDrawMode(DrawMode::INUSE);
    paintJulia(scene->getImage(),
               std::complex<double>(0.285, 0.01),
               std::complex<double>(-2.4, -1.6),
               std::complex<double>(2.4, 1.6), 100);
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

void MainWindow::on_actionClear_triggered()
{
     scene->setDrawMode(DrawMode::NONE);
     scene->getImage().clear();
     scene->update();
     ui->labelTitle->setText("Clear everything");
     ui->labelMessage->setText("Nothing here");
}

void MainWindow::on_actionSave_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,
            tr("Save Address Book"), "", tr("Portable Network Graphics (*.png)"));

    if (!path.endsWith(".png"))
        path += ".png";

    QImage image = scene->getImage().toQImage();
    image.save(path, "PNG");
}
