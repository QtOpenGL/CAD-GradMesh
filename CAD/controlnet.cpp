#include "controlnet.h"

ControlNet::ControlNet()
  : mesh(nullptr)
  , terMesh( new Mesh )
  , indices(new QVector<unsigned int>)
  , graIndices(new QVector<unsigned int>)
  , coords(new QVector<QVector2D>)
  , colGrads(new QVector<QVector2D>)
  , colours(new QVector<QVector3D>)
  , terCoords( new QVector <QVector2D>)
  , terColours( new QVector <QVector3D>)
{}

void ControlNet::showCoords()
{
  qDebug() << "\nControl mesh coordinates:";
  for (int i = 0; i < mesh->Vertices.size(); ++i)
    mesh->dispVertInfo(i);

}

void ControlNet::updateColours(){
  unsigned int k;
  Mesh *currentMesh = mesh;

  colours->clear();
  colours->reserve(currentMesh->Vertices.size());

  for (k=0; k<(unsigned int)currentMesh->Vertices.size(); ++k)
    colours->append(currentMesh->Vertices[k].colour);

}

void ControlNet::ternarySubdiv(){
  ternaryStep(mesh, terMesh);
  terCoords->clear();
  terCoords->squeeze();
  terColours->clear();
  terColours->squeeze();
  for (int i = 0; i < terMesh->Vertices.size(); ++i){
      terCoords->append(terMesh->Vertices[i].coords);
      terColours->append(terMesh->Vertices[i].colour);
    }
}


void ControlNet::buildCoords(){
  ternarySubdiv();
  unsigned int k;
  unsigned short n, m;
  HalfEdge* currentEdge;

  Mesh *currentMesh = mesh;
  coords->clear();
  coords->reserve(currentMesh->Vertices.size());

  for (k=0; k<(unsigned int)currentMesh->Vertices.size(); ++k)
    coords->append(currentMesh->Vertices[k].coords);

  updateColours();
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

  colGrads->clear();
  colGrads->squeeze();
  graIndices->clear();
  graIndices->squeeze();
  for (k=0; k < currentMesh->HalfEdges.size(); ++k){
    currentEdge = &currentMesh->HalfEdges[k];
    colGrads->append(currentEdge->twin->target->coords);
    colGrads->append(currentEdge->twin->target->coords + currentEdge->colGrad);

    graIndices->append(2 * k);
    graIndices->append(2 * k + 1);
  }

}
