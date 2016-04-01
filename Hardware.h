#pragma once

//#include <NewPing.h>

/*#define frontTP 1
#define frontEP 4
#define leftTP 3
#define leftEP 5
#define rightEP 2
#define rightTP 11 */

#define frontIRSensor 5
#define leftIRSensor 11
#define rightIRSensor 2
#define rightEncoder 7
#define leftEncoder 3

#warning check pins

namespace hova {

/*class Motors {
  public:
  Motors();
  
  private:
  
};

class IRSensorArray {
  public:
  IRSensorArray();

  private:
};*/

class Gyroscope {
  public:
  Gyroscope();

  private:
};

//left encoder ISR
void leftEncoderUpdate();

//right encoder ISR
void rightEncoderUpdate();

void resetEncoders();

void buttonUpdate();
}

//#endif
