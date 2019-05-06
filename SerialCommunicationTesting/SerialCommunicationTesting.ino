
void setup() {
 Serial.begin(115200); 
 Serial.setTimeout(20);
}

void test_w() {
   Serial.println("Pong");
   delay(10);
}

void test_wr() {
   Serial.println(Serial.readString());
}

void loop() {
 if (Serial.available() > 0)
 {
   test_wr();
 }
}
