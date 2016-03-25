#ifndef mouse_h
#define mouse_h

#include "DataTypes.h"
#include "Hardware.h"

namespace hova {
  class MicroMouse {
    public:
    MicroMouse(int corner = 0);

    void moveTo(const Cardinal &dir, bool mazeDiscovered);
    void updateMovement(bool reset = false);
    
    bool NorthWall() const;
    bool SouthWall() const;
    bool WestWall() const;
    bool EastWall() const;

    Position getPosition() const;
    
    private:
    Position CurrentPosition; //mouse xy coordinates
    char wallsSeen; //last 4 bits are whether the mouse saw those walls
    
    //hardware
    Motors *motors;
    IRSensorArray IR;
    Encoders *encoders;
  };
}

#endif
