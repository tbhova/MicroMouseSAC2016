#ifndef Hardware_h
#define hardware_h
#include <NewPing.h>

#define frontTP 1
#define frontEP 4
#define leftTP 3
#define leftEP 5
#define rightEP 2
#define rightTP 11

namespace hova {

class Motors {
  public:
  Motors();
  
  private:
  
};

class IRSensorArray {
  public:
  IRSensorArray();

  private:
};

class Encoders {
  public:
  Encoders();

  private:
};

class Gyroscope {
  public:
  Gyroscope();

  private:
};

}

#endif
