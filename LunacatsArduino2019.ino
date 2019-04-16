#include <ArduinoJson.h>
#include <MoveRobot.h>
#include <Wire.h>
#include <LIDARLite.h>
#include <Stepper.h>

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

Stepper stepper(STEPS, 8, 10, 9, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  Serial.setTimeout(50);
  robot.start(10,11);
  
  //setup Lidar
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations

  //setup stepper motor
  stepper.setSpeed(200);
  
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
  stepper.step(90);
  Serial.println("Moving 90 steps");
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
  
}
