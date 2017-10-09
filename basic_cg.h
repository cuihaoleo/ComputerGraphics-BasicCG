#ifndef BASIC_CG_H
#define BASIC_CG_H

#include <cstdint>
#include <QImage>

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

    inline void setPixel(int x, int y, uint8_t val) {
        data[y][x] = val;
    }

    inline void setPixel(int x, int y) {
        setPixel(x, y, 0);
    }

    QImage toQImage() const;
};

// Line-drawing algorithm
void lineBresenham(GrayImage &im, int xa, int ya, int xb, int yb);
// Circle-drawing algorithm
void circleMidpoint(GrayImage &im, int cx, int cy, int radius);
// Ellipse-drawing algorithm
void ellipseMidpoint(GrayImage &im, int cx, int cy, int rx, int ry);
// Filled-area algorithm
void fillingScanlineSeed(GrayImage &im, int sx, int sy, uint8_t val);

#endif // BASIC_CG_H
