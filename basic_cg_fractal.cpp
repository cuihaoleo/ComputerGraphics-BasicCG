#include "basic_cg.h"

#include <cmath>
#include <complex>
#include <random>

static void _snowflakeKoch(GrayImage &im, QPointF &pointer, double angle, double step) {

    QPointF p = pointer;
    if (step <= 5) {
        QPoint p1(std::round(pointer.x()), std::round(pointer.y()));
        pointer += QPointF(step*std::cos(angle), step*std::sin(angle));
        QPoint p2(std::round(pointer.x()), std::round(pointer.y()));
        lineBresenham(im, p1, p2);
        return;
    }

    // F -> F-F++F+F
    _snowflakeKoch(im, p, angle, step / 3);  // F
    angle -= M_PI / 3;                       // -
    _snowflakeKoch(im, p, angle, step / 3);  // F
    angle += M_PI / 3 * 2;                   // + +
    _snowflakeKoch(im, p, angle, step / 3);  // F
    angle -= M_PI / 3;                       // +
    _snowflakeKoch(im, p, angle, step / 3);  // F

    pointer += QPointF(step*std::cos(angle), step*std::sin(angle));
}

void snowflakeKoch(GrayImage &im, const QPoint &start, double step, double theta) {
    QPointF pointer(start.x(), start.y());
    _snowflakeKoch(im, pointer, theta, step);
    _snowflakeKoch(im, pointer, theta + M_PI/3*2, step);
    _snowflakeKoch(im, pointer, theta + M_PI/3*4, step);
}

void paintMandelbrot(GrayImage &im,
                     std::complex<double> z0,
                     std::complex<double> c0,
                     std::complex<double> c1,
                     int max_round) {
    static const double ESCAPE_RADIUS = 2;

    double x0 = c0.real(), x1 = c1.real();
    double y0 = c0.imag(), y1 = c1.imag();
    double height = im.H(), width = im.W();

    for (int yi=0; yi<height; yi++) {
        double y = y0 + (y1 - y0) / height * yi;

        for (int xi=0; xi<width; xi++) {
            double x = x0 + (x1 - x0) / width * xi;
            std::complex<double> c(x, y);
            std::complex<double> z = z0;
            int round = 0;

            while (round<max_round && std::abs(z)<=ESCAPE_RADIUS) {
                z = z * z + c;
                round++;
            }

            double color = 1.0 - double(round) / max_round;
            im.setPixel(QPoint(xi, yi), int(color * 255));
        }
    }
}

void paintJulia(GrayImage &im,
                     std::complex<double> c0,
                     std::complex<double> z0,
                     std::complex<double> z1,
                     int max_round) {
    static const double ESCAPE_RADIUS = 100;
    double x0 = z0.real(), x1 = z1.real();
    double y0 = z0.imag(), y1 = z1.imag();
    double height = im.H(), width = im.W();

    for (int yi=0; yi<height; yi++) {
        double y = y0 + (y1 - y0) / height * yi;

        for (int xi=0; xi<width; xi++) {
            double x = x0 + (x1 - x0) / width * xi;
            std::complex<double> z(x, y);
            int round = 0;

            while (round<max_round && std::abs(z)<=ESCAPE_RADIUS) {
                z = z * z + c0;
                round++;
            }

            double color = 1.0 - double(round) / max_round;
            im.setPixel(QPoint(xi, yi), int(color * 255));
        }
    }
}

void paintFern(GrayImage &im, const QPoint &start, double size, int pts) {
    double desc[4][7] = {
        { 0.01,  0,     0,     0,     0.16,  0,     0,   },
        { 0.85,  0.85,  0.04,  -0.04, 0.85,  0,     1.60 },
        { 0.07,  0.20,  -0.26, 0.23,  0.22,  0,     1.60 },
        { 0.07,  -0.15, 0.28,  0.26,  0.24,  0,     0.44 }};
    double x = 0, y = 0;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int i=0; i<pts; i++) {
        double dice = distribution(generator);
        int choice;

        for (choice=0; choice<4 && dice>0; choice++)
            dice -= desc[i][0];

        double *para = desc[choice];
    }
}
