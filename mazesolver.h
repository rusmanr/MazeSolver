#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "maze.h"
#include <vector>


typedef std::pair< int, int > Point;
typedef std::tuple< int, int, Point > Node;

class MazeSolver
{
public:
    enum SolverType
        {
            BACKTRACKING,
            ASTAR
        };
private:
    Maze* maze;
    SolverType solver;
    bool isPositionSafe(int x, int y);
    std::vector<std::pair<int, int> > visited;
    int mDistance(const Point &a, const Point &b);

public:
    MazeSolver(Maze* _maze, SolverType _solver);
    bool solve();
    std::vector<std::pair<int, int> > getPath() const;
    SolverType getSolver() const;
    void setSolver(const SolverType &value);

    bool solveMazeWithBacktracking(int x, int y);
    bool solveMazeWithAstar(int x, int y);
};

#endif // MAZESOLVER_H
