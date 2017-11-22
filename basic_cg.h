#ifndef BASIC_CG_H
#define BASIC_CG_H

#include <cstdint>
#include <QImage>
#include <QPoint>

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
    size_t H() { return height; };
    size_t W() { return width; };
    QImage toQImage() const;
};

// Line-drawing algorithm
void lineBresenham(GrayImage &im, const QPoint &a, const QPoint &b);
// Circle-drawing algorithm
void circleMidpoint(GrayImage &im, const QPoint &center, int radius);
// Ellipse-drawing algorithm
void ellipseMidpoint(GrayImage &im, const QPoint &center, int rx, int ry);
// Filled-area algorithm
void fillingScanlineSeed(GrayImage &im, const QPoint &seed, uint8_t val);

#endif // BASIC_CG_H
