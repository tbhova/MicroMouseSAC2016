#ifndef maze_h
#define maze_h

#include "DataTypes.h"

namespace hova {
  class Maze {
    public:
    Maze();
    Maze(const Maze &orig);
  
    //reset cells visited and walls
    void resetMaze();
  
    //return true if all maze cells were visited
    bool allCellsVisited();
  
    void removeWall(int row, int column, Cardinal wall);
  
    void placeWall(int row, int column, Cardinal wall);
  
    //get the best direciton from the inner router class
    Cardinal getDirection(const Position &pos);
    
    private:
    
    //indices are rows, columns, last 4 bits of each number is used for the 4 walls
    char mazeWalls[16][16];
  
    //indicies are rows (columns are the 16 bits of each number)
    short int cellsVisited[16];
  
    Cardinal discoverMoreCells(const Position &pos);
    Cardinal bestRoute(const Position &pos);
  };
}

#endif