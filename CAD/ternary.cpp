#include "ternary.h"

#include "halfedge.h"
#include "vertex.h"
#include "face.h"

QVector2D centroid(Face *face){
  QVector2D center;
  HalfEdge *currentEdge = face->side;
  size_t n = face->val;
  for (int i = 0; i < n; ++i){
      center += currentEdge->target->coords;
      currentEdge = currentEdge->next;
    }
  return center / n;
}

void ternaryStep(Mesh *inputMesh, Mesh *outputMesh){

  size_t k, l;
  size_t vertCount = inputMesh->Vertices.size();
  size_t edgeCount = inputMesh->HalfEdges.size();
  size_t faceCount = inputMesh->Faces.size();

  outputMesh->Vertices.clear();
  outputMesh->Vertices.squeeze();
  for (k = 0; k < vertCount; ++k){
      outputMesh->Vertices.append(inputMesh->Vertices[k]); // out pointer is nonsense at this point
    }
  HalfEdge *currentEdge;
  HalfEdge *twinEdge;
  QVector2D newPosition;
  for (k = 0; k < edgeCount; ++k){
      currentEdge = &inputMesh->HalfEdges[k];
      twinEdge = currentEdge->twin;
      newPosition = twinEdge->target->coords + currentEdge->colGrad;
      outputMesh->Vertices.append(Vertex(
                                    newPosition,
                                    nullptr,
                                    4,
                                    vertCount + k,
                                    0,
                                    twinEdge->target->colour)
                                  );
    }

  float d1, d2, N1, N2, V01, V02;
  QVector2D V0, V1, V2, D1, D2, C;      // V1 ---- V0 ---- V2

  for (k = 0; k < faceCount; ++k){
      currentEdge = inputMesh->Faces[k].side;
      C = centroid(&inputMesh->Faces[k]);
      for (l = 0; l < inputMesh->Faces[k].val; ++l){
          V1 = currentEdge->prev->target->coords;
          V0 = currentEdge->target->coords;
          V2 = currentEdge->next->target->coords;
          D1 = currentEdge->twin->colGrad;
          D2 = currentEdge->next->colGrad;
          N1 = D1.length();
          N2 = D2.length();
          if (N1 == 0.0 || N2 == 0.0)
            qDebug() << "Zero length colour gradient";

          V01 = QVector2D(V0 - V1).length();
          V02 = QVector2D(V0 - V2).length();

          d1 = 2 * N1 / V01;
          d2 = 2 * N2 / V02;

          newPosition =
              (1 - d1) * (1-d2) * V0 +
              d1 * d2 * C +
              d2 * (1 - d1) * (V0 + V01 * D1 / (2 * N1)) +
              d1 * (1 - d2) * (V0 + V02 * D2 / (2 * N2));

          outputMesh->Vertices.append(Vertex(
                                        newPosition,
                                        nullptr,
                                        4,
                                        0,
                                        0,
                                        currentEdge->target->colour)
                                      );
          currentEdge = currentEdge->next;

        }
    }

}
