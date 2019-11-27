#include <SoftwareSerial.h>
SoftwareSerial s(5,6); //rx tx
 int data2;
void setup() {
s.begin(115200);
Serial.begin(115200);
}
 
void loop() {

if(s.available()>0)
{
 data2 = s.read();
 Serial.println(data2);
 if (data2==99){
  s.write(77);
 }
}

}
