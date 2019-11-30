#include <Servo.h> 
Servo servo_rotate;
Servo servo_grabber;
Servo servo_height;
Servo servo_extender;

 void setup() 
{ 
  Serial.begin(115200);
  /*servo_extender.attach(8);servo_extender.write(90);
  servo_rotate.attach(9);  servo_rotate.write(90);
  servo_grabber.attach(10);  servo_grabber.write(90);*/
  servo_height.attach(11);  servo_height.write(90);
}

void loop() {
  if (Serial.available() > 0) {
  String a = Serial.readString();
  test(a.toInt());  
  }
}
