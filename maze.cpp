#include "globals.h"
#include "maze.h"
#include <QueueList.h>

namespace hova {

  Maze::Maze() {
    this->resetMaze();
    //place outside border walls
    for (byte i = 0; i < 16; i++) {
      this->placeWall(0, i, west);
      this->placeWall(15, i, east);
      this->placeWall(i, 0, south);
      this->placeWall(i, 15, north);
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
    /*#warning add count cells visited and if number is small enough, set cells with 4 walls to visited (watch for big enclosed areas)
    
    static bool allVisited = false;
    if (allVisited)
      return allVisited;
    
    bool temp = true;
    for (byte i = 0; i < 16; i++) {
      if (cellsVisited[i] != 0xFF)
        temp = false;
    }

    allVisited = temp;
    return allVisited;*/
    return false;
  }

  void Maze::placeWall(unsigned char row, unsigned char column, Cardinal wall) {
    mazeWalls[row][column] |= (1 << wall);
  }

  void Maze::cellVisited(unsigned char row, unsigned char column) {
    cellsVisited[row] |= (1 << column);
  }

  void Maze::removeWall(unsigned char row, unsigned char column, Cardinal wall) {
    mazeWalls[row][column] &= ((1 << wall) ^ 0x0F); //set the wall-th bit to 0, shift in a 1 then negate (xor) the number
  }

  Cardinal Maze::getDirection(const Position &pos) {
    /*if (this->allCellsVisited())
        return this->bestRoute(pos);*/
    //return this->discoverMoreCells(pos);
    return this->floodFill(pos);
  }
  
/*  Cardinal Maze::discoverMoreCells(Position pos) {
    //set up bfs
    short unsigned int bfsDisc[16];
    //unsigned char distanceTo[16][16];
    for (byte i = 0; i < 16; i++) {
      bfsDisc[i] = 0;
    }

    //select nearest undiscovered cell
    Position dest = findNearestUndiscoveredCell(pos);
    /*Serial.print("dest ");
    Serial.print(dest.x);
    Serial.print(" ");
    Serial.println(dest.y);*/
  /*  return directionToCell(dest, pos, bfsDisc);
  }*/

  /*Position Maze::findNearestUndiscoveredCell(Position current) const {
    //determine whether there are cells that aren't the middle that are undiscovered
    /*bool otherCellsAvailable = false;
    for (byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 16; j++) {
        if (!((i == 7 || i == 8) && (j == 7 || j == 8)) && !isCellVisited(i, j)) {
          otherCellsAvailable = true;
          break;
        }
      }
    }
    ///return early if only center cells exist;
    if (!otherCellsAvailable) {*/
    /*Position dest;
    if(!((current.x == 7 || current.x == 8) && (current.y == 7 || current.y==8))) {
      current.x = 7; current.y = 7;
      return current;
    } else {
      #warning ensure to check for walls in center, that will break this code
      static unsigned char centersVisited = 1;
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
    /*for (byte i = 1; i < 15; i++) {
      if(!((current.x + i == 7 || current.x + i == 8) && (current.y == 7 || current.y == 8)) && (current.x + i < 16) && !isCellVisited(current.x + i, current.y)) {
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
      /*if(!((current.x == 7 || current.x == 8) && (current.y + i == 7 || current.y + i == 8)) && (current.y + i < 16) && !isCellVisited(current.x, current.y+i)) {
        current.y += i;
        return current;
      }
      if(!((current.x - i == 7 || current.x - i == 8) && (current.y == 7 || current.y == 8)) && (current.x - i >= 0) && !isCellVisited(current.x - i, current.y)) {
        current.x -= i;
        return current;
      }
      if(!((current.x == 7 || current.x == 8) && (current.y - i == 7 || current.y - i == 8)) && (current.y - i >= 0) && !isCellVisited(current.x, current.y-i)) {
        current.y -= i;
        return current;
      }
    }
    return current;*/
/*  }

  Cardinal Maze::directionToCell(const Position &des, const Position &current, short unsigned int discovered[]) {
    Serial.print("dir to ");
    Serial.print(des.x);
    Serial.print(' ');
    Serial.print(des.y);
    Serial.print(" from ");
    Serial.print(current.x);
    Serial.print(' ');
    Serial.println(current.y);
    unsigned char whereFrom[16][16];
    for (byte i = 0; i < 16; i++) {
      for (byte j = 0; j < 16; j++) {
        whereFrom[i][j] = 0;
      }
    }
    
    QueueList<Cell> queue;

    Cell cur;
    cur.x = current.x;
    cur.y = current.y;
    queue.push(cur);

    while(!queue.isEmpty()) {
      cur = queue.peek();
        
      /*Serial.print(cur.x);
      Serial.print(' ');
      Serial.println(cur.y);*/
      
  /*    //go through adjacency list
      for (byte i = north; i <= west; i++) {
        /*Serial.print("Loop ");
        Serial.println(i);*/
     /*   cur = queue.peek(); //update front element since we modify it
        //check if cells are adjacent (no wall)
        if (!this->isWall(cur.x, cur.y, (Cardinal) i)) {
          /*Serial.print("No wall ");
          Serial.println(i);*/
       /*   Cardinal opposite;
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
          if ((discovered[cur.x] & (1 << cur.y)) == 0) {
            whereFrom[cur.x][cur.y] = (unsigned char)opposite;
           /* Serial.print("push cell ");
            Serial.print(cur.x);
            Serial.print(' ');
            Serial.println(cur.y);*/
         /*   queue.push(cur);
            //if we are at destination, stop
            if (cur.x == des.x && cur.y == des.y) {
              //Serial.println("BFS found");
              //we are done, found our cell
              //empty the queue and break out of for loop
              cur = queue.pop();
              while(!queue.isEmpty())
                queue.pop();
              break;
            }
          }
        }
      }
      //we are done with front element now
      if (!queue.isEmpty())
        cur = queue.pop();
      //set cell to visited
      discovered[cur.x] |= (1 << (cur.y));
    }
    
    //follow the whereFrom and return the first direction
    cur.x = des.x; cur.y = des.y;
    while (true) {
      /*Serial.print("trace ");
      Serial.print(cur.x);
      Serial.print(' ');
      Serial.println(cur.y);*/
      /*if (cur.x >= 16 || cur.x < 0 || cur.y < 0 || cur.y >=16)
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
        Serial.print("BFS direction is ");
        Serial.println(lastMoveOpposite);
        return lastMoveOpposite;
      }
    }
    Serial.println("Error BFS failed");
    return north;
  }*/

  /*Cardinal Maze::bestRoute(const Position &pos) {
    #warning use bfs from above, break up into more methods, making the method more generic, so bfs can be recycled
    return north;
  }*/

  bool Maze::isCellVisited(const unsigned char row, const unsigned char column) const {
    return ((cellsVisited[row] & (1 << (column))) > 0);
  }
  
  bool Maze::isWall(const unsigned char row, const unsigned char column, const Cardinal dir) const {
    return ((mazeWalls[row][column] & (1 << dir)) > 0);
  }
  void Maze:: floodVisit(byte x, byte y) {
    byte minimum = 255;
    if (y+1 < 16 && floodValues[x][y+1] < minimum)
      minimum = floodValues[x][y+1];
    if (x+1 < 16 && floodValues[x+1][y] < minimum)
      minimum = floodValues[x+1][y];
    if (y-1 >= 0 && floodValues[x][y-1] < minimum)
      minimum = floodValues[x][y-1];
    if (x-1 >= 0 && floodValues[x-1][y] < minimum)
      minimum = floodValues[x-1][y];
    minimum = (minimum == 255) ? 0 : minimum;
    floodValues[x][y] = minimum+1;
  }

  void Maze::addFloodQueue(const Cell &cur, QueueList<Cell> &queue, short unsigned int visited[]) {
    Cell Push;
    /*Serial.print("add flood queue ");
    Serial.print(cur.x);
    Serial.print(' ');
    Serial.println(cur.y);*/
    //Serial.println(visited[cur.x] & (1 << (cur.y-1)));
    if (cur.y - 1 >= 0 && !isWall(cur.x, cur.y, south) && ((visited[cur.x] & (1 << (cur.y-1))) == 0)) {
      Push.x = cur.x; Push.y = cur.y-1;
      visited[Push.x] |= (1 << Push.y);
      queue.push(Push);
    }
    //Serial.println(visited[cur.x+1] & (1 << (cur.y)));
    if (cur.x + 1 < 16 && !isWall(cur.x, cur.y, east) && (visited[cur.x+1] & (1 << cur.y) == 0)) {
      Push.x = cur.x+1; Push.y = cur.y;
      visited[Push.x] |= (1 << Push.y);
      queue.push(Push);
    }
    //Serial.println(visited[cur.x] & (1 << (cur.y+1)));
    if (cur.y + 1 < 16 && !isWall(cur.x, cur.y, north) && (visited[cur.x] & (1 << (cur.y+1)) == 0)) {
      Push.x = cur.x; Push.y = cur.y+1;
      visited[Push.x] |= (1 << Push.y);
      queue.push(Push);
    }
    //Serial.println(visited[cur.x-1] & (1 << (cur.y)));
    if (cur.x - 1 >= 0 && !isWall(cur.x, cur.y, west) && (visited[cur.x-1] & (1 << cur.y) == 0)) {
      Push.x = cur.x-1; Push.y = cur.y;
      visited[Push.x] |= (1 << Push.y);
      queue.push(Push);
    }
  }

  byte Maze::floodMinimum(byte x, byte y) {
    byte ret = 255;
    if (x - 1 >= 0 && !isWall(x, y, west)) {
      ret = (ret < floodValues[x-1][y]) ? ret : floodValues[x-1][y];
    }
    if (x+1 < 16 && !isWall(x, y, north)) {
      ret = (ret < floodValues[x][y+1]) ? ret : floodValues[x][y+1];
    }
    if (x+1 < 16 && !isWall(x, y, east)) {
      ret = (ret < floodValues[x+1][y]) ? ret : floodValues[x+1][y];
    }
    if (y-1 >= 0 && !isWall(x, y, south)) {
      ret = (ret < floodValues[x][y-1]) ? ret : floodValues[x][y-1];
    }
    return ret;
  }
  
  Cardinal Maze::floodFill(const Position &current) {
    Serial.println("Flood Fill");
    short unsigned int visited[16];
    //reset
    for (int i = 0; i < 16; i++) {
      visited[i] = 0;
      for (int j = 0; j < 16; j++)
        floodValues[i][j]=255;
    }
    visited[7] |= (1 << 7);
    visited[7] |= (1 << 8);
    visited[8] |= (1 << 7);
    visited[8] |= (1 << 8);
    floodVisit(7,7);
    floodVisit(8,7);
    floodVisit(7,8);
    floodVisit(8,8);
    
    Cell cur;
    QueueList<Cell> queue;
    cur.x = 7; cur.y = 7;
    addFloodQueue(cur, queue, visited);
    cur.x = 7; cur.y = 8;
    addFloodQueue(cur, queue, visited);
    cur.x = 8; cur.y = 7;
    addFloodQueue(cur, queue, visited);
    cur.x = 8; cur.y = 8;
    addFloodQueue(cur, queue, visited);
    Serial.println(queue.count());
    while(!queue.isEmpty()) {
      Serial.println(queue.count());
      cur = queue.pop();
      floodVisit(cur.x, cur.y);
      addFloodQueue(cur, queue, visited);
    }

    byte minimum = floodMinimum(current.x, current.y);
    Serial.print("min ");
    Serial.println(minimum);

    this->printFloodMaze();
    return FloodDirection(current, minimum);
  }

  byte Maze::FloodValueAt(byte x, byte y, Cardinal dir) {
    switch(dir) {
      case (north) :
        return floodValues[x][y+1];
      case (south) :
        return floodValues[x][y-1];
      case (west) :
        return floodValues[x-1][y];
      case (east) :
        return floodValues[x+1][y];  
    }
  }
  Cardinal Maze::FloodDirection(const Position &current, byte minimum) {
    //prefer straight
    if(!isWall(current.x, current.y, current.dir) && minimum == FloodValueAt(current.x, current.y, current.dir)) {
      Serial.println("Front");
      return current.dir;
    }
    byte right = current.dir + 1;
    right = (right > 4) ? right - 4 : right;
    byte left = current.dir - 1;
    left = (left < 1) ? 4 - left : left;
    byte rear = current.dir + 2;
    rear = (rear > 4) ? rear - 4: rear;
    if (!isWall(current.x, current.y, (Cardinal)right) && minimum == FloodValueAt(current.x, current.y, (Cardinal)right)) {
      Serial.println("right");
      return (Cardinal)right;
    }
    else if (!isWall(current.x, current.y, (Cardinal)left) && minimum == FloodValueAt(current.x, current.y, (Cardinal)left)) {
      Serial.println("left");
      return (Cardinal)left;
    } else if (!isWall(current.x, current.y, (Cardinal)rear) && minimum == FloodValueAt(current.x, current.y, (Cardinal)rear)) {
      Serial.println("rear");
      return (Cardinal)rear;
    }
  }

void Maze::printFloodMaze() {
  //print floodValues
  Serial.println();
  Serial.println("Flood Fill matrix");

  //add code here
  for(int i=0; i<16; i++){
    for(int j=0; j<16; j++){
      Serial.print(floodValues[i][j]);
      Serial.println();
    }
  }
  //Serial.print(floodValues[0][0]);
  //Serial.println(floodValues[0][1]);
}

}
