#ifndef CONTROLNET_H
#define CONTROLNET_H

#include <QVector>
#include <QVector2D>

#include "mesh.h"

class ControlNet
{
public:
    ControlNet();
    Mesh *mesh;
    QVector<unsigned int> *indices;
    QVector<QVector2D> *coords;

    unsigned int maxInt = ((unsigned int) -1);

    inline size_t size(){ return coords->size(); }
    inline QVector2D *data(){ return coords->data(); }
    void showCoords();
    void buildCoords();

};

#endif // CONTROLNET_H
