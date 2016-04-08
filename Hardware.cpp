#include "Hardware.h"
#include "globals.h"

unsigned int leftEncoderCount;
unsigned int rightEncoderCount;

namespace hova {

static bool started = false;

//left encoder ISR
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
