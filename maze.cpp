#include "globals.h"
#include <assert.h>

namespace hova {

  Maze::Maze() {
    this->resetMaze();
    //place outside border walls
    for (int i = 0; i < 16; i++) {
      this->placeWall(0, i, north);
      this->placeWall(15, i, south);
      this->placeWall(i, 0, west);
      this->placeWall(i, 16, east);
    }
  }

  Maze::Maze(const Maze &orig) {
    assert(false);
  }

  void Maze::resetMaze() {
    for(int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        mazeWalls[i][j] = 0;
      }
     cellsVisited[i] = 0; 
    }
  }

  bool Maze::allCellsVisited() {
    static bool allVisited = false;
    if (allVisited)
      return allVisited;
    
    bool temp = true;
    for (int i = 0; i < 16; i++) {
      if (cellsVisited[i] == 0xFF)
        temp = false;
    }

    allVisited = temp;
    return allVisited;
  }

  void Maze::placeWall(int row, int column, Cardinal wall) {
    switch (wall) {
      case (north):
        mazeWalls[row][column] = mazeWalls[row][column] | 8; //1000 binary, set the 4th bit
        break;
      case (south):
        mazeWalls[row][column] = mazeWalls[row][column] | 4; //0100 binary, set the 3rd bit
        break;
      case (west):
        mazeWalls[row][column] = mazeWalls[row][column] | 2; //0010 binary, set the 2nd bit
        break;
      case (east):
        mazeWalls[row][column] = mazeWalls[row][column] | 1; //0001 binary, set the 1st bit
        break;
    }
  }

  void Maze::removeWall(int row, int column, Cardinal wall) {
    switch (wall) {
      case (north):
        mazeWalls[row][column] = mazeWalls[row][column] & 7; //0111 binary, unset the 4th bit
        break;
      case (south):
        mazeWalls[row][column] = mazeWalls[row][column] & 11; //1011 binary, unset the 3rd bit
        break;
      case (west):
        mazeWalls[row][column] = mazeWalls[row][column] & 13; //1101 binary, unset the 2nd bit
        break;
      case (east):
        mazeWalls[row][column] = mazeWalls[row][column] & 14; //1110 binary, unset the 1st bit
        break;
    }
  }

  Cardinal Maze::getDirection(const Position &pos) {
    if (this->allCellsVisited())
        return this->bestRoute(pos);
    return this->discoverMoreCells(pos);
  }

  Cardinal Maze::discoverMoreCells(const Position &pos) {
    
  }

  Cardinal Maze::bestRoute(const Position &pos) {
    
  }
  
}

