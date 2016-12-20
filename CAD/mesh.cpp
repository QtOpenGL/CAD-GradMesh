#include "mesh.h"

Mesh::Mesh()
    : Vertices(new QVector<Vertex>)
    , Faces(new QVector<Face>)
    , HalfEdges(new QVector<HalfEdge>)
{
}

Mesh::~Mesh(){
    delete Vertices;
    delete HalfEdges;
    delete Faces;
}
