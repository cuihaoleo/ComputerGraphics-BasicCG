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

    inline void setPixel(int x, int y, uint8_t val) {
        data[y][x] = val;
    }

    inline void setPixel(int x, int y) {
        setPixel(x, y, 0);
    }

    QImage toQImage();
};

#endif // BASIC_CG_H
