#include "globals.h"
#include "maze.h"
#include <QueueList.h>

namespace hova {

  Maze::Maze() {
    this->resetMaze();
    //place outside border walls
    for (byte i = 0; i < 16; i++) {
      this->placeWall(0, i, north);
      this->placeWall(15, i, south);
      this->placeWall(i, 0, west);
      this->placeWall(i, 16, east);
    }
  }

  /*Maze::Maze(const Maze &orig) {
    assert(false);
  }*/

  void Maze::resetMaze() {
    for(byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 16; j++) {
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
    for (byte i = 0; i < 16; i++) {
      if (cellsVisited[i] != 0xFF)
        temp = false;
    }

    allVisited = temp;
    return allVisited;
  }

  void Maze::placeWall(unsigned char row, unsigned char column, Cardinal wall) {
    mazeWalls[row][column] |= (1 << wall);
  }

  void Maze::cellVisited(unsigned char row, unsigned char column) {
    cellsVisited[row] |= (1 << column);
  }

  void Maze::removeWall(unsigned char row, unsigned char column, Cardinal wall) {
    mazeWalls[row][column] &= ((1 << wall) ^ 0x0F); //set the wall-th bit to 0, shift in a 1 then negate (xor) the number
    /*
    switch (wall) {
      case (west):
        mazeWalls[row][column] = mazeWalls[row][column] & 7; //0111 binary, unset the 4th bit
        break;
      case (south):
        mazeWalls[row][column] = mazeWalls[row][column] & 11; //1011 binary, unset the 3rd bit
        break;
      case (east):
        mazeWalls[row][column] = mazeWalls[row][column] & 13; //1101 binary, unset the 2nd bit
        break;
      case (north):
        mazeWalls[row][column] = mazeWalls[row][column] & 14; //1110 binary, unset the 1st bit
        break;
    }*/
  }

  Cardinal Maze::getDirection(const Position &pos) {
    if (this->allCellsVisited())
        return this->bestRoute(pos);
    return this->discoverMoreCells(pos);
  }

  /*int discoverCellsRecursive(Position pos, int depth) {
    //need to avoid visiting the middle cells before the others are discovered
    
    //if discovered return depth
    if (!isCellVisited(pos.x, pos.y)
      return depth;
      
    //if !discovered  
    char bestChoice[4]; // number of cells to move through to find an undiscovered cell, NSWE
    if (isWall(pos.x, pos.y, north) {
      pos.y++;
      bestChoice[0] = discoverCellsRecursive(pos, 0);
      pos.y--;
    } else {
      bestChoice[0] = 127; // infinity for the purpose of a char
    }
    
    if (isWall(pos.x, pos.y, south) {
      pos.y--;
      bestChoice[1] = discoverCellsRecursive(pos, 0);
      pos.y++;
    } else {
      bestChoice[1] = 127; // infinity for the purpose of a char
    }

    if (isWall(pos.x, pos.y, west) {
      pos.x--;
      bestChoice[2] = discoverCellsRecursive(pos, 0);
      pos.x++;
    } else {
      bestChoice[2] = 127; // infinity for the purpose of a char
    }

    if (isWall(pos.x, pos.y, east) {
      pos.x++;
      bestChoice[3] = discoverCellsRecursive(pos, 0);
      pos.x--;
    } else {
      bestChoice[3] = 127; // infinity for the purpose of a char
    } 

    char minimum = 0;
    for (int i = 1; i <4; i ++) {
      if (bestChoice[i] < bestChoice[minimum])
        minimum = i;
    }
    return bestChoice[minimum];
  }*/
  
  Cardinal Maze::discoverMoreCells(Position pos) {
    //set up bfs
    short unsigned int bfsDisc[16];
    //unsigned char distanceTo[16][16];
    for (byte i = 0; i < 16; i++) {
      bfsDisc[i] = 0;
    }
    
    //select a cell to go to

    //prefer to go straight
    /*swtich (pos.dir) {
      case (north) :
        if(!isWall(pos.x, pos.y+1) && !isCellVisited(pos.x, pos.y+1))
          return north;
        break;
      case (south) : 
       if(!isWall(pos.x, pos.y-1) && !isCellVisited(pos.x, pos.y-1))
          return north;
        break;
      case (west) : 
       if(!isWall(pos.x-1, pos.y) && !isCellVisited(pos.x-1, pos.y))
          return north;
        break;
      case (east) : 
       if(!isWall(pos.x+1, pos.y) && !isCellVisited(pos.x+1, pos.y))
          return north;
        break;
    }*/

    //select nearest undiscovered cell
    Position dest = findNearestUndiscoveredCell(pos);
    Serial.print("dest ");
    Serial.print(dest.x);
    Serial.print(" ");
    Serial.println(dest.y);
    return directionToCell(dest, pos, bfsDisc);
  }

  Position Maze::findNearestUndiscoveredCell(Position current) const {
    //determine whether there are cells that aren't the middle that are undiscovered
    bool otherCellsAvailable = false;
    for (byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 16; j++) {
        if (i != 7 && i != 8 && j != 7 && j != 8 && !isCellVisited(i, j)) {
          otherCellsAvailable = true;
          break;
        }
      }
    }
    ///return early if only center cells exist;
    if (!otherCellsAvailable) {
      #warning ensure to check for walls in center, that will break this code
      static unsigned char centersVisited = 0;
      switch (centersVisited) {
      case(0):
        current.x = 7; current.y = 7;
        centersVisited++;
        break;
      case(1):
        current.x = 7; current.y = 8;
        centersVisited++;
        break;
      case(2):
        current.x = 8; current.y = 8;
        centersVisited++;
        break;
      case(3):
        current.x = 8; current.y = 7;
        centersVisited++;
        break;
      }
      return current;
    }
    //look for cell
    for (byte i = 1; i < 15; i++) {
      if(current.x + i != 7 && current.x + i != 8 && (current.x + i < 16) && !isCellVisited(current.x + i, current.y)) {
        current.x += i;
        return current;
      } /*else {
        Serial.print(current.x);
        Serial.print(' ');
        Serial.print(i);
        Serial.print(' ');
        if(isCellVisited(current.x+i, current.y))
          Serial.println('T');
        else
          Serial.println('F');
      }*/
      if(current.y + i != 7 && current.y + i != 8 && (current.y + i < 16) && !isCellVisited(current.x, current.y+i)) {
        current.y += i;
        return current;
      }
      if(current.x - i != 7 && current.x - i != 8 && (current.x - i >= 0) && !isCellVisited(current.x - i, current.y)) {
        current.x -= i;
        return current;
      }
      if(current.x - i != 7 && current.y - i != 8 && (current.y - i >= 0) && !isCellVisited(current.x, current.y-i)) {
        current.y -= i;
        return current;
      }
    }
    return current;
  }

