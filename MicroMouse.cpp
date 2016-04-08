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
  const short unsigned int leftTurnSpeed = 312, rightTurnSpeed = 250;
  const short unsigned int turnArch = 164;

  resetEncoders();
  
  char dir = 1;
  Serial.print("Turn ");
  if (right) {
    dir = -1;
    Serial.println("right");
  } else {
    Serial.println("left");
  }
  motors.setSpeeds(-leftTurnSpeed*dir, rightTurnSpeed*dir);
  
  while(this->getEncoderDistance() < turnArch)  {
    static byte i = 0;
    i++;
  }
  motors.setSpeeds(0, 0);
  Serial.print("Encoder pules ");
  Serial.println(getEncoderDistance());
  delay(500);
  resetEncoders();
}

#define isLeftWall  300
#define isRightWall 300
#define isFrontWall 300
    
bool MicroMouse::moveForwardOneCell() {
  Serial.println("Forward");
  #define leftSpeed 375
  #define rightSpeed 300
  resetEncoders();
  //motors.setSpeeds(forwardSpeed, forwardSpeed);

  byte forwardError = 0;
  
  //120.5743 mm per rev
  #define encoderPulsesPerCell 339
  bool frontWallPresent = false;
  while(getEncoderDistance() < encoderPulsesPerCell && !frontWallPresent) {
    /*Serial.print("r ");
    Serial.print(rightEncoderCount);
    Serial.print(' ');
    Serial.println(encoderPulsesPerCell);*/
    #warning //detect wall edge and use as landmark for centering
    #warning //makesure there is no wall in front of us
    #define lTargetDist 550
    #define rTargetDist 550
    #define loopTime 1
    #define Kp 0.4
    #define Kd 0.32
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
          break; //s-top loop
        }
      }
      
      #warning //try to eliminate some of these variables
      int errorP, errorD, totalError = 0;
      static int oldErrorP = 0;
      /*Serial.print("f ");
      Serial.print(fWall);
      Serial.print(" l ");
      Serial.print(lWall);
      Serial.print(" r ");
      Serial.println(rWall);*/
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
      
      motors.setSpeeds(leftSpeed - totalError, rightSpeed + totalError);
    } 
  }
  motors.setSpeeds(0, 0);
  Serial.print("Encoder pules ");
  Serial.println(getEncoderDistance());
  delay(500);
  bool ret = true;
  //determine whether we moved forward one cell
  if(frontWallPresent && getEncoderDistance() < (encoderPulsesPerCell/3))
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

void MicroMouse::calibratePosition() {
  const byte rearCalDist = 600, sideCalDist = 570, roughCalDist = 500;
  const Cardinal origDir = CurrentPosition.dir;
  
  byte currentWalls = 0;

  //for 0 to 3 (3 turns, 360*)
  for (byte  i = 0; i < 4; i++) {
    //discover walls
    this->discoverWalls();
    //find direction behind us
    Cardinal opposite;
      if (CurrentPosition.dir <= east) {
        opposite = (Cardinal)((byte)CurrentPosition.dir + 2);
      } else {
        opposite = (Cardinal)((byte)CurrentPosition.dir - 2);
      }
    for (byte j = north; j <= west; j++) {
      //check all dir but opposite
      if ((Cardinal)j == opposite)
        continue;
      
      //add/remove walls
      if (isWall((Cardinal)j)) {
        currentWalls |= (1 << j);
      } else {
        currentWalls &= ((1 << Cardinal(j)) ^ 0x0F);
      }

      //if wall in front, move slightly to it
      if (isWall(CurrentPosition.dir)) {
        
      }
      byte rDir = (byte)origDir + 1;
      rDir = (rDir > 4) ? 1 : rDir;
      
      if (isWall((Cardinal)rDir) {
        this->updateDir((Cardinal)rDir);
        

        
      }
    }
  }
  //calibrate rear

  //else if no rear, calibrate front

  //calibrate both sides if exist

  void calForwardWall() {
    resetEncoders();
    motors.setSpeeds(leftTurnSpeed, rightTurnSpeed);
    while(analogRead(fIRSensor) < roughCalDist && getEncoderDist < encoderPulsesPerCell/2) {
      byte i = 0;
      i++;
    }
    motors.setSpeeds(0,0);
    delay(500);
    resetEncoders();
  }
}
