#include "basic_cg.h"
#include <cstdint>

GrayImage::GrayImage(size_t width, size_t height)
{
    this->width = width;
    this->height = height;
    this->data = new uint8_t* [height];

    for (size_t i=0; i<height; i++) {
        this->data[i] = new uint8_t[width]();
    }
}

GrayImage::~GrayImage()
{
    for (size_t i=0; i<height; i++) {
        delete[] data[i];
    }

    delete[] data;
}

QImage GrayImage::toQImage() {
    QImage qi(width, height, QImage::Format_Grayscale8);

    for (size_t i=0; i<height; i++) {
        uchar *line = qi.scanLine(i);

        for (size_t j=0; j<width; j++) {
            line[j] = data[i][j];
        }
    }

    return qi;
}
