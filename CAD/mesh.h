#ifndef MESH_H
#define MESH_H

#include <QVector>

#include "vertex.h"
#include "face.h"
#include "halfedge.h"
#include "objfile.h"



class Mesh {

public:
  Mesh();
  Mesh(OBJFile* loadedOBJFile);
  ~Mesh();

  QVector<Vertex> Vertices;
  QVector<Face> Faces;
  QVector<HalfEdge> HalfEdges;

  void setTwins(unsigned int numHalfEdges, unsigned int indexH);

  QVector<QVector<unsigned int> > PotentialTwins;

  // For debugging
  void dispVertInfo(unsigned short vertIndex);
  void dispHalfEdgeInfo(unsigned short edgeIndex);
  void dispFaceInfo(unsigned short faceIndex);

};

#endif // MESH_H
