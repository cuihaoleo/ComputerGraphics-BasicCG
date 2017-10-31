#include "basic_cg.h"

#include <cstdlib>
#include <algorithm>
#include <cmath>

#include <QStack>
#include <QPair>


void lineBresenham(GrayImage &im, int xa, int ya, int xb, int yb) {
    int dx = xb - xa, dy = yb - ya;
    int adx = std::abs(dx), ady = std::abs(dy);
    int rflag = adx < ady;  // whether |m| = |dy/dx| > 1
    int iflag = (dx < 0) == (dy < 0);  // whether m < 0

    int x, y, xEnd;

    if ((!rflag && xa > xb) || (rflag && ya > yb)) {
        std::swap(xa, xb);
        std::swap(ya, yb);
    }

    if (!rflag) {
        x = xa;
        y = ya;
        xEnd = xb;
        im.setPixel(x, y);
    } else {
        x = ya;
        y = xa;
        xEnd = yb;
        std::swap(adx, ady);
        im.setPixel(y, x);
    }

    int twoDy = 2 * ady, twoDyDx = 2 * (ady - adx);
    int p = twoDy - adx;

    while (x < xEnd) {
        x++;

        if (p < 0)
            p += twoDy;
        else {
            iflag ? y++ : y--;
            p += twoDyDx;
        }

        rflag ? im.setPixel(y, x) : im.setPixel(x, y);
    }
}


void circleMidpoint(GrayImage &im, int cx, int cy, int radius) {
    int dx = 0, dy = radius;
    int p = 1 - radius;

    do {
        im.setPixel(cx + dx, cy + dy);
        im.setPixel(cx - dx, cy + dy);
        im.setPixel(cx + dx, cy - dy);
        im.setPixel(cx - dx, cy - dy);
        im.setPixel(cx + dy, cy + dx);
        im.setPixel(cx - dy, cy + dx);
        im.setPixel(cx + dy, cy - dx);
        im.setPixel(cx - dy, cy - dx);

        dx++;
        if (p < 0)
            p += 2*dx + 1;
        else {
            dy--;
            p += 2 * (dx - dy) + 1;
        }
    } while (dx <= dy);
}


void ellipseMidpoint(GrayImage &im, int cx, int cy, int rx, int ry) {
    const int rx2 = rx * rx;
    const int ry2 = ry * ry;
    const int twoRx2 = 2 * rx2, twoRy2 = 2 * ry2;

    int x = 0, y = ry;
    int p, px = 0, py = twoRx2 * y;

    if (rx == 0 || ry == 0) {
        lineBresenham(im, cx + rx, cy + ry, cx - rx, cy - ry);
        return;
    }

    p = ry2 - rx2 * ry + (int)std::round(0.25 * rx2);
    while (px <= py) {
        im.setPixel(cx-x, cy+y);
        im.setPixel(cx+x, cy+y);
        im.setPixel(cx-x, cy-y);
        im.setPixel(cx+x, cy-y);

        x++;
        px += twoRy2;

        if (p < 0)
            p += ry2 + px;
        else {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }
    }

    p = (int)round(ry2*(x+0.5)*(x+0.5)) + rx2*(y-1)*(y-1) - rx2*ry2;
    do {
        im.setPixel(cx-x, cy+y);
        im.setPixel(cx+x, cy+y);
        im.setPixel(cx-x, cy-y);
        im.setPixel(cx+x, cy-y);

        y--;
        py -= twoRx2;

        if (p > 0)
            p += rx2 - py;
        else {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }
    } while (y >= 0);
}


void fillingScanlineSeed(GrayImage &im, int sx, int sy, uint8_t val) {
    uint8_t oldval = im.getPixel(sx, sy);
    typedef QPair<int, int> Coordinate;
    QStack<Coordinate> stack;

    // move initial seed to leftmost
    for (; im.getPixel(sx, sy) == oldval; sx--)
        continue;
    sx++;

    stack.push(Coordinate(sx, sy));
    while (!stack.isEmpty()) {
        Coordinate seed = stack.pop();
        int yy = seed.second;
        int xLeft = seed.first, xRight;

        if (im.getPixel(xLeft, yy) != oldval)
            continue;

        // go right
        for (xRight=xLeft;
             im.getPixel(xRight, yy) == oldval;
             xRight++)
            continue;
        xRight--;

        for (int x=xLeft; x<=xRight; x++)
            im.setPixel(x, yy, val);

        // find next seed point
        for (int yd = -1; yd <= 1; yd += 2) {
            int x = xRight;
            while (x >= xLeft) {
                if (im.getPixel(x, yy+yd) == oldval) {
                    while (im.getPixel(x, yy+yd) == oldval)
                        x--;
                    stack.push(Coordinate(1+x, yy+yd));
                }
                else
                    while (x >= xLeft && im.getPixel(x, yy+yd) != oldval)
                        x--;
            }
        }
    }
}
