#include "mainview.h"
#include "math.h"

MainView::MainView(QWidget *parent)
    : QOpenGLWidget(parent)
    , controlNet( new ControlNet )
    , mouseHandler( new MouseHandler(controlNet, this) )
{
    qDebug() << "✓✓ MainView constructor";
    mouseHandler->width = width();
    mouseHandler->height = height();
}

MainView::~MainView() {
  qDebug() << "✗✗ MainView destructor";

  glDeleteBuffers(1, &netCoordsBO);
  glDeleteVertexArrays(1, &netVAO);

  delete mainShaderProg;
  delete blackShaderProg;

  debugLogger->stopLogging();
}

void MainView::createShaderPrograms() {

  mainShaderProg = new QOpenGLShaderProgram();
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

  mainShaderProg->link();

  blackShaderProg = new QOpenGLShaderProgram();
  blackShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  blackShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_black.glsl");

  blackShaderProg->link();
}

void MainView::createBuffers() {

  glGenVertexArrays(1, &netVAO);
  glBindVertexArray(netVAO);

  glGenBuffers(1, &netCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &netColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, netColourBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &netIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, netIndexBO);

  glBindVertexArray(0);

  // Colour gradients

  glGenVertexArrays(1, &graVAO);
  glBindVertexArray(graVAO);

  glGenBuffers(1, &graCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, graCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &graIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graIndexBO);

  // Ternary step

  glGenVertexArrays(1, &terVAO);
  glBindVertexArray(terVAO);

  glGenBuffers(1, &terCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, terCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &terColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, terColourBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
}

void MainView::updateBuffers() {

  glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*controlNet->size(), controlNet->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, netColourBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*controlNet->colours->size(), controlNet->colours->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, netIndexBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*controlNet->indices->size(), controlNet->indices->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, graCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*controlNet->colGrads->size(), controlNet->colGrads->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graIndexBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*controlNet->graIndices->size(), controlNet->graIndices->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, terCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*controlNet->terCoords->size(), controlNet->terCoords->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, terColourBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*controlNet->terColours->size(), controlNet->terColours->data(), GL_DYNAMIC_DRAW);

  update();
}

void MainView::resizeGL(int newWidth, int newHeight) {

  Q_UNUSED(newWidth);
  Q_UNUSED(newHeight);

  qDebug() << ".. resizeGL";

  mouseHandler->width = width();
  mouseHandler->height = height();
}

void MainView::initializeGL() {

  initializeOpenGLFunctions();
  qDebug() << ":: OpenGL initialized";

  debugLogger = new QOpenGLDebugLogger();
  connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

//  if ( debugLogger->initialize() ) {
//    qDebug() << ":: Logging initialized";
//    debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
//    debugLogger->enableMessages();
//  }

  QString glVersion;
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(maxInt);

  createShaderPrograms();
  createBuffers();
}

void MainView::paintGL() {
  updateBuffers();

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mainShaderProg->bind();

  glBindVertexArray(netVAO);

  // Draw control net
  glDrawElements(GL_TRIANGLE_FAN, controlNet->indices->size(), GL_UNSIGNED_INT, 0);

    if (controlNet->terCoords->size() && showTernaryPoints){
        glBindVertexArray(terVAO);

        blackShaderProg->bind();
        glPointSize(6.0);
        glDrawArrays(GL_POINTS, 0, controlNet->terCoords->size());

        mainShaderProg->bind();
        glPointSize(4.0);
        glDrawArrays(GL_POINTS, 0, controlNet->terCoords->size());
      }



    // Highlight selected control point
    if (mouseHandler->selectedPt > -1) {

      glPointSize(4.0);
      glDrawArrays(GL_LINES, 0, controlNet->colGrads->size());

      blackShaderProg->bind();
      glPointSize(8.0);
      glDrawArrays(GL_POINTS, mouseHandler->selectedPt, 1);

      mainShaderProg->bind();
      glPointSize(6.0);
      glDrawArrays(GL_POINTS, mouseHandler->selectedPt, 1);

    }


  glBindVertexArray(0);

  mainShaderProg->release();
}

void MainView::mousePressEvent(QMouseEvent *event) {
  mouseHandler->mousePressEvent(event);
}
void MainView::mouseMoveEvent(QMouseEvent *event) {
  mouseHandler->mouseMoveEvent(event);
}
void MainView::mouseReleaseEvent(QMouseEvent *event){
  Q_UNUSED(event);
//  mouseHandler->selectedPt = -1;
}
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
  qDebug() << " → Log:" << Message;
}
