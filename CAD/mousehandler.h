#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <QMouseEvent>
#include "controlnet.h"


class MouseHandler
{
public:
    MouseHandler(ControlNet *_controlNet);
    int selectedPt = -1;
    int height, width;

    ControlNet *controlNet;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    short int findClosest(float x, float y);

};

#endif // MOUSEHANDLER_H
