#ifndef CONTROLNET_H
#define CONTROLNET_H

#include <QVector>
#include <QVector2D>

#include "mesh.h"

class ControlNet
{
public:
    ControlNet();
    QVector<QVector2D> *coords;
    Mesh *mesh;

    inline size_t size(){ return coords->size(); }
    inline QVector2D *data(){ return coords->data(); }

};

#endif // CONTROLNET_H