  Cardinal Maze::directionToCell(const Position &des, const Position &current, short unsigned int discovered[]) {
    Serial.print("dir to ");
    Serial.print(des.x);
    Serial.print(' ');
    Serial.print(des.y);
    Serial.print(' ');
    Serial.print(current.x);
    Serial.print(' ');
    Serial.println(current.y);
    unsigned char whereFrom[16][16];
    for (byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 16; j++) {
        whereFrom[i][j] = 0;
      }
    }
    
    //our bfs has now visited the current cell
    discovered[current.x] |= (1 << (current.y));
    
    QueueList<Cell> queue;

    Cell cur;
    cur.x = current.x;
    cur.y = current.y;
    queue.push(cur);

    while(!queue.isEmpty()) {
      cur = queue.peek();
      if(queue.isEmpty())
        Serial.println("empty");
        
      Serial.print(cur.x);
      Serial.print(' ');
      Serial.println(cur.y);
      //set cellVisited bool bit after popping to avoid infinite looping
      discovered[cur.x] |= (1 << (cur.y));

      //go through adjacency list
      for (byte i = north; i <= east; i++) {
        cur = queue.peek(); //update front element since we modify it
        //check if cells are adjacent (no wall)
        if (!this->isWall(cur.x, cur.y, (Cardinal) i)) {
          //if cell is adjacent check if cell has already been vistied
          if ((discovered[cur.x] & (1 << (cur.y))) == 0) {
            //if has not been visited, queue cell
            Cardinal opposite;
            switch ((Cardinal)i) {
              case (north) :
                cur.y++;
                opposite = south;
                break;
              case (south) :
                cur.y--;
                opposite = north;
                break;
              case (west) : 
                cur.x--;
                opposite = east;
                break;
              case (east) :
                cur.x++;
                opposite = west;
                break;
            }
            whereFrom[cur.x][cur.y] = opposite;
            if(queue.isEmpty())
              Serial.println("empty2");
            queue.push(cur);
            //if we are at destination, stop
            if(queue.isEmpty())
              Serial.println("empty3");
            if (cur.x == des.x && cur.y == des.y) {
              Serial.println("BFS found");
              //we are done, found our cell
              break;
            }
          }
        }
      }
      //we are done with front element now
      queue.pop();
    }
    
    //follow the whereFrom and return the first direction
    cur.x = des.x; cur.y = des.y;
    while (true) {
      Serial.print("trace ");
      Serial.print(cur.x);
      Serial.print(' ');
      Serial.println(cur.y);
      if (cur.x >= 16 || cur.x < 0 || cur.y < 0 || cur.y >=16)
        break;
      if (whereFrom[cur.x][cur.y] == 0)
        break;
      Cardinal lastMoveOpposite;
      switch ((Cardinal)whereFrom[cur.x][cur.y]) {
        case (north) :
          cur.y++;
          lastMoveOpposite = south;
          break;
        case (south) :
          cur.y--;
          lastMoveOpposite = north;
          break;
        case (west) :
          cur.x--;
          lastMoveOpposite = east;
          break;
        case (east) :
          cur.x++;
          lastMoveOpposite = west;
          break;
        }
        
      if (cur.x == current.x && cur.y == current.y) {
        return lastMoveOpposite;
      }
    }
    Serial.println("Error BFS failed");
    return north;
  }

  Cardinal Maze::bestRoute(const Position &pos) {
    #warning use bfs from above, break up into more methods, making the method more generic, so bfs can be recycled
    return north;
  }

  bool Maze::isCellVisited(const unsigned char row, const unsigned char column) const {
    return ((cellsVisited[row] & (1 << (column+1))) > 0);
  }
  
  bool Maze::isWall(const unsigned char row, const unsigned char column, const Cardinal dir) const {
    return ((mazeWalls[row][column] & (1 << dir)) > 0);
  }
}
