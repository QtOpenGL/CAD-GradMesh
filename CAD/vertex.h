#ifndef VERTEX
#define VERTEX

#include <QVector2D>
#include <QDebug>

// Forward declaration
class HalfEdge;

class Vertex {
public:
  QVector2D coords;
  HalfEdge* out;
  unsigned short val;
  unsigned int index;
  unsigned short sharpness;

  // Inline constructors
  Vertex() {
    // qDebug() << "Default Vertex Constructor";
    coords = QVector2D();
    out = nullptr;
    val = 0;
    index = 0;
    sharpness = 0;
  }

  Vertex(QVector2D &vcoords, HalfEdge* vout, unsigned short vval, unsigned int vindex, float vsharpness = 0) {
    //qDebug() << "QVector3D Vertex Constructor";
    coords = vcoords;
    out = vout;
    val = vval;
    index = vindex;
    sharpness = vsharpness;
  }
};

#endif // VERTEX
