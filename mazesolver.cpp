#include <random>
#include <algorithm>
#include <iostream>
#include <QDebug>
#include <cstdio>
#include <set>
#include <queue>

#include "mazesolver.h"
#include "maze.h"


const double FLT_MAX = 3.402823e+38;
const int obstacle = 1;
const int point_visited = 3;

typedef std::pair<int, int> Pair;
typedef std::pair<double, std::pair<int, int>> pPair;

void MazeSolver::setSolver(const SolverType &value)
{
    solver = value;
}

MazeSolver::MazeSolver(Maze* _maze, SolverType _solver) : maze(_maze), solver(_solver)
{
}

bool MazeSolver::solve()
{
    switch (solver) {
    case SolverType::BACKTRACKING:
        return solveMazeWithBacktracking(1,1);
        break;
    case SolverType::ASTAR:
        return solveMazeWithAstar(1,1);
        break;
    default:
        break;
    }
    return false;
}

std::vector<std::pair<int, int> > MazeSolver::getPath() const
{
    return visited;
}

bool MazeSolver::isPositionSafe(int x, int y)
{
    return (maze->at(x,y) == 0 || maze->at(x,y) == 2);
}

bool MazeSolver::solveMazeWithBacktracking(int x, int y)
{
    if (maze->at(x,y) == 2)
    {
        qDebug() << "Solution found!";
        return true;
    }
    else if (maze->at(x,y) == 1)
        return false;
    else if (maze->at(x,y) == 3)
        return false;

    maze->set(x,y,3);
    visited.push_back(std::make_pair(x,y));
    if ((x < maze->getHeight() && solveMazeWithBacktracking(x+1, y)) ||
            (y > 1 && solveMazeWithBacktracking(x, y-1)) ||
            (x > 1 && solveMazeWithBacktracking(x-1, y)) ||
            (y < maze->getWidth() && solveMazeWithBacktracking(x, y+1)))
        return true;

    return false;
}

int MazeSolver::mDistance( const Point& a, const Point& b ){
    return (std::abs( a.first - b.first ) + std::abs( a.second - b.second ));
}

bool MazeSolver::solveMazeWithAstar(int x, int y)
{
    std::priority_queue< Node, std::vector<Node>, std::greater<Node> > Q;
    std::vector< Node > debugArr;

    Point start(x,y);
    Point end(maze->getHeight()-2, maze->getWidth()-2);
    maze->set(maze->getHeight()-2, maze->getWidth()-2, 2);

    std::map< Point, Point > Graph;
    std::vector<std::vector<bool> > visitedNodes(maze->getHeight(), std::vector<bool>(maze->getWidth()));
    std::vector<std::vector<Point> > cameFrom(maze->getHeight(), std::vector<Point>(maze->getWidth()));

    for (int i = 0; i < maze->getHeight(); i++)
    {
        for (int j = 0; j < maze->getWidth(); j++)
        {
            visitedNodes[i][j] = 0;
            cameFrom[i][j] = Point(1,1);
        }
    }

    Node clostestPoint = Node( mDistance( start, end), 0, start );

    auto reconstructPath = [&]( Point endPt )
    {
        std::vector<Point> path;

        int cx, cy;
        Point tmpPt;

        cx = endPt.first;
        cy = endPt.second;

        while( start.first != cx || start.second != cy ){
            path.push_back( Point( cx, cy ) );
            tmpPt = cameFrom[cx][cy];
            cx = tmpPt.first;
            cy = tmpPt.second;
        }

        return path;
    };

    auto push = [&]( const Point& from, int x, int y, int step )
    {

        if( x >= maze->getHeight() || x < 0 || y >= maze->getWidth() || y < 0 ) return;
        if( maze->at(x,y) == obstacle ) return;

        //        if( visited[x][y] && step > bitmap[x][y] ) return;
        if( visitedNodes[x][y] ) return;
        visitedNodes[x][y] = true;


        const Point pt( x, y );

        int distance = mDistance( pt, end );

        if( std::get<0>(clostestPoint) > distance ||
                (std::get<0>(clostestPoint) == distance && std::get<1>(clostestPoint) > step )
                ) {
            clostestPoint = Node( distance, step, pt );
        }

        // add to open list
        Q.push( Node( step + distance * 3/2, step, pt ) );
        cameFrom[x][y] = from;
    };

    push( start, start.first, start.second, 0 );
    // if( maze->at( end.first , end.second ) == obstacle ) return false;

    while( !Q.empty() )
    {
        const Node current = Q.top();

        const Point cpoint = std::get<2>( current );
        //const int cscore = std::get<0>( current );
        const int cstep = std::get<1>( current );


        if( cpoint == end ){
            visited = reconstructPath( end );
            visited.push_back(Point(1,1));
            std::reverse(visited.begin(), visited.end());
            visited.pop_back();
            return true;
        }

        Q.pop();

        push( cpoint, cpoint.first + 1, cpoint.second, cstep + 1 );
        push( cpoint, cpoint.first - 1, cpoint.second, cstep + 1 );
        push( cpoint, cpoint.first, cpoint.second + 1, cstep + 1 );
        push( cpoint, cpoint.first, cpoint.second - 1, cstep + 1 );

    }
    visited = reconstructPath(std::get<2>(clostestPoint ));
    visited.push_back(Point(1,1));
    std::reverse(visited.begin(), visited.end());
    visited.pop_back();
    return false;
}
