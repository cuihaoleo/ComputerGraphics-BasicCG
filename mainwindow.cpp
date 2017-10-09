#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "basic_cg.h"

#include <cstdint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLine_triggered()
{
    const int canvas_height = 256;
    const int canvas_width = 256;

    GrayImage im(canvas_width, canvas_height);

    int mx = canvas_width / 2, my = canvas_height / 2;
    lineBresenham(im, mx, my, mx+100, my);
    lineBresenham(im, mx, my, mx+90, my+50);
    lineBresenham(im, mx, my, mx+50, my+90);
    lineBresenham(im, mx, my, mx, my+100);
    lineBresenham(im, mx, my, mx-50, my+90);
    lineBresenham(im, mx, my, mx-90, my+50);
    lineBresenham(im, mx, my, mx-100, my);
    lineBresenham(im, mx, my, mx-90, my-50);
    lineBresenham(im, mx, my, mx-50, my-90);
    lineBresenham(im, mx, my, mx, my-100);
    lineBresenham(im, mx, my, mx+50, my-90);
    lineBresenham(im, mx, my, mx+90, my-50);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();

    ui->labelTitle->setText("Bresenham's line-drawing algorithm");
}

void MainWindow::on_actionCircle_triggered()
{
    const int canvas_height = 256;
    const int canvas_width = 256;

    GrayImage im(canvas_width, canvas_height);
    int cx = canvas_height / 2, cy = canvas_width / 2;

    circleMidpoint(im, cx, cy, 1);
    circleMidpoint(im, cx, cy, 5);
    circleMidpoint(im, cx, cy, 10);
    circleMidpoint(im, cx, cy, 20);
    circleMidpoint(im, cx, cy, 40);
    circleMidpoint(im, cx, cy, 80);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();

    ui->labelTitle->setText("Midpoint circle algorithm");
}

void MainWindow::on_actionEllipse_triggered()
{
    const int canvas_height = 256;
    const int canvas_width = 256;

    GrayImage im(canvas_width, canvas_height);
    int cx = canvas_height / 2, cy = canvas_width / 2;

    ellipseMidpoint(im, cx, cy, 2, 4);
    ellipseMidpoint(im, cx, cy, 8, 4);
    ellipseMidpoint(im, cx, cy, 8, 16);
    ellipseMidpoint(im, cx, cy, 32, 16);
    ellipseMidpoint(im, cx, cy, 32, 64);
    ellipseMidpoint(im, cx, cy, 100, 64);

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();

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
