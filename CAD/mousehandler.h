#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <QMouseEvent>
#include <QWidget>
#include "controlnet.h"

class MainView;

class MouseHandler
{
public:
  MouseHandler(ControlNet *_controlNet, MainView *_widget);
  int selectedPt = -1;
  int height, width;

  ControlNet *controlNet;

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  short int findClosest(float x, float y);
  MainView *mainview;
};

#endif // MOUSEHANDLER_H
