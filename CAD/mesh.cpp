#include "mesh.h"
#include "math.h"

Mesh::Mesh() {
  qDebug() << "✓✓ Mesh constructor (Empty)";
}

Mesh::Mesh(OBJFile* loadedOBJFile) {

  qDebug() << "✓✓ Mesh constructor (OBJ)";

  // Convert loaded OBJ file to HalfEdge mesh
  size_t numVertices, numHalfEdges, numFaces;
  size_t k, m, n;

  numVertices = loadedOBJFile->vertexCoords.size();
  numHalfEdges = 0;

  for (k=0; k<(size_t)loadedOBJFile->faceValences.size(); k++) {
    numHalfEdges += loadedOBJFile->faceValences[k];
  }

  numFaces = loadedOBJFile->faceValences.size();

  // Note - resize() invokes the Vertex() constructor, reserve() does not.
  Vertices.reserve(numVertices);
  // If boundaries are present, reserve twice as much = worst case scenario
  HalfEdges.reserve(2*numHalfEdges);
  Faces.reserve(numFaces);

  // Add Vertices

  for (k=0; k<numVertices; k++) {
    // Coords (x,y,z), Out, Valence, Index
    Vertices.append(Vertex(loadedOBJFile->vertexCoords[k],
                           nullptr,
                           0,
                           k));
    // Out and valence are unknown at this point.
  }

  qDebug() << "   # Vertices" << Vertices.capacity() << Vertices.size();

  size_t indexH = 0;
  size_t currentIndex = 0;

  // Initialize every entry of PotentialTwins with an empty QVector (using resize() )
  PotentialTwins.resize(loadedOBJFile->vertexCoords.size());

  // Add Faces and most of the HalfEdges

  for (m=0; m<numFaces; m++) {
    // Side, Val, Index
    Faces.append(Face(nullptr,
                      loadedOBJFile->faceValences[m],
                      m));

    for (n=0; n<loadedOBJFile->faceValences[m]; n++) {
      // Target, Next, Prev, Twin, Poly, Index
      HalfEdges.append(HalfEdge(&Vertices[loadedOBJFile->faceCoordInd[currentIndex+n]],
                                nullptr,
                                nullptr,
                                nullptr,
                                &Faces[m],
                                indexH));

      // Next, Prev and Twin of the above HalfEdge have to be assigned later! Starting below...

      if (n > 0) {
        HalfEdges[indexH-1].next = &HalfEdges[indexH];
        HalfEdges[indexH].prev = &HalfEdges[indexH-1];

        // Append index of HalfEdge to list of OutgoingHalfEdges of its TailVertex.
        PotentialTwins[loadedOBJFile->faceCoordInd[currentIndex+n-1]].append(indexH);
      }
      indexH++;
    }

    // HalfEdges[indexH-1] is the most recent addition.
    Faces[m].side = &HalfEdges[indexH-1];

    HalfEdges[indexH-1].next = &HalfEdges[indexH-n];
    HalfEdges[indexH-n].prev = &HalfEdges[indexH-1];

    PotentialTwins[loadedOBJFile->faceCoordInd[currentIndex+n-1]].append(indexH-n);

    currentIndex += loadedOBJFile->faceValences[m];
  }

  qDebug() << "   # Faces" << Faces.capacity() << Faces.size();
  qDebug() << "   # HalfEdges" << HalfEdges.capacity() << HalfEdges.size();

  // Outs and Valences of vertices
  for (k=0; k<(size_t)Vertices.size(); k++) {
    if (PotentialTwins[k].size() == 0) {
      qWarning() << " ! Isolated Vertex? PotentialTwins empty for Index" << k;
      dispVertInfo(k);
      continue;
    }
    Vertices[k].out = &HalfEdges[PotentialTwins[k][0]];
    // Not the correct valence when on the boundary! Fixed below.
    Vertices[k].val = PotentialTwins[k].size();
  }

  setTwins(numHalfEdges, indexH);

  PotentialTwins.clear();
  PotentialTwins.squeeze();

  qDebug() << "   # Updated HalfEdges" << HalfEdges.capacity() << HalfEdges.size();

  HalfEdge *currentEdge;
  for (int i = 0; i < HalfEdges.size(); ++i){
    currentEdge = &HalfEdges[i];
    currentEdge->colGrad = 0.33 * (currentEdge->target->coords - currentEdge->twin->target->coords);
  }

}

