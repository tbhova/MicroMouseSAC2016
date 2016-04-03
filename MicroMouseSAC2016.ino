#include "globals.h"
#include "maze.h"
#include "MicroMouse.h"
#include "DataTypes.h"
//#include <NewPing.h>
#include "Hardware.h"
#include <Pushbutton.h>

using namespace std;
using namespace hova;

MicroMouse *mouse;
Maze *maze;
/*NewPing *frontSensor;
NewPing *leftSensor;
NewPing *rightSensor;*/
Pushbutton button(ZUMO_BUTTON);
void setup() {
  // put your setup code here, to run once:
  mouse = new MicroMouse();
  maze = new Maze();

  pinMode(frontIRSensor, INPUT);
  pinMode(leftIRSensor, INPUT);
  pinMode(rightIRSensor, INPUT);
  pinMode(rightEncoder, INPUT);
  pinMode(leftEncoder, INPUT);

  resetEncoders();
  attachInterrupt(digitalPinToInterrupt(leftEncoder), leftEncoderUpdate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEncoder), rightEncoderUpdate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ZUMO_BUTTON), buttonUpdate, CHANGE);

  Serial.begin(115200);
  
  /*frontSensor = new NewPing(frontTP, frontEP, 18);
  leftSensor = new NewPing(leftTP, leftEP, 18);
  rightSensor = new NewPing(rightTP, rightEP, 18);*/

  /*pinMode(frontTP, OUTPUT);
  pinMode(frontEP, INPUT);
  pinMode(rightTP, OUTPUT);
  pinMode(rightEP, INPUT);
  pinMode(leftTP, OUTPUT);
  pinMode(leftEP, INPUT);*/

  maze->cellVisited(0, 0);
  mouse->discoverWalls();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //get mouse position
  Position currentPosition = mouse->getPosition();
  /*Serial.print("Mouse pos ");
  Serial.print(currentPosition.x);
  Serial.print(' ');
  Serial.print(currentPosition.y);
  switch (currentPosition.dir) {
    case (north) :
    Serial.println(" N");
    break;
    case (south) :
    Serial.println(" S");
    break;
    case (west) :
    Serial.println(" W");
    break;
    case (east) :
    Serial.println(" E");
    break;
  }
  Serial.println();*/
  //get new direction from maze and have mouse move there (maze handles optimal route)
  mouse->moveTo(maze->getDirection(currentPosition), maze->allCellsVisited());
  
  if (!maze->allCellsVisited()) {
    //update current position with mouse's new position
    currentPosition = mouse->getPosition();
    
    //state we visited this cell
    maze->cellVisited(currentPosition.x, currentPosition.y);
    
    //update walls
    for (unsigned char i = north; i <= west; i++) {
      if (mouse->isWall((Cardinal)i))
        maze->placeWall(currentPosition.x, currentPosition.y, (Cardinal)i);
    }
  }
}
