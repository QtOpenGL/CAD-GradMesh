#include "ternary.h"

#include "halfedge.h"
#include "vertex.h"
#include "face.h"

QVector2D centroid(Face *face){
    QVector2D center;
    HalfEdge *currentEdge = face->side;
    size_t n = face->val;
    for (size_t i = 0; i < n; ++i){
        center += currentEdge->target->coords;
        currentEdge = currentEdge->next;
    }
    return center / n;
}

void setNext(HalfEdge *next, HalfEdge *prev){
    next->prev = prev;
    prev->next = next;
}

void setTwin(HalfEdge *twin1, HalfEdge *twin2){
    twin1->twin = twin2;
    twin2->twin = twin1;
}

void ternaryStep(Mesh *inputMesh, Mesh *outputMesh){

    size_t k, l;
    size_t vertCount = inputMesh->Vertices.size();
    size_t edgeCount = inputMesh->HalfEdges.size();
    size_t faceCount = inputMesh->Faces.size();

    size_t sumFaceVal = 0;
    size_t newFaceCount = 0;

    for (k = 0; k < faceCount; ++k){
        sumFaceVal += inputMesh->Faces[k].val;
        newFaceCount += 1 + 2 * inputMesh->Faces[k].val;
    }

    size_t newVertCount = vertCount + edgeCount + sumFaceVal;
    outputMesh->Vertices.clear();
    outputMesh->Vertices.reserve(newVertCount);

    for (k = 0; k < vertCount; ++k)
        outputMesh->Vertices.append(inputMesh->Vertices[k]); // out pointer is nonsense at this point

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
                                            currentEdge->target->colour
                                            ));
            currentEdge = currentEdge->next;

          }
    }

    size_t newEdgeCount = 3 * edgeCount + 6 * sumFaceVal;
    outputMesh->HalfEdges.clear();
    outputMesh->HalfEdges.reserve(newEdgeCount);

