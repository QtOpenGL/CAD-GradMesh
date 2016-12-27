#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLWidget>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include <QVector2D>
#include <QMouseEvent>
#include "controlnet.h"
#include "mousehandler.h"

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

  Q_OBJECT

public:
  MainView(QWidget *parent = 0);
  ~MainView();

  unsigned int maxInt = ((unsigned int) -1);
  void updateBuffers();
  ControlNet *controlNet;



protected:
  void initializeGL();
  void paintGL();

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:

  MouseHandler *mouseHandler;

  QOpenGLDebugLogger* debugLogger;

  QOpenGLShaderProgram* mainShaderProg;
  GLuint netVAO, netCoordsBO, netIndexBO;

  void createShaderPrograms();
  void createBuffers();

private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
