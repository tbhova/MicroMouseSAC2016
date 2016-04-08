#pragma once

#define frontIRSensor A0
#define leftIRSensor A3
#define rightIRSensor A1
#define rightEncoder 3
#define leftEncoder 2

namespace hova {

//left encoder ISR
void leftEncoderUpdate();

//right encoder ISR
void rightEncoderUpdate();

void resetEncoders();

void buttonUpdate();
}

//#endif
