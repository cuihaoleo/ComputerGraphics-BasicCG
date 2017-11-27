#include "rview.h"

#include <QVector2D>
#include <QMatrix4x4>

#include <cmath>

static QMatrix4x4 eulerRotateZXZ(double alpha, double beta, double gamma) {
    using namespace std;
    double c1 = cos(alpha), s1 = sin(alpha);
    double c2 = cos(beta),  s2 = sin(beta);
    double c3 = cos(gamma), s1 = sin(gamma);
    QMatrix4x4 transform(
                c1*c3-c2*s1*s3, -c1*s3-c2*c3*s1, s1*s2,  0,
                c3*s1+c1*c2*s3, c1*c2*c3-s1*s3,  -c1*s2, 0,
                s2*s3,          c3*s2,           c2,     0,
                0,              0,               0,      1);
    return transform;
}

RView::RView(double radius, double alpha, double beta, double gamma)
{
    QMatrix4x4 transform = eulerRotateZXZ(alpha, beta, gamma);

    QVector3D initViewPoint(0, 0, radius);
    QVector3D initViewUp(0, 1, 0);
    QVector3D viewPoint = transform.map(initViewPoint);
    QVector3D viewUp = transform.map(initViewUp);

    this->viewPoint = viewPoint;
    this->viewUp = viewUp;
}

void RView::lookAt(RSence scene)
{

}
