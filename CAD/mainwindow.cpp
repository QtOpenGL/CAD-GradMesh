#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);

}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  delete ui;
}

void MainWindow::on_pushButton_released()
{
    ui->mainView->controlNet->showCoords();
}

void MainWindow::on_ImportOBJ_clicked() {
  OBJFile newModel = OBJFile(QFileDialog::getOpenFileName(this, "Import OBJ File", "models/", tr("Obj Files (*.obj)")));
  delete ui->mainView->controlNet->mesh;
  ui->mainView->controlNet->mesh = new Mesh(&newModel);
  ui->mainView->controlNet->buildCoords();
  ui->mainView->updateBuffers();
}
