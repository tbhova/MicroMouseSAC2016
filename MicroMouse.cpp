#include "MicroMouse.h"
#include <assert.h>

using namespace hova;

MicroMouse::MicroMouse(int corner) {
  switch (corner) {
  case 0: //initial corner (south west)
    CurrentPosition.x = 0;
    CurrentPosition.y = 0;
    CurrentPosition.dir = north;
    break;
  case 1: //northwest corner
    CurrentPosition.x = 0;
    CurrentPosition.y = 15;
    CurrentPosition.dir = south;
    break;
  case 2: //northeast corner
    CurrentPosition.x = 15;
    CurrentPosition.y = 15;
    CurrentPosition.dir = south;
    break;
  case 3: //southeast corner
    CurrentPosition.x = 15;
    CurrentPosition.y = 0;
    CurrentPosition.dir = north;
    break;
  default:
    assert(false);
  }
}

void MicroMouse::updateMovement(bool reset) {
  static int distance = 0;
  if (reset) {
    distance = 0;
  }
  //update motors with encoder feedback
}

void MicroMouse::moveTo(const Cardinal &dir, bool mazeDiscovered) {
  wallsSeen = 0; //reset all bits
  #warning //hardware
  //to do hardware movement
  bool inNewCell = false;
  while (!inNewCell) {
    this->updateMovement();
    if (!mazeDiscovered) {  
      //to do hardware wall updates  
    }
  }

  
  
  
  //update coordinates
  switch (dir) {
    case north:
      CurrentPosition.y++;
      break;
    case south:
      CurrentPosition.y--;
      break;
    case west:
      CurrentPosition.x--;
      break;
    case east:
      CurrentPosition.x++;
      break;
  }
  //update direction
  CurrentPosition.dir = dir;
}

bool MicroMouse::NorthWall() const{
  return ((wallsSeen & 0x8) > 0);
}

bool MicroMouse::SouthWall() const{
  return ((wallsSeen & 0x4) > 0);
}

bool MicroMouse::WestWall() const{
  return ((wallsSeen & 0x2) > 0);
}

bool MicroMouse::EastWall() const{
  return ((wallsSeen & 0x1) > 0);
}

Position MicroMouse::getPosition() const{
  return CurrentPosition;
}

