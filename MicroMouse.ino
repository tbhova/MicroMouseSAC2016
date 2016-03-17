#include "globals.h"
#include "maze.h"
#include "MicroMouse.h"
#include "DataTypes.h"

using namespace std;
using namespace hova;

MicroMouse *mouse;
Maze *maze;
void setup() {
  // put your setup code here, to run once:
  mouse = new MicroMouse();
  maze = new Maze();
}

void loop() {
  // put your main code here, to run repeatedly:

  //get mouse position
  Position currentPosition = mouse->getPosition();
  
  //get new direction from maze and have mouse move there (maze handles optimal route)
  mouse->moveTo(maze->getDirection(currentPosition));
  
  if (maze->allCellsVisited()) {
    //walls do not need updated
  }
  else {
    //update walls
    if (mouse->NorthWall()) {
      maze->placeWall(currentPosition.x, currentPosition.y, north);
    }
    if (mouse->SouthWall()) {
      maze->placeWall(currentPosition.x, currentPosition.y, south);
    }
    if (mouse->WestWall()) {
      maze->placeWall(currentPosition.x, currentPosition.y, west);
    }
    if (mouse->EastWall()) {
      maze->placeWall(currentPosition.x, currentPosition.y, east);
    }
  }
}
