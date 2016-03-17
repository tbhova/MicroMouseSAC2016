#ifndef DataTypes_h
#define DataTypes_h

//cardinal directions
  enum Cardinal {
    north,
    south,
    east,
    west
  };

  //store xy coordinate and pointing direction
  struct Position {
    int x; int y; Cardinal dir;
  };

 #endif
