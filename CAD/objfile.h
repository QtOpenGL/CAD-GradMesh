#ifndef OBJFILE_H
#define OBJFILE_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class OBJFile {

public:
  OBJFile(QString fileName);
  ~OBJFile();

  QVector<QVector2D> vertexCoords;
  QVector<unsigned short> faceValences;
  QVector<unsigned int> faceCoordInd;

};

#endif // OBJFILE_H
