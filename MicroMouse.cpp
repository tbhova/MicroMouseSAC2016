#include "MicroMouse.h"
//#include <assert.h>
#include "globals.h"

using namespace hova;

MicroMouse::MicroMouse(unsigned char corner) {
  //motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  
  switch (corner) {
  case 0: //initial corner (south west)
    CurrentPosition.x = 0;
    CurrentPosition.y = 0;
    CurrentPosition.dir = north;
    break;
  case 1: //northwest corner
    CurrentPosition.x = 0;
    CurrentPosition.y = 15;
    CurrentPosition.dir = south;
    break;
  case 2: //northeast corner
    CurrentPosition.x = 15;
    CurrentPosition.y = 15;
    CurrentPosition.dir = south;
    break;
  case 3: //southeast corner
    CurrentPosition.x = 15;
    CurrentPosition.y = 0;
    CurrentPosition.dir = north;
    break;
  default:
    CurrentPosition.x = 0;
    CurrentPosition.y = 0;
  }
}

unsigned int MicroMouse::getEncoderDistance() {
  return (leftEncoderCount + rightEncoderCount) /2;
}

void MicroMouse::updateDirection(const Cardinal &desired) {
  char delta = desired - CurrentPosition.dir;
  //return early if we are going strait
  if (delta == 0)
    return;
    
  this->CurrentPosition.dir = desired;  
  
  if (delta == 3) {
    //north to west turn left
    turn90();
    return;
  }
  if (delta == -3) {
    //west to north turn right
    turn90(true);
    return;
  }
  if (delta > 0) {
    for (byte i = 0; i < delta; i++) {
      //turn left
      turn90();
    }
  } else if (delta < 0) {
    for (byte i = 0; i < -delta; i++) {
      //turn right
      turn90(true);
    }
  }
}

void MicroMouse::turn90(bool right) {
  const short unsigned int turnSpeed = 300;
  const byte turnArch = 7; //68 * 12 / 120

  resetEncoders();
  
  char dir = 1;
  if (right)
    dir = -1;
  motors.setSpeeds(turnSpeed*dir, -turnSpeed*dir);

  //#warning //bad
  //delay(350);
  
  while(this->getEncoderDistance() < turnArch) {
    //process encoders and update motor speeds
    //break;
  }
  motors.setSpeeds(0, 0);
  resetEncoders();
}

void MicroMouse::moveForwardOneCell() {
  #define forwardSpeed 400
  resetEncoders();
  motors.setSpeeds(forwardSpeed, forwardSpeed);

  //#warning bad
  //delay(500);
  //120.5743 mm per rev
  #define encoderPulsesPerCell 18 //(180/120.5743)*12
  
  
  unsigned int distance = 0;
  while(distance < encoderPulsesPerCell) {
    distance = getEncoderDistance();

    #warning //detect wall edge and use as landmark for centering
    
    //PID Follow Wall Code (take median of 3 readings)
    #warning //makesure there is no wall in front of us
    #define isLeftWall  102
    #define isRightWall 102
    #define isFrontWall 102
    #define lTargetDist 220
    #define rTargetDist 220
    #define lTargetSpeed 400
    #define rTargetSpeed 400
    #define loopTime 2
    #define Kp 0.4
    #define Kd 0.2
    static unsigned long oldMillis = 0;
    if (oldMillis + loopTime <= millis()) {
      oldMillis = millis();
      
      short unsigned int rWall = analogRead(rightIRSensor); 
      short unsigned int lWall = analogRead(leftIRSensor);
      //short unsigned int fWall = analogRead(frontIRSensor);
      //try to eliminate some of these variables
      int errorP, errorD, totalError;
      static int oldErrorP = 0;
      
      if (rWall >= isRightWall && lWall >= isLeftWall) {
        //if right and left wall
        //Serial.print("both ");
        
        errorP = rWall - lWall;
        errorD = errorP - oldErrorP;
      } else if (rWall >= isRightWall) {
        //else if right wall
        //Serial.print("right ");
    
        errorP = 2 * (rWall - rTargetDist);
        errorD = errorP - oldErrorP;
      } else if (lWall >= isLeftWall) {
        //else if left wall
        //Serial.print("left ");
    
        errorP = 2 * (lTargetDist - lWall);
        errorD = errorP - oldErrorP;
      } else {
        //else dead reckoning
        //Serial.print("dead ");
        errorP = 0;
        errorD = 0;
      }
    
      totalError = Kp * errorP + Kd * errorD;
      oldErrorP = errorP;
      /*Serial.print(lWall);
      Serial.print(' ');
      Serial.print(rWall);
      Serial.print(' ');
      Serial.println(totalError);*/
    
      motors.setSpeeds(lTargetSpeed + totalError, rTargetSpeed - totalError);
    } 
  }
  motors.setSpeeds(0, 0);
  resetEncoders();
}

void MicroMouse::discoverWalls() {
  #define isWallPresent 105
  
  wallsSeen = 0; //reset all bits
  
  //get median of 5 readings for each wall
  //back wall is certainly open so only process 3 front walls
  //if (frontSensor->ping() < isWallPresent) {
  if (analogRead(frontIRSensor) > isWallPresent) {
    wallsSeen |= (1 << CurrentPosition.dir);
  }
  //if (leftSensor->ping() < isWallPresent) {
  if (analogRead(leftIRSensor) > isWallPresent) {
    unsigned char wallDir = CurrentPosition.dir - 1;
    if (wallDir == 0)
      wallDir = 4;
    wallsSeen |= (1 << wallDir);
  }
  //if (rightSensor->ping() < isWallPresent) {
  if (analogRead(leftIRSensor) < isWallPresent) {
    unsigned char wallDir = CurrentPosition.dir + 1;
    if (wallDir == 5)
      wallDir = 1;
    wallsSeen |= (1 << wallDir);
  }
}

void MicroMouse::moveTo(const Cardinal &dir, const bool mazeDiscovered) {
  this->updateDirection(dir);
  this->moveForwardOneCell();
  
  if (!mazeDiscovered) {  
      this->discoverWalls();
  }
  
  //update coordinates
  switch (dir) {
    case north:
      CurrentPosition.y++;
      break;
    case south:
      CurrentPosition.y--;
      break;
    case west:
      CurrentPosition.x--;
      break;
    case east:
      CurrentPosition.x++;
      break;
  }
}

bool MicroMouse::isWall(const Cardinal &dir) const{
  return ((wallsSeen & (1 << dir)) > 0);
}

Position MicroMouse::getPosition() const{
  return CurrentPosition;
}
