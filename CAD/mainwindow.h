#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_pushButton_released();
  void on_ImportOBJ_clicked();                 // Load object

  void on_ternaryStepPB_released();

  void on_showTernaryCB_toggled(bool checked);

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
