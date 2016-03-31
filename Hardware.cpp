#include "Hardware.h"
#include "globals.h"

unsigned int LeftEncoderCount;
unsigned int RightEncoderCount;
/*Motors::Motors() {
  
}


IRSensorArray::IRSensorArray() {
  
}*/

namespace hova {

Gyroscope::Gyroscope() {
  
}


//left encoder ISR
void leftEncoderUpdate() {
  //leftEncoderCount++;
}

//right encoder ISR
void rightEncoderUpdate() {
  //rightEncoderCount++;
}

void resetEncoders() {
//  leftEncoderCount = 0;
  //rightEncoderCount = 0;
}

}
