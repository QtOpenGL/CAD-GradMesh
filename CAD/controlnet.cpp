#include "controlnet.h"

ControlNet::ControlNet()
    : coords(new QVector<QVector2D>)
    , mesh(new Mesh)
{
    coords->append(QVector2D(0.5, 0.5));
    coords->append(QVector2D(0.5, -0.5));
    coords->append(QVector2D(-0.5, -0.5));
    coords->append(QVector2D(-0.5, 0.5));
}

