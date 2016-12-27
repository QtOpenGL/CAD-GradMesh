#include "mainview.h"
#include "math.h"

MainView::MainView(QWidget *parent)
    : QOpenGLWidget(parent)
    , controlNet( new ControlNet )
    , mouseHandler( new MouseHandler(controlNet) )
{
    qDebug() << "✓✓ MainView constructor";
}

MainView::~MainView() {
    qDebug() << "✗✗ MainView destructor";

    glDeleteBuffers(1, &netCoordsBO);
    glDeleteVertexArrays(1, &netVAO);

    delete mainShaderProg;

    debugLogger->stopLogging();
}

// ---

void MainView::createShaderPrograms() {

    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    mainShaderProg->link();
}

void MainView::createBuffers() {

    // Pure OpenGL
    glGenVertexArrays(1, &netVAO);
    glBindVertexArray(netVAO);

    glGenBuffers(1, &netCoordsBO);
    glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &netIndexBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, netIndexBO);

    glBindVertexArray(0);
}

void MainView::updateBuffers() {

  glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*controlNet->size(), controlNet->data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, netIndexBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*controlNet->indices->size(), controlNet->indices->data(), GL_DYNAMIC_DRAW);

  update();
}

void MainView::initializeGL() {

    initializeOpenGLFunctions();
    qDebug() << ":: OpenGL initialized";

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

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
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Draw control net
    glDrawElements(GL_LINE_LOOP, controlNet->indices->size(), GL_UNSIGNED_INT, 0);
    glPointSize(8.0);
    glDrawArrays(GL_POINTS, 0, controlNet->size());

    // Highlight selected control point
    if (mouseHandler->selectedPt > -1) {
        glPointSize(12.0);
        glDrawArrays(GL_POINTS, mouseHandler->selectedPt, 1);
    }

    glBindVertexArray(0);


    mainShaderProg->release();
}

void MainView::mousePressEvent(QMouseEvent *event) {
    mouseHandler->width = width();
    mouseHandler->height = height();
    mouseHandler->mousePressEvent(event);
}
void MainView::mouseMoveEvent(QMouseEvent *event) {
    mouseHandler->width = width();
    mouseHandler->height = height();
    mouseHandler->mouseMoveEvent(event);
}
void MainView::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    mouseHandler->selectedPt = -1;
}
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
