#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  Serial.begin(115200);
  myservo.attach(9);
  myservo.write(100);  // set servo to mid-point
} 

void loop() {
  if (Serial.available() > 0) {
  String a = Serial.readString();
  myservo.write(a.toInt());
  Serial.println(a);
  } 
}
