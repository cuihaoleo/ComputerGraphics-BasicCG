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
    const int canvas_height = 768;
    const int canvas_width = 1024;

    GrayImage im(canvas_width, canvas_height);

    // simple test
    for (int i=0; i<500; i++) {
        im.setPixel(100, i);
    }

    QImage qi = im.toQImage();
    QGraphicsScene* scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(qi));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->show();
}
