#ifndef CONTROLNET_H
#define CONTROLNET_H

#include <QVector>
#include <QVector2D>

#include "mesh.h"
#include "ternary.h"

class ControlNet
{
public:
    ControlNet();
    Mesh *mesh;
    Mesh *terMesh;

    QVector<unsigned int> *indices;
    QVector<unsigned int> *graIndices;
    QVector<QVector2D> *coords;
    QVector<QVector2D> *colGrads;
    QVector<QVector3D> *colours;
    QVector <QVector2D> *terCoords;
    QVector <QVector3D> *terColours;

    void ternarySubdiv();

    unsigned int maxInt = ((unsigned int) -1);

    inline size_t size(){ return coords->size(); }
    inline QVector2D *data(){ return coords->data(); }
    void showCoords();
    void buildCoords();
    void updateColours();

};

#endif // CONTROLNET_H
