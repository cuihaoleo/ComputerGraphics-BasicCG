#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    ellipseMidpoint(im, cx, cy, 100, 64);
    fillingScanlineSeed(im, cx, cy, 127);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();

    ui->labelTitle->setText("Scanline seed filling algorithm");
}
