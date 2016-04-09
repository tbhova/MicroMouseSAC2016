#include "Hardware.h"
#include "globals.h"

unsigned int leftEncoderCount;
unsigned int rightEncoderCount;

namespace hova {

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
}
