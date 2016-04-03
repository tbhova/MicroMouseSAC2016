#include "Hardware.h"
#include "globals.h"

unsigned int leftEncoderCount;
unsigned int rightEncoderCount;
/*Motors::Motors() {
  
}


IRSensorArray::IRSensorArray() {
  
}*/

namespace hova {

static bool started = false;

Gyroscope::Gyroscope() {
  
}


//left encoder ISR0
void leftEncoderUpdate() {
  leftEncoderCount++;
}

//right encoder ISR
void rightEncoderUpdate() {
  rightEncoderCount++;
}

void resetEncoders() {
  leftEncoderCount = 0;
  rightEncoderCount = 0;
}

void buttonUpdate() {
  //reset mouse position in maze, stop motors
  static bool down = false;
  if (button.isPressed()) {
    down = true;
    if (started) {
      started = false;
    }
  }
  else if (!button.isPressed() && down) {
    down = false;
    if(!started)
      started = true;
  }
}
}
