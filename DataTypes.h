#ifndef DataTypes_h
#define DataTypes_h

#warning use values assigned to directions to reduce switches
//cardinal directions
enum Cardinal {
  north = 1,
  east = 2,
  south = 3,
  west = 4
};

//store xy coordinate and pointing direction
struct Position {
  unsigned char x; unsigned char y; Cardinal dir;
};

struct Cell {
  unsigned char x, y;
};

#endif
