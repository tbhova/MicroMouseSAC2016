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
  //motors.setSpeeds(50,50);
}

unsigned int MicroMouse::getEncoderDistance() {
  return (leftEncoderCount + rightEncoderCount) /2;
}

void MicroMouse::updateDirection(const Cardinal &desired) {
  /*Serial.print("desired ");
  Serial.print(desired);
  Serial.print(" cur ");
  Serial.println(CurrentPosition.dir);*/
  char delta = (char)desired - (char)CurrentPosition.dir;
  //return early if we are going strait
  if (delta == 0)
    return;
    
  this->CurrentPosition.dir = desired;  
  
  if (delta == 3) {
    //north to west turn left
    turn90();
  } else if (delta == -3) {
    //west to north turn right
    turn90(true);
  } else if (delta > 0) {
    for (byte i = 0; i < delta; i++) {
      //turn right
      turn90(true);
    }
  } else if (delta < 0) {
    for (byte i = 0; i < -delta; i++) {
      //turn left
      turn90();
    }
  }
}

void MicroMouse::turn90(bool right) {
  const short unsigned int turnSpeed = 300;
  const byte turnArch = 8; //68 * 12 / 120   68mm turn radius, 12 encoder pulses per 120mm of movement

  resetEncoders();
  
  char dir = 1;
  Serial.print("Turn ");
  if (right) {
    dir = -1;
    Serial.println("right");
  } else {
    Serial.println("left");
  }
  motors.setSpeeds(-turnSpeed*dir, turnSpeed*dir);

  //#warning //bad
  //delay(350);
  
  while(this->getEncoderDistance() < turnArch)  {
    static byte i = 0;
    i++;
  }
  motors.setSpeeds(0, 0);
  delay(2);
  resetEncoders();
}

#define isLeftWall  300
#define isRightWall 300
#define isFrontWall 300
    
bool MicroMouse::moveForwardOneCell() {
  Serial.println("Forward");
  #define forwardSpeed 400
  resetEncoders();
  //motors.setSpeeds(forwardSpeed, forwardSpeed);

  byte forwardError = 0;
  //#warning bad
  //delay(500);
  //120.5743 mm per rev
  #define encoderPulsesPerCell 20 //(180/120.5743)*12
  bool frontWallPresent = false;
  while(getEncoderDistance() < encoderPulsesPerCell && !frontWallPresent) {
    /*Serial.print("r ");
    Serial.print(rightEncoderCount);
    Serial.print(' ');
    Serial.println(encoderPulsesPerCell);*/
    #warning //detect wall edge and use as landmark for centering
    
    //PID Follow Wall Code (take median of 3 readings)
    #warning //makesure there is no wall in front of us
    #define lTargetDist 450
    #define rTargetDist 450
    #define loopTime 1
    #define Kp 0.5
    #define Kd 0.8
    #define frontStoppingDistance 400
    
    static unsigned long oldMillis = 0;
    if (oldMillis + loopTime <= millis()) {
      oldMillis = millis();
      
      short unsigned int rWall = analogRead(rightIRSensor); 
      short unsigned int lWall = analogRead(leftIRSensor);
      short unsigned int fWall = analogRead(frontIRSensor);

      //check for front wall (landmark) and stop if found
      if (fWall > frontStoppingDistance) {
        forwardError++;
        if (forwardError > 2) { //if we have have seen at least 3 readings with the front wall present
          frontWallPresent = true;
          break; //stop loop
        }
      }
      
      #warning //try to eliminate some of these variables
      int errorP, errorD, totalError = 0;
      static int oldErrorP = 0;
      Serial.print("f ");
      Serial.print(fWall);
      Serial.print(" l ");
      Serial.print(lWall);
      Serial.print(" r ");
      Serial.println(rWall);
      delay(2);
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
      
      motors.setSpeeds(forwardSpeed - totalError, forwardSpeed + totalError);
    } 
  }
  motors.setSpeeds(0, 0);
  delay(500);
  bool ret = true;
  //determine whether we moved forward one cell
  if(frontWallPresent && getEncoderDistance() < encoderPulsesPerCell/3)
    ret = false;
  resetEncoders();
  return ret;
}

void MicroMouse::discoverWalls() {
  //#define isWallPresent 115
  
  wallsSeen = 0; //reset all bits
  Serial.print("Add walls ");
  //get median of 5 readings for each wall
  //back wall is certainly open so only process 3 front walls
  //if (frontSensor->ping() < isWallPresent) {
  if (analogRead(frontIRSensor) > isRightWall) {
    Serial.print("front ");
    wallsSeen |= (1 << CurrentPosition.dir);
  }
  //if (leftSensor->ping() < isWallPresent) {
  if (analogRead(leftIRSensor) > isLeftWall) {
    Serial.print("left ");
    unsigned char wallDir = CurrentPosition.dir - 1;
    if (wallDir == 0)
      wallDir = 4;
    wallsSeen |= (1 << wallDir);
  }
  //if (rightSensor->ping() < isWallPresent) {
  if (analogRead(rightIRSensor) > isRightWall) {
    Serial.print("right ");
    unsigned char wallDir = CurrentPosition.dir + 1;
    if (wallDir == 5)
      wallDir = 1;
    wallsSeen |= (1 << wallDir);
  }
  Serial.println();
}

void MicroMouse::moveTo(const Cardinal &dir, const bool mazeDiscovered) {
  this->updateDirection(dir);
  bool forwardSuccessful = this->moveForwardOneCell();
  
  if (!mazeDiscovered) {  
      this->discoverWalls();
  }

  if (forwardSuccessful) {
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
}

bool MicroMouse::isWall(const Cardinal &dir) const{
  return ((wallsSeen & (1 << dir)) > 0);
}

Position MicroMouse::getPosition() const{
  return CurrentPosition;
}
