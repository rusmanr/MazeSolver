#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "maze.h"
#include "mazesolver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generateMaze_btn_clicked();
    void on_solve_btn_clicked();
    void updateView();

private:
    Ui::MainWindow *ui;
    Maze* maze;
    MazeSolver* mazeSolver;
    QTimer *timer;
    int currentPosition;
};

#endif // MAINWINDOW_H
