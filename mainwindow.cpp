#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "maze.h"
#include "mazesolver.h"
#include <QDebug>
#include <QRgb>
#include <QTimer>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentPosition = 0;
    timer = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete maze;
    delete mazeSolver;
}

void MainWindow::on_generateMaze_btn_clicked()
{
    int width = ui->width_entry->text().toInt();
    int height = ui->height_entry->text().toInt();
    if (timer != nullptr)
        timer->stop();
    currentPosition = 0;
    try
    {
        maze = new Maze(width,height);
        mazeSolver = new MazeSolver(maze, MazeSolver::SolverType::BACKTRACKING);
    }
    catch (const std::exception& e)
    {
        qDebug() << e.what();
    }


    if (maze != nullptr)
    {
        QImage image(height, width, QImage::Format_Indexed8);
        QRgb blackColor = qRgb(0,0,0);
        QRgb whiteColor = qRgb(255,255,255);
        QRgb blueColor = qRgb(0,0,255);
        QRgb redColor = qRgb(255,0,0);
        image.setColor(0, whiteColor);
        image.setColor(1, blackColor);
        image.setColor(2, blueColor);
        image.setColor(3, redColor);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image.setPixel(i,j,maze->at(i,j));
            }
        }
        image = image.scaled(ui->mazeView->height(),ui->mazeView->width(),Qt::KeepAspectRatio);
        ui->mazeView->setPixmap(QPixmap::fromImage(image));
        ui->solve_btn->setEnabled(true);
    }
}

void MainWindow::on_solve_btn_clicked()
{
    bool res = false;

    switch(ui->solver_combo->currentIndex())
    {
    case 0:
        qDebug() << ui->solver_combo->currentText();
        //res = maze->solveMazeWithBacktracking(1, 1);
        mazeSolver->setSolver(MazeSolver::SolverType::BACKTRACKING);
        break;

    case 1:
        qDebug() << ui->solver_combo->currentText();
        //maze->solveMazeWithAstar(1,1);
        mazeSolver->setSolver(MazeSolver::SolverType::ASTAR);
        //qDebug() << "result: " << res;
        break;

    default:
        break;
    }
    QElapsedTimer currentTime;
    currentTime.start();
    res = mazeSolver->solve();
    if (res)
    {
        qDebug() << "Soulution found in: " << currentTime.nsecsElapsed()/1000.0 << " usec";
        std::vector<std::pair<int, int> > visited = mazeSolver->getPath();
        //qDebug() << visited;
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
        timer->start(10);
    }
}

void MainWindow::updateView()
{
    int height = maze->getHeight();
    int width = maze->getWidth();
    std::vector<std::pair<int, int> > visited = mazeSolver->getPath();
    QImage image(height, width, QImage::Format_Indexed8);
    QRgb blackColor = qRgb(0,0,0);
    QRgb whiteColor = qRgb(255,255,255);
    QRgb blueColor = qRgb(0,0,255);
    QRgb redColor = qRgb(255,0,0);
    QRgb magentaColor = qRgb(255,0,255);
    image.setColor(0, whiteColor);
    image.setColor(1, blackColor);
    image.setColor(2, blueColor);
    image.setColor(3, whiteColor);
    image.setColor(4, redColor);
    image.setColor(5, magentaColor);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image.setPixel(i,j,maze->at(i,j));
        }
    }
    for (int i = 0; i < currentPosition; i++)
    {
        int posX = visited[i].first;
        int posY = visited[i].second;
        image.setPixel(posX,posY,4);
        if (i == currentPosition-1 && i < (int)visited.size()-1)
            image.setPixel(posX,posY,5);
    }
    image = image.scaled(ui->mazeView->height(),ui->mazeView->width(),Qt::KeepAspectRatio);
    ui->mazeView->setPixmap(QPixmap::fromImage(image));
    if (currentPosition < (int)visited.size())
        currentPosition++;
    else
    {
        timer->stop();
    }
}