Mesh::~Mesh() {
  qDebug() << "✗✗ Mesh destructor";

  qDebug() << "   # Vertices:" << Vertices.size();
  qDebug() << "   # HalfEdges:" << HalfEdges.size();
  qDebug() << "   # Faces:" << Faces.size();

  Vertices.clear();
  Vertices.squeeze();
  HalfEdges.clear();
  HalfEdges.squeeze();
  Faces.clear();
  Faces.squeeze();
}

void Mesh::setTwins(size_t numHalfEdges, size_t indexH) {

  size_t m, n;
  size_t hTail, hHead, len;
  QSet<size_t> Twinless;

  // Assign Twins
  for (m=0; m<numHalfEdges; m++) {
    if (HalfEdges[m].twin == nullptr) {
      hTail = HalfEdges[m].prev->target->index;
      hHead = HalfEdges[m].target->index;
      len = HalfEdges[m].target->val;
      for (n=0; n<len; n++) {
        if (HalfEdges[PotentialTwins[hHead][n]].target->index == hTail) {
          //qDebug() << "Found Twin!";
          HalfEdges[m].twin = &HalfEdges[PotentialTwins[hHead][n]];
          HalfEdges[PotentialTwins[hHead][n]].twin = &HalfEdges[m];
          break;
        }
      }
      if (n == len) {
        // Twin not found...
        Twinless.insert(m);
      }
    }
  }

  if (Twinless.size() > 0) {

    HalfEdge* initialEdge;
    HalfEdge* currentEdge;
    size_t startBoundaryLoop;

    while (Twinless.size() > 0) {
      // Select a HalfEdge without Twin. The Twin that we will create is part of a boundary edge loop
      qDebug() << " → Processing new Boundary Edge Loop";

      initialEdge = &HalfEdges[*Twinless.begin()];
      Twinless.remove(initialEdge->index);

      // Target, Next, Prev, Twin, Poly, Index
      HalfEdges.append(HalfEdge( initialEdge->prev->target,
                                 nullptr,
                                 nullptr,
                                 initialEdge,
                                 nullptr,
                                 indexH ));
      startBoundaryLoop = indexH;
      // Twin of initialEdge should be assigned AFTER the central while loop!
      indexH++;

      // Use a sketch to properly understand these steps (assume counter-clockwise HalfEdges) :)
      currentEdge = initialEdge->prev;
      while (currentEdge->twin != nullptr) {
        currentEdge = currentEdge->twin->prev;
      }

      // Trace the current boundary loop
      while (currentEdge != initialEdge) {
        Twinless.remove(currentEdge->index);

        // Target, Next, Prev, Twin, Poly, Index
        HalfEdges.append(HalfEdge( currentEdge->prev->target,
                                   nullptr,
                                   &HalfEdges[indexH-1],
                                   currentEdge,
                                   nullptr,
                                   indexH ));
        HalfEdges[indexH-1].next = &HalfEdges[indexH];

        currentEdge->target->val += 1;
        currentEdge->twin = &HalfEdges[indexH];
        indexH++;

        currentEdge = currentEdge->prev;
        while (currentEdge->twin != nullptr) {
          currentEdge = currentEdge->twin->prev;
        }
      }

      HalfEdges[startBoundaryLoop].prev = &HalfEdges[indexH-1];
      HalfEdges[indexH-1].next = &HalfEdges[startBoundaryLoop];

      initialEdge->target->val += 1;
      // Set Twin of initialEdge!
      initialEdge->twin = &HalfEdges[startBoundaryLoop];
    }

  }

}

void Mesh::dispVertInfo(unsigned short vertIndex) {
  Vertex* dispVert = &Vertices[vertIndex];
  qDebug() << "Vertex at Index =" << dispVert->index << "Coords =" << dispVert->coords << "Out =" << dispVert->out << "Val =" << dispVert->val;
}

void Mesh::dispHalfEdgeInfo(unsigned short edgeIndex) {
  HalfEdge* dispEdge = &HalfEdges[edgeIndex];
  qDebug() << "HalfEdge at Index =" << dispEdge->index << "Target =" << dispEdge->target << "Next =" << dispEdge->next << "Prev =" << dispEdge->prev << "Twin =" << dispEdge->twin << "Poly =" << dispEdge->polygon;
}

void Mesh::dispFaceInfo(unsigned short faceIndex){
  Face* dispFace = &Faces[faceIndex];
  qDebug() << "Face at Index =" << dispFace->index << "Side =" << dispFace->side << "Val =" << dispFace->val;
}
