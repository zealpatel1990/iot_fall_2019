// Arduino code is available to download - link below the video.

// UV index explanation: http://www2.epa.gov/sunwise/uv-index-scale
// UV index explanation: http://www.epa.gov/sunwise/doc/what_is_uvindex.html

/* 
Connection:

 Arduino           Ultraviolet (UV) sensor
 A0 (Analog 0)          SIG
 +5V                    VCC
 GND                    GND
*/

float Vsig;

void setup(){
  Serial.begin(9600);
}
 
void loop()
{  
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
 Serial.print("The voltage value: ");
 Serial.print(Vsig);
 Serial.print(" mV    --   ");
 
if (Vsig < 50) {
    Serial.print("UV Index: 0 "); Serial.println("   Exposure level - NONE (You're probably at home!) ");
 }
if (Vsig > 50 && Vsig < 227) {
    Serial.print("UV Index: 1 "); Serial.println("   Exposure level - LOW (You're probably at home!) ");
 }
if (Vsig > 227 && Vsig < 318) {
    Serial.print("UV Index: 2 "); Serial.println("   Exposure level - LOW (You can go outside and have fun!) ");
 }
if (Vsig > 318 && Vsig < 408) {
    Serial.print("UV Index: 3 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
 }
if (Vsig > 408 && Vsig < 503) {
    Serial.print("UV Index: 4 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
 }
if (Vsig > 503 && Vsig < 606) {
    Serial.print("UV Index: 5 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
 }
if (Vsig > 606 && Vsig < 696) {
    Serial.print("UV Index: 6 "); Serial.println("   Exposure level - HIGH (Get out from the sunlight! get out now!) ");
 }
if (Vsig > 696 && Vsig < 795) {
    Serial.print("UV Index: 7 "); Serial.println("   Exposure level - HIGH (Get out from the sunlight! get out now!) ");
 }
if (Vsig > 795 && Vsig < 881) {
    Serial.print("UV Index: 8 "); Serial.println("   Exposure level - VERY HIGH (Get out from the sunlight! get out now!) ");
 }
if (Vsig > 881 && Vsig < 976) {
    Serial.print("UV Index: 9 "); Serial.println("   Exposure level - VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
 }
if (Vsig > 976 && Vsig < 1079) {
    Serial.print("UV Index: 10 "); Serial.println("   Exposure level - VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
 }
if (Vsig > 1079 && Vsig < 1170) {
    Serial.print("UV Index: 11 "); Serial.println("   Exposure level - EXTREME (If you value your health, don't go outside, just stay at home!) ");
 }
if (Vsig > 1170) {
    Serial.print("UV Index: 11+ "); Serial.println("   Exposure level - EXTREME (You will probably die in 3, 2, 1... Just JOKING, don't be scared...) - intensity of sunlight is really at maximum ");
 }
}
