#ifndef MAZE_H
#define MAZE_H

#include <vector>

class Maze
{
private:
    int width;
    int height;
    std::vector<std::vector<int> > map;
    void initMaze();
    void generateMaze();
    std::vector<int> generateRandomDirections();
    void recursion(int r, int c);

public:
    Maze(int _w, int _h);
    void printMaze();
    int at(int i, int j);
    void set(int i, int j, int val);
    int getWidth() const;
    void setWidth(int value);
    int getHeight() const;
    void setHeight(int value);
};

#endif // MAZE_H
