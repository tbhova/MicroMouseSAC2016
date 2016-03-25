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
    char x; char y; Cardinal dir;
  };
  
  struct Cell {
    char x, y;
  };

 #endif
