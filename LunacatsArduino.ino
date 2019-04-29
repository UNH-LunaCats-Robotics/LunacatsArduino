#include <Servo.h>

//Used for the Actuator
#define NEUTRAL 92
#define ACT_DOWN_MATCH 140
#define ACT_UP 10
#define ACT_DOWN 170
#define ACT_UP_MATCH 45

Servo FLWheel; //This is definitely correct. Good job Tom
Servo BLWheel;
Servo FRWheel;
Servo BRWheel;
Servo ActLeft;
Servo ActRight;

int Neutral = 90;
int powerOffset = 10;

void forward()
{
  FLWheel.write(Neutral + powerOffset);
  BLWheel.write(Neutral + powerOffset);
  FRWheel.write(Neutral + powerOffset);
  BRWheel.write(Neutral + powerOffset);
}

void left()
{
  FLWheel.write(Neutral - powerOffset);
  BLWheel.write(Neutral - powerOffset);
  FRWheel.write(Neutral + powerOffset);
  BRWheel.write(Neutral + powerOffset);
}

void right()
{
  FLWheel.write(Neutral + powerOffset);
  BLWheel.write(Neutral + powerOffset);
  FRWheel.write(Neutral - powerOffset);
  BRWheel.write(Neutral - powerOffset);
}

void back()
{
  FLWheel.write(Neutral - powerOffset);
  BLWheel.write(Neutral - powerOffset);
  FRWheel.write(Neutral - powerOffset);
  BRWheel.write(Neutral - powerOffset);
}

void halt()
{
  FLWheel.write(Neutral);
  BLWheel.write(Neutral);
  FRWheel.write(Neutral);
  BRWheel.write(Neutral);
}

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

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  FLWheel.attach(A0);
  BLWheel.attach(A2);
  FRWheel.attach(A1);
  BRWheel.attach(A3);
  ActLeft.attach(A5);
  ActRight.attach(A6);
  
  Serial.begin(115200);
  Serial.setTimeout(50);
  
  halt();
}

void parseCommand(int msg)
{
  if (msg == 0)
  {
    
    changeLEDState();
    back();
  }
  else if (msg == 1)
  {
    forward();
  }
  else if (msg == 2)
  {
    left();
  }
  else if (msg == 3)
  {
    right();
  } else if (msg ==10)
  {
    upAct();
  } else if (msg ==11)
  {
    downAct();
  } else if (msg ==12)
  {
    powerOffset += 10;
  } else if (msg ==13)
  {
    powerOffset -= 10;
  }
  
  else
  {
    stopAct();
    halt();
  }
  
  
}

bool flashLED = false;

void changeLEDState() {
  flashLED = !flashLED;
  if(flashLED) {
    digitalWrite(LED_BUILTIN,HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN,LOW);
  }
}

void loop()
{
  
  if (Serial.available() > 0)
  {
    int cmd = Serial.parseInt();
    parseCommand(cmd);
  }
  
}
 
