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

/*Gyroscope::Gyroscope() {
  
}*/


//left encoder ISR0
void leftEncoderUpdate() {
  //Serial.print("L enc ");
  //Serial.println(leftEncoderCount);
  leftEncoderCount++;
}

//right encoder ISR
void rightEncoderUpdate() {
  //Serial.print("R enc ");
  //Serial.println(rightEncoderCount);
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
    Serial.println("button pressed");
    down = true;
  }
  else if (!button.isPressed() && down) {
    Serial.println("button released");
    down = false;
    started = !started;
  }
}
}
