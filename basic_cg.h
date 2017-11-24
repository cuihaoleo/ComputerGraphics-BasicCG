#ifndef BASIC_CG_H
#define BASIC_CG_H

// Use math constants in Visual Studio
#define _USE_MATH_DEFINES

#include <cstdint>
#include <QImage>
#include <QPoint>
#include <complex>
#include <QDebug>

class GrayImage {
private:
    uint8_t **data;
    size_t width, height;
public:
    GrayImage(size_t width, size_t height, uint8_t bg = 255);
    ~GrayImage();

    inline uint8_t getPixel(int x, int y) const {
        return data[y][x];
    }

    void setPixel(int x, int y, uint8_t val=0);
    inline void setPixel(const QPoint &p, uint8_t val=0) {
        setPixel(p.x(), p.y(), val);
    }

    size_t H() { return height; }
    size_t W() { return width; }
    QImage toQImage() const;
};

bool isEightNeighbor(const QPoint &a, const QPoint &b);

// Line-drawing algorithm
void lineBresenham(GrayImage &im, const QPoint &a, const QPoint &b);
// Circle-drawing algorithm
void circleMidpoint(GrayImage &im, const QPoint &center, int radius);
// Ellipse-drawing algorithm
void ellipseMidpoint(GrayImage &im, const QPoint &center, int rx, int ry);
// Filled-area algorithm
void fillingScanlineSeed(GrayImage &im, const QPoint &seed, uint8_t val);

// Third order Bezier curve with De Casteljau's algorithm
void thirdOrderBezier(GrayImage &im,
                      const QPoint &p1,
                      const QPoint &p2,
                      const QPoint &p3,
                      const QPoint &p4);
// B-Spline
void bSpline(GrayImage &im,
             QList<QPoint> &control, QList<double> &ubreak,
             int ndiv);

void snowflakeKoch(GrayImage &im, const QPoint &start, double step, double theta);

void paintMandelbrot(GrayImage &im,
                     std::complex<double> z0,
                     std::complex<double> c0,
                     std::complex<double> c1,
                     int max_round);
void paintJulia(GrayImage &im,
                     std::complex<double> c0,
                     std::complex<double> z0,
                     std::complex<double> z1,
                     int max_round);

void paintFern(GrayImage &im, const QPoint &start, double size, int pts);

#endif // BASIC_CG_H
