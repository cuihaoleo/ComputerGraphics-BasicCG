#include "basic_cg.h"
#include <cstdint>
#include <cstdlib>
#include <algorithm>

GrayImage::GrayImage(size_t width, size_t height, uint8_t bg)
{
    this->width = width;
    this->height = height;
    this->data = new uint8_t* [height];

    for (size_t i=0; i<height; i++) {
        this->data[i] = new uint8_t[width]();
        std::fill_n(this->data[i], width, bg);
    }
}

GrayImage::~GrayImage()
{
    for (size_t i=0; i<height; i++) {
        delete[] data[i];
    }

    delete[] data;
}

void GrayImage::setPixel(int x, int y, uint8_t val) {
    if (x >= 0 && (size_t)x < width
            && y >= 0 && (size_t)y < height)
        data[y][x] = val;
}

QImage GrayImage::toQImage() const {
    QImage qi((int)width, (int)height, QImage::Format_Grayscale8);

    for (size_t i=0; i<height; i++) {
        uchar *line = qi.scanLine((int)i);

        for (size_t j=0; j<width; j++) {
            line[j] = data[i][j];
        }
    }

    return qi;
}

bool isEightNeighbor(const QPoint &a, const QPoint &b) {
    int diff_x = std::abs(a.x() - b.x());
    int diff_y = std::abs(a.y() - b.y());
    return (qMax(diff_x, diff_y) == 1) && (diff_x || diff_y);
}
