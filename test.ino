#include <ArduinoJson.h>
#include <Servo.h>


Servo FLWheel; //These could be wrong
Servo BLWheel;
Servo FRWheel;
Servo BRWheel;


void forward() { 
    int i = 110;
    FLWheel.write(i%180);
    BLWheel.write(i%180);
    FRWheel.write(i%180);
    BRWheel.write(i%180);
}


void left() { 
    int i = 90;
    FLWheel.write(i%180-25);
    BLWheel.write(i%180-25);
    FRWheel.write(25+i%180);
    BRWheel.write(25+i%180);
}

void right() { 
    int i = 90;
    FLWheel.write(25+i%180);
    BLWheel.write(25+i%180);
    FRWheel.write(i%180-25);
    BRWheel.write(i%180-25);
}

void back() { 
    int i = 70;
    FLWheel.write(i%180);
    BLWheel.write(i%180);
    FRWheel.write(i%180);
    BRWheel.write(i%180);
}

void halt() { 
    int i = 90;
    FLWheel.write(i%180);
    BLWheel.write(i%180);
    FRWheel.write(i%180);
    BRWheel.write(i%180);
}


void setup() {
//  for(int i = 0; i < 8; i++) {
//    pinMode(i,OUTPUT);
//  }
  FLWheel.attach(A0);
  BLWheel.attach(A2);
  FRWheel.attach(A1);
  BRWheel.attach(A3);

  Serial.begin(115200);
  
  halt();
}

String buff;

void loop() {

  if(Serial.available() > 0) {
    StaticJsonBuffer<200> jsonBuffer;
    String buff = Serial.readString();
    JsonObject& root = jsonBuffer.parseObject(buff);
    if(!root.success()){
      Serial.println("Bad Json String");
    } else {
      
      if(root["c"] == 0) {
        back();
      }
      else if(root["c"] == 1) {
        forward();
      }
      else if(root["c"] == 2) {
        left();
      }
      else if(root["c"] == 3) {
        right();
      }
      else {
        halt();
      }
      
    }
    
  }
}

