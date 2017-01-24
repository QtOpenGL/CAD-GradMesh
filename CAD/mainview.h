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
  void paintGL();

  void ternarySubdiv(){
    controlNet->ternarySubdiv();
  }

  bool showTernaryPoints;
protected:
  void initializeGL();

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:

  MouseHandler *mouseHandler;
  QOpenGLDebugLogger* debugLogger;
  QOpenGLShaderProgram* mainShaderProg,* blackShaderProg,* whiteShaderProg;

  GLuint netVAO, netCoordsBO, netIndexBO, netColourBO;
  GLuint terVAO, terCoordsBO, terColourBO;
  GLuint graVAO, graCoordsBO, graIndexBO;

  void createShaderPrograms();
  void createBuffers();
  void resizeGL(int newWidth, int newHeight);

private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
