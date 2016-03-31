#pragma once

//#ifndef mouse_h
//#define mouse_h

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

    Position getPosition() const;
    
    private:
    void updateDirection(const Cardinal &desired);
    void moveForwardOneCell();
    void turn90(const bool right = false);
    void discoverWalls();
    
    Position CurrentPosition; //mouse xy coordinates
    char wallsSeen; //last 4 bits are whether the mouse saw those walls
    
    //hardware
    //Motors *motors;
    ZumoMotors motors;
    
//    Encoders encoders;
  };
}

//#endif
