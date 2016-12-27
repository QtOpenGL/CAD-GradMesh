#include "controlnet.h"

ControlNet::ControlNet()
  : mesh(nullptr)
  , indices(new QVector<unsigned int>)
  , coords(new QVector<QVector2D>)
{}

void ControlNet::showCoords()
{
  qDebug() << "\nControl mesh coordinates:";
  for (int i = 0; i < mesh->Vertices.size(); ++i)
    mesh->dispVertInfo(i);

}

void ControlNet::buildCoords(){

  unsigned int k;
  unsigned short n, m;
  HalfEdge* currentEdge;

  Mesh *currentMesh = mesh;
  coords->clear();
  coords->reserve(currentMesh->Vertices.size());

  for (k=0; k<(unsigned int)currentMesh->Vertices.size(); ++k)
    coords->append(currentMesh->Vertices[k].coords);

  indices->clear();
  indices->reserve(currentMesh->HalfEdges.size() + currentMesh->Faces.size());

  for (k=0; k<(unsigned int)currentMesh->Faces.size(); k++) {
      n = currentMesh->Faces[k].val;
      currentEdge = currentMesh->Faces[k].side;
      for (m=0; m<n; m++) {
          indices->append(currentEdge->target->index);
          currentEdge = currentEdge->next;
      }
      indices->append(maxInt);
  }

}
