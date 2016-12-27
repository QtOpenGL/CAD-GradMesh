#include "mousehandler.h"

MouseHandler::MouseHandler(ControlNet *_controlNet)
  : controlNet(_controlNet)
{
}

void MouseHandler::mousePressEvent(QMouseEvent *event) {

  float xRatio, yRatio, xScene, yScene;

  xRatio = (float)event->x() / width;
  yRatio = (float)event->y() / height;

  xScene = 2 * xRatio - 1;
  yScene = 1 - 2 * yRatio;

  switch (event->buttons()) {
  case Qt::LeftButton:
    // Select control point
    selectedPt = findClosest(xScene, yScene);
    break;
  }

}

void MouseHandler::mouseMoveEvent(QMouseEvent *event) {

  if (selectedPt < 0)
    return;

  float xRatio, yRatio, xScene, yScene;

  xRatio = (float)event->x() / width;
  yRatio = (float)event->y() / height;

  xScene = (1-xRatio)*-1 + xRatio*1;
  yScene = yRatio*-1 + (1-yRatio)*1;

  // Update position of the control point
  controlNet->mesh->Vertices[selectedPt].coords = QVector2D(xScene, yScene);
  controlNet->buildCoords();

}

short int MouseHandler::findClosest(float _x, float _y) {

  short int ptIndex;
  float currentDist, minDist = 4;

  for (size_t k = 0; k < (size_t)controlNet->mesh->Vertices.size(); k++) {
    currentDist = pow(( controlNet->mesh->Vertices[k].coords[0] - _x),2) + pow(( controlNet->mesh->Vertices[k].coords[1] - _y),2);
    if (currentDist < minDist) {
      minDist = currentDist;
      ptIndex = k;
    }
  }

  return ptIndex;
}
