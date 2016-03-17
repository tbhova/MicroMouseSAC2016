#ifndef mouse_h
#define mouse_h

#include "DataTypes.h"

namespace hova {
  class MicroMouse {
    public:
    MicroMouse();

    void moveTo(const Cardinal &dir);
    
    bool NorthWall() const;
    bool SouthWall() const;
    bool WestWall() const;
    bool EastWall() const;

    Position getPosition() const;
    
    private:
    Position CurrentPosition; //mouse xy coordinates
    
  };
}

#endif
