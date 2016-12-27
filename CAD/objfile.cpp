#include "objfile.h"

#include <QDebug>
#include <QFile>

OBJFile::OBJFile(QString fileName) {
  qDebug() << "✓✓ OBJFile constructor";

  qDebug() << ":: Loading" << fileName;
  QFile newModel(fileName);

  if(newModel.open(QIODevice::ReadOnly)) {
    QTextStream fileContents(&newModel);

    QString currentLine;
    QStringList values;
    QStringList indices;

    unsigned short k;

    vertexCoords.clear();
    faceCoordInd.clear();

    while(!fileContents.atEnd()) {

      currentLine = fileContents.readLine();
      values = currentLine.split(" ");

      if (values[0] == "v") {
        // qDebug() << "Vertex coords";
        // Only x, y and z. If there's a w value (homogenous coordinates), ignore it.
        vertexCoords.append(QVector2D(values[1].toFloat(), values[2].toFloat() ));
      }
      else if (values[0] == "f") {
        // qDebug() << "Face";

        for (k=1; k<values.size(); k++) {
          indices = values[k].split("/");

          // Note -1, OBJ starts indexing from 1.
          faceCoordInd.append(indices[0].toInt() -1 );
        }

        faceValences.append(k-1);

      }
      else {
        qDebug() << " * Line contents ignored," << currentLine;
      }

    }

    newModel.close();

  }

}

OBJFile::~OBJFile() {
  qDebug() << "✗✗ OBJFile destructor";
}
