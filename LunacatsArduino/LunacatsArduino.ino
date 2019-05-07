#include <ArduinoJson.h>
#include <Servo.h>

//##########################################//
//                DEFINITIONS               //
//##########################################//

//Used for the Actuator
#define NEUTRAL 92
#define ACT_DOWN_MATCH 140
#define ACT_UP 10
#define ACT_DOWN 170
#define ACT_UP_MATCH 45

#define AUGUR_ON_OFF 28
#define AUGUR_DIRECTION 22

Servo FLWheel; //This is definitely correct. Good job Tom
Servo BLWheel;
Servo FRWheel;
Servo BRWheel;
Servo ActLeft;
Servo ActRight;
Servo BallScrews;
Servo Conveyor;

//##########################################//
//                 MOVEMENT                 //
//##########################################//

// ------------ MOTOR FUNCTIONS ------------ //

// #-NOTE-# 90 is not exact 0 for all motor 
// encoders due to variation in creation. 
// Therefore, trial and error determined 
// the offset of the values read. 

// ZERO     - Solid Orange
// BACKWARD - flashing or solid red   (full speed = solid)
// FORWARD  - flashing or solid green (full speed = solid)

void powerWheel_FL(int i) {
  FLWheel.write(i+1);
}

void powerWheel_FR(int i) {
  FRWheel.write(i+2);
}

void powerWheel_BL(int i) {
  BLWheel.write(i+1);
}

void powerWheel_BR(int i) {
  BRWheel.write(i+1);
}

int offset = 0;
void forward()
{
  int i = 110;
  powerWheel_FL(i + offset);
  BLWheel.write(i + offset);
  FRWheel.write(i + offset);
  BRWheel.write(i + offset);
}

void left()
{
  int i = 90;
  powerWheel_FL(i - 25 - offset);
  powerWheel_BL(i - 25 - offset);
  powerWheel_FR(25 + i + offset);
  powerWheel_BR(25 + i + offset);
}

void right()
{
  int i = 90;
  powerWheel_FL(25 + i + offset);
  powerWheel_BL(25 + i + offset);
  powerWheel_FR(i - 25 - offset);
  powerWheel_BR(i - 25 - offset);
}

void back()
{
  int i = 70;
  powerWheel_FL(i - offset);
  powerWheel_BL(i - offset);
  powerWheel_FR(i - offset);
  powerWheel_BR(i - offset);
}

void halt()
{
  int i = 90;
  powerWheel_FL(i);
  powerWheel_BL(i);
  powerWheel_FR(i);
  powerWheel_BR(i);
}

// ------------ ACTUATOR ------------ //

void upAct()
{
  ActLeft.write(ACT_UP);
  ActRight.write(ACT_UP_MATCH);
}

void stopAct() {
  ActLeft.write(NEUTRAL);
  ActRight.write(NEUTRAL);
}

void downAct()
{
  ActLeft.write(ACT_DOWN);
  ActRight.write(ACT_DOWN_MATCH);
}


// ------------ DRILL ------------ //

void drillUp()
{
  ActLeft.write(ACT_DOWN);
  ActRight.write(ACT_DOWN_MATCH);
}

// ------------ BALL SCREWS------------ //

void ballsDrop()
{
  BallScrews.write(10);
}
void ballsUp()
{
  BallScrews.write(170);
}
void ballsHalt()
{
  BallScrews.write(90);
}

// ------------ CONVEYOR ------------ //

void ConveyorEmpty()
{
  Conveyor.write(10);
}
void ConveyorCollect()
{
  Conveyor.write(170);
}
void ConveyorHalt()
{
  Conveyor.write(90);
}

// ------------ AUGER ------------ //

void turnAugurClockwise() {
  digitalWrite(AUGUR_ON_OFF,HIGH);
  digitalWrite(AUGUR_DIRECTION,LOW);
}
void turnAugurCounterClockwise() {
  digitalWrite(AUGUR_ON_OFF,HIGH);
  digitalWrite(AUGUR_DIRECTION,HIGH);
}
void turnAugurOff() {
  digitalWrite(AUGUR_ON_OFF,   LOW);
  digitalWrite(AUGUR_DIRECTION,LOW);
}

//##########################################//
//                   MAIN                   //
//##########################################//

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  FLWheel.attach(A0);
  BLWheel.attach(A2);
  FRWheel.attach(A1);
  BRWheel.attach(A3);
  BallScrews.attach(A4);
  ActLeft.attach(A5);
  ActRight.attach(A6);
  Conveyor.attach(A7);
  
  
  Serial.begin(115200);
  Serial.setTimeout(20);

  pinMode(AUGUR_ON_OFF,OUTPUT);
  pinMode(AUGUR_DIRECTION,OUTPUT);

  
  halt();
}


void parseCommand(String buff)
{
  Serial.println(buff);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(buff);
  if (!root.success())
  {
//    Serial.println("Bad Json String");
  }
  else
  {

    if (root["c"] == 0)
    {
      back();
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (root["c"] == 1)
    {
      forward();
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (root["c"] == 2)
    {
      left();
    }
    else if (root["c"] == 3)
    {
      right();
    } else if (root["c"] ==10)
    {
      upAct();
    } else if (root["c"] ==11)
    {
      downAct();
    } else if (root["c"] ==12)
    {
      ballsDrop();
    } else if (root["c"] ==13)
    {
      ballsUp();
    } else if (root["c"] ==14)
    {
      ConveyorCollect();
    } else if (root["c"] ==15)
    {
      ConveyorEmpty();
    } else if (root["c"] == 16)
    {
      turnAugurClockwise();
    }
    else if (root["c"] == 17)
    {
      turnAugurCounterClockwise();
    }
    else if (root["c"] == 18)
    {
      ballsUp();
    }
    else if (root["c"] == 19)
    {
      ballsDrop();
    }
    else if (root["c"] == 20)
    {
      offset += 5;
    }
    else if(root["c"] == 21)
    {
      offset -= 5;
    }
    else
    {
//      Serial.println("wrong command");
      stopAct();
      halt();
      turnAugurOff();
      ConveyorHalt();
      ballsHalt();
    }

  }
}

void loop()
{
  // interpretCommand();

  if (Serial.available() > 0)
  {
    String recieved = Serial.readString();
    if(recieved == "init"){
      Serial.println("success");
    }
    else{
      parseCommand(recieved);
    }
  }
  
}
