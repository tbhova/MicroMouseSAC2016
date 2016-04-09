#pragma once
#include "DataTypes.h"
#include <QueueList.h>

namespace hova {
  class Maze {
    public:
    Maze();
    Maze(const Maze &orig);
  
    //reset cells visited and walls
    void resetMaze();
  
    //return true if all maze cells were visited
    bool allCellsVisited();
  
    void removeWall(unsigned char row, unsigned char column, Cardinal wall);
  
    void placeWall(unsigned char row, unsigned char column, Cardinal wall);

    void cellVisited(unsigned char row, unsigned char column);
  
    //get the best direciton from the inner router class
    Cardinal getDirection(const Position &pos);
    
    private:
    
    //indices are rows, columns, last 4 bits of each number is used for the 4 walls
    //bits in order from MSB first north, south, west, east
    unsigned char mazeWalls[16][16];
    unsigned char floodValues[16][16];
  
    //indicies are rows (columns are the 16 bits of each number)
    short unsigned int cellsVisited[16];
  
    Cardinal discoverMoreCells(const Position pos);
    //Cardinal bestRoute(const Position &pos);
    bool isCellVisited(const unsigned char row, const unsigned char column) const;
    bool isWall(const unsigned char row, const unsigned char column, const Cardinal dir) const;
    //Position findNearestUndiscoveredCell(Position pos) const;
    //Cardinal directionToCell(const Position &dest, const Position &current, short unsigned int discovered[]);
    Cardinal floodFill(const Position &current);
    void floodVisit(byte x, byte y);
    byte floodMinimum();
    Cardinal FloodDirection(const Position &current, byte minimum);
    void addFloodQueue(const Cell &cur, QueueList<Cell> &queue, short unsigned int visted[]);
    byte floodMinimum(byte x, byte y);
    byte FloodValueAt(byte x, byte y, Cardinal dir);
    void printFloodMaze();
  };
}

//#endif
