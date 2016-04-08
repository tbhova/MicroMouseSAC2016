#pragma once

#include "DataTypes.h"
#include "Hardware.h"
#include <ZumoMotors.h>


namespace hova {
  class MicroMouse {
    public:
    MicroMouse(unsigned char corner = 0);

    void moveTo(const Cardinal &dir, const bool mazeDiscovered);
    unsigned int getEncoderDistance();
    
    bool isWall(const Cardinal &dir) const;
    void discoverWalls();

    Position getPosition() const;
    ZumoMotors motors;

    void calibratePosition();
    
    private:
    void updateDirection(const Cardinal &desired);
    bool moveForwardOneCell();
    void turn90(const bool right = false);
    
    Position CurrentPosition; //mouse xy coordinates
    char wallsSeen; //last 4 bits are whether the mouse saw those walls
  };
}

