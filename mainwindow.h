#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic_cg.h"
#include "pixmapscene.h"

#include <QSize>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLine_triggered();
    void on_actionCircle_triggered();
    void on_actionEllipse_triggered();
    void on_actionFilling_triggered();
    void on_action3rdBezier_triggered();
    void on_actionBspline_triggered();
    void on_actionKoch_triggered();
    void on_actionMandelbrot_triggered();
    void on_actionJulia_triggered();
    void on_actionFerns_triggered();

    void on_actionClear_triggered();

    void on_actionSave_triggered();

protected:
    virtual void showEvent(QShowEvent* event);

private:
    Ui::MainWindow *ui;
    PixmapScene *scene;
};


#endif // MAINWINDOW_H