//    HalfEdge(Vertex* htarget, HalfEdge* hnext, HalfEdge* hprev, HalfEdge* htwin, Face* hpolygon, unsigned int hindex, float hsharpness=0) {
    for (k = 0; k < edgeCount; ++k){
        currentEdge = &inputMesh->HalfEdges[k];
        outputMesh->HalfEdges.append(HalfEdge(
                                         &outputMesh->Vertices[vertCount + currentEdge->index],
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     3 * k));

        outputMesh->HalfEdges.append(HalfEdge(
                                         &outputMesh->Vertices[vertCount + currentEdge->twin->index],
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     3 * k + 1));

        outputMesh->HalfEdges.append(HalfEdge(
                                         &outputMesh->Vertices[currentEdge->target->index],
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     3 * k + 2));

        // /|    /|
        //  |     |  2
        //  |     |
        //  |    /|
        //  | ->  |  1
        //  |     |
        //  |    /|
        //  |     |  0
        //  |     |
    }
    size_t twinIndex;
    for (k = 0; k < edgeCount; ++k){
        currentEdge = &inputMesh->HalfEdges[k];
        twinIndex = currentEdge->twin->index;
        outputMesh->HalfEdges[3 * k].twin     = &outputMesh->HalfEdges[3 * twinIndex + 2];
        outputMesh->HalfEdges[3 * k + 1].twin = &outputMesh->HalfEdges[3 * twinIndex + 1];
        outputMesh->HalfEdges[3 * k + 2].twin = &outputMesh->HalfEdges[3 * twinIndex];

        outputMesh->HalfEdges[3 * k].prev     = &outputMesh->HalfEdges[3 * currentEdge->prev->index + 2];
        outputMesh->HalfEdges[3 * k + 2].next = &outputMesh->HalfEdges[3 * currentEdge->next->index];

        if (not currentEdge->polygon){
            setNext(&outputMesh->HalfEdges[3 * k + 1], &outputMesh->HalfEdges[3 * k]);
            setNext(&outputMesh->HalfEdges[3 * k + 2], &outputMesh->HalfEdges[3 * k + 1]);
        }
    }

    Face *currentFace;
    size_t n;
    size_t edgeIndex = 3 * edgeCount;
    size_t vertIndex = vertCount + edgeCount;
    size_t currentIndex;
    size_t nextIndex;
    size_t edgesAdded;
    for (k = 0; k < faceCount; ++k){
        currentFace = &inputMesh->Faces[k];
        n = currentFace->val;
        currentEdge = currentFace->side;

        for (l = 0; l < n; ++l) {
            currentIndex = currentEdge->index;
            nextIndex = currentEdge->next->index;
            twinIndex = currentEdge->twin->index;

            outputMesh->HalfEdges.append(HalfEdge(&outputMesh->Vertices[vertIndex]            , nullptr, nullptr, nullptr, nullptr, edgeIndex    ));
            outputMesh->HalfEdges.append(HalfEdge(&outputMesh->Vertices[vertIndex]            , nullptr, nullptr, nullptr, nullptr, edgeIndex + 1));
            outputMesh->HalfEdges.append(HalfEdge(&outputMesh->Vertices[vertCount + nextIndex], nullptr, nullptr, nullptr, nullptr, edgeIndex + 2));
            outputMesh->HalfEdges.append(HalfEdge(&outputMesh->Vertices[vertIndex]            , nullptr, nullptr, nullptr, nullptr, edgeIndex + 3));
            outputMesh->HalfEdges.append(HalfEdge(&outputMesh->Vertices[vertCount + twinIndex], nullptr, nullptr, nullptr, nullptr, edgeIndex + 2));
            outputMesh->HalfEdges.append(HalfEdge(&outputMesh->Vertices[vertIndex]            , nullptr, nullptr, nullptr, nullptr, edgeIndex + 5));

            setNext( &outputMesh->HalfEdges[edgeIndex + 2]       , &outputMesh->HalfEdges[edgeIndex + 1]       );
            setNext( &outputMesh->HalfEdges[3 * nextIndex + 1]   , &outputMesh->HalfEdges[edgeIndex + 2]       );
            setNext( &outputMesh->HalfEdges[edgeIndex + 4]       , &outputMesh->HalfEdges[edgeIndex + 3]       );
            setNext( &outputMesh->HalfEdges[edgeIndex + 3]       , &outputMesh->HalfEdges[3 * nextIndex]       );
            setNext( &outputMesh->HalfEdges[3 * currentIndex + 2], &outputMesh->HalfEdges[edgeIndex + 4]       );
            setNext( &outputMesh->HalfEdges[edgeIndex + 5]       , &outputMesh->HalfEdges[3 * currentIndex + 1]);

            setTwin( &outputMesh->HalfEdges[edgeIndex + 2], &outputMesh->HalfEdges[edgeIndex + 3] );
            setTwin( &outputMesh->HalfEdges[edgeIndex + 4], &outputMesh->HalfEdges[edgeIndex + 5] );

            ++vertIndex;
            edgeIndex += 6;
            currentEdge = currentEdge->next;
        }

        edgesAdded = 6 * n;
        edgeIndex -= edgesAdded;

        setTwin( &outputMesh->HalfEdges[edgeIndex], &outputMesh->HalfEdges[edgeIndex + 6 * (n - 1) + 1] );
        setNext( &outputMesh->HalfEdges[edgeIndex + 6 * (n - 1) + 1], &outputMesh->HalfEdges[edgeIndex + 5] );

        for (l = 0; l < n - 1; ++l){
            setTwin( &outputMesh->HalfEdges[edgeIndex + 1], &outputMesh->HalfEdges[edgeIndex + 6] );
            setNext( &outputMesh->HalfEdges[edgeIndex + 1], &outputMesh->HalfEdges[edgeIndex + 11] );
            edgeIndex += 6;
        }
        edgeIndex += 6;
    }

    for (int i = 0; i < outputMesh->HalfEdges.size(); ++i){
        currentEdge = &outputMesh->HalfEdges[k];
        if (not (currentEdge->prev->next == currentEdge)){
            qDebug() << "fout";
        }

    }

}
