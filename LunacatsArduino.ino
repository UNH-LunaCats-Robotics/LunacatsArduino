#include <ArduinoJson.h>
#include <MoveRobot.h>
#include <Wire.h>
#include <LIDARLite.h>

#define FORWARD 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define STOP 4
#define GETPIXY 5
#define GETLIDAR 6

#define VL6180X_ADDRESS 0x29

#define STEPS 200

String buff;
MoveRobot robot;

LIDARLite lidarLite;

const int stepPin = 8; 
const int dirPin = 9; 

/*
 * const int stepPin = 3; 
const int dirPin = 4; 
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}
void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 400; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  Serial.setTimeout(50);
  robot.start(10,11);
  
  //setup Lidar
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations

  //setup stepper motor
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  
}

enum DriveCommand {
  forward = FORWARD,
  back = BACK,
  left = LEFT,
  right = RIGHT,
  stop = STOP
};

DriveCommand lastCommand = stop;

int getLidarCm(){
  return lidarLite.distance(false);
}
//Recieve a new command through the arduino {c:9}
//ZZ

void parseCommand() {

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(buff);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("{'error':'Bad Command', 'cmd':'"+buff+"'}");
    return;
  }

  if(root["c"] == 9) {
    rotateStepperMotor();
  }
  else if(root["c"] == 6) {
  Serial.println("{\"L\":" + String(getLidarCm()) + "}");
  }
  else {
    executeMotorCommand(root);
  }
}

void rotateStepperMotor() {
//  stepper.step(1);
  Serial.println("Moving 1 step");
}

void executeMotorCommand(JsonObject& root) {
  int cmd = root["c"];
  switch(cmd) {
    case FORWARD:
      lastCommand = forward;
      if(root.containsKey("timing")) {
        Serial.println("Going Forward With Timing");
        robot.forward(root["p"],root["t"]);
      }
      else {
        Serial.println("Going Forward");
        robot.forward(root["p"]);
      }
      break;
    case BACK:
      lastCommand = back;
      if(root.containsKey("timing")) {
        Serial.println("Going Back With Timing");
        robot.back(root["p"],root["t"]);
      }
      else {
        Serial.println("Going Back");
        robot.back(root["p"]);
      }
      break;
    case LEFT:
      lastCommand = left;
      if(root.containsKey("timing")) {
        Serial.println("Going Left With Timing");
        robot.left(root["p"],root["t"]);
      }
      else {
        Serial.println("Going Left");
        robot.left(root["p"]);
      }
      break;
    case RIGHT:
      lastCommand = right;
      if(root.containsKey("timing")) {
        Serial.println("Going Right With Timing");
        robot.right(root["p"],root["t"]);
      }
      else {
        Serial.println("Going Right");
        robot.right(root["p"]);
      }
      break;
    case STOP:
      lastCommand = stop;
      Serial.println("Stopping");
      robot.stop();
      break;
  }
}



void loop() {
  
  if (Serial.available() > 0) { //&& commandRecieved == false && count++ < 50) {
    buff = Serial.readString();
    parseCommand();
  }

  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 400; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
  
}
