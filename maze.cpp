#include <random>
#include <QDebug>
#include <cstdio>
#include <iostream>

#include "maze.h"

int Maze::getWidth() const
{
    return width;
}

void Maze::setWidth(int value)
{
    width = value;
}

int Maze::getHeight() const
{
    return height;
}

void Maze::setHeight(int value)
{
    height = value;
}

void Maze::initMaze()
{
    map = std::vector<std::vector<int> > (height, std::vector<int>(width));
    for (int i = 0; i < height; i++)
        map[i] = std::vector<int>(width);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            map[i][j] = 1;
}

void Maze::generateMaze()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disHeight(0, height-1);
    std::uniform_int_distribution<> disWidth(0, width-1);

    int r = disHeight(gen);
    while (r % 2 == 0) {
        r = disHeight(gen);
    }
    int c = disWidth(gen);
    while (c % 2 == 0) {
        c = disWidth(gen);
    }
    map[r][c] = 0;

    recursion(r, c);

    // Exit position
    map[height-2][width-2] = 2;
}

void Maze::recursion(int r, int c)
{
    std::vector<int> randDirs = generateRandomDirections();
    for (int i = 0; i < 4; i++) {

        switch(randDirs[i]){
        case 1: // Up
            if (r - 2 <= 0)
                continue;
            if (map[r - 2][c] != 0) {
                map[r-2][c] = 0;
                map[r-1][c] = 0;
                recursion(r - 2, c);
            }
            break;
        case 2: // Right
            if (c + 2 >= width - 1)
                continue;
            if (map[r][c + 2] != 0) {
                map[r][c + 2] = 0;
                map[r][c + 1] = 0;
                recursion(r, c + 2);
            }
            break;
        case 3: // Down
            if (r + 2 >= height - 1)
                continue;
            if (map[r + 2][c] != 0) {
                map[r+2][c] = 0;
                map[r+1][c] = 0;
                recursion(r + 2, c);
            }
            break;
        case 4: // Left
            if (c - 2 <= 0)
                continue;
            if (map[r][c - 2] != 0) {
                map[r][c - 2] = 0;
                map[r][c - 1] = 0;
                recursion(r, c - 2);
            }
            break;
        }
    }
}

std::vector<int> Maze::generateRandomDirections()
{
    std::vector<int> res;
    for (int i = 0; i < 4; i++)
        res.push_back(i+1);
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(res.begin(), res.end(), g);
    return res;
}


Maze::Maze(int _w, int _h)
{
    width = _w;
    height = _h;
    initMaze();
    generateMaze();
    //printMaze();
}

void Maze::printMaze()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            std::cout << map[i][j]<< " ";
        std::cout << std::endl;
    }
}

int Maze::at(int i, int j)
{
    return map[i][j];
}

void Maze::set(int i, int j, int val)
{
    map[i][j] = val;
}
