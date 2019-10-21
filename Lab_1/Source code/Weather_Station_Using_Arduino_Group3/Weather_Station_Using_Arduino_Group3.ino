
//Libraries
#include <DHT.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define DEBUG true
SoftwareSerial esp8266(9,10); 
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
Adafruit_BMP280 bmp;

#define SSID "a"     // "SSID-WiFiname" 
#define PASS "11111111"       // "password"

#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=45IYQNDNQAXADS18";

//Variables
int chk;
float hum=0;  //Stores humidity value
float temp=0; //Stores temperature value
float light;
int UV_I;
float Pressure,Altitude=0;
float ratio = 0;
float concentration = 0;
unsigned long starttime;
unsigned long duration;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;

//pulse sensor
int analogPin= A2;
int pulsePin = 0; 


void setup()
{
  lcd.setBacklight(255);
  Serial.begin(9600);
  pinMode(7,INPUT);   //temperature sensor input
  pinMode(2,OUTPUT);  //temperature green LED
  pinMode(3,OUTPUT);  //temperature red LED

  pinMode(A1,INPUT);  //Light sensor input
  pinMode(4,OUTPUT);  //Light Green LED
  pinMode(5,OUTPUT);  //Light Red LED
  
  pinMode(A0,INPUT);  //UV sensor input
  pinMode(8,INPUT);   //Dust Sensor input
  
  pinMode(A2,INPUT);  // Pulse Input
  pinMode(13,OUTPUT);  //Pulse Green LED
  pinMode(12,OUTPUT);  //Pulse Red LED
 
  pinMode(6,OUTPUT);  //pressure green LED
  pinMode(11,OUTPUT);  //pressure red LED
  

  digitalWrite(2,0); //initialize to 0
  digitalWrite(3,0); //initialize to 0
  digitalWrite(4,0);
  digitalWrite(5,0);
  digitalWrite(6,0); //initialize to 0
  digitalWrite(11,0);
  digitalWrite(12,0);
  digitalWrite(13,0);
  dht.begin();
  starttime = millis(); 
  lcd.begin(16, 2);
  lcd.print("Group-3");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  Serial.begin(9600); //or use default 115200.
  esp8266.begin(115200);
  Serial.println("AT");
  esp8266.println("AT");
  delay(3000);
  if(esp8266.find("OK")){
    connectWiFi();
  }
  
}

void loop()
{
  delay(2000);
  int sensorValue;
  long  sum=0;
  float Vsig;
  delay(500);
  lcd.clear();
  lcd.display();
  
  //Humidity & Temperature
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(hum);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.println(" C");
  if (25 < temp && temp < 30){
    digitalWrite(3,0);
    digitalWrite(2,1); //turn green LED ON
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Normal Temp:");  
  }
  else if (temp <= 25){
    digitalWrite(2,0);
    digitalWrite(3,1); // glow red LED
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert:");
    lcd.setCursor(0, 1);
    lcd.print("Its Cold");
  }
  else {
    digitalWrite(2,0);
    digitalWrite(3,1);  // glow red LED
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert:");
    lcd.setCursor(0, 1);
    lcd.print("Its Hot");
  }

  
  //Light
  light=analogRead(A1);
  delay(2000);
  
  if (light < 200 && light > 40){
    digitalWrite(5,0);
    digitalWrite(4,1);  // glow green LED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Light: ");
    lcd.print(light);
    lcd.setCursor(0, 1);
    lcd.print("Normal Light");
    delay(1000);
  }
  else {
    digitalWrite(4,0);
    digitalWrite(5,1);  // glow red LED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Light: ");
    lcd.print(light);
    lcd.setCursor(0, 1);
    lcd.print("Its Dark/");
    lcd.print("Bright");
    delay(2000);
    lcd.clear();
  }

  //Barometer
  lcd.clear();
  lcd.display();
  lcd.print(F("P :"));
  //Pressure = bmp.readPressure();
  Pressure = 101000;
  lcd.print(Pressure);
  lcd.print(" Pa");
  lcd.setCursor(0, 1);
  lcd.print(F("Alt "));
  Altitude = bmp.readAltitude();
  lcd.print(Altitude); // this should be adjusted to your local forcase
  lcd.print(" m");  
  delay(1000);
  if (90000 < Pressure && Pressure < 109000){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Normal Press");
    delay(1000);
    digitalWrite(11,0);
    digitalWrite(6,1); //turn green LED ON  
  }
  else {
    digitalWrite(6,0);
    digitalWrite(11,1); // glow red LED
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert:low/");
    lcd.setCursor(0, 1);
    lcd.print("High Press");
    delay(1000);
  }
  
  //UV
  lcd.clear();
  lcd.display();
  lcd.setCursor(0, 0);
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
 
if (Vsig < 50) {
    lcd.print("UV Index: 0 ");
    lcd.setCursor(0, 1);
    lcd.print("LOW ");
 }
if (Vsig > 50 && Vsig < 227) {
    lcd.print("UV Index: 1 "); 
 }
if (Vsig > 227 && Vsig < 318) {
    lcd.print("UV Index: 2 "); 
 }
if (Vsig > 318 && Vsig < 408) {
    lcd.print("UV Index: 3 "); 
 }
if (Vsig > 408 && Vsig < 503) {
    lcd.print("UV Index: 4 "); 
 }
if (Vsig > 503 && Vsig < 606) {
    lcd.print("UV Index: 5 "); 
 }
if (Vsig > 606 && Vsig < 696) {
    lcd.print("UV Index: 6 "); lcd.print("HIGH (Get out from the sunlight! get out now!) ");
 }
if (Vsig > 696 && Vsig < 795) {
    lcd.print("UV Index: 7 "); lcd.print("HIGH (Get out from the sunlight! get out now!) ");
 }
if (Vsig > 795 && Vsig < 881) {
    lcd.print("UV Index: 8 "); lcd.print("VERY HIGH (Get out from the sunlight! get out now!) ");
 }
if (Vsig > 881 && Vsig < 976) {
    lcd.print("UV Index: 9 "); lcd.print("VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
 }
if (Vsig > 976 && Vsig < 1079) {
    lcd.print("UV Index: 10 "); lcd.print("VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
 }
if (Vsig > 1079 && Vsig < 1170) {
    lcd.print("UV Index: 11 "); lcd.print("EXTREME (If you value your health, don't go outside, just stay at home!) ");
 }
if (Vsig > 1170) {
    lcd.print("UV Index: 11+ "); lcd.print("EXTREME (You will probably die in 3, 2, 1... Just JOKING, don't be scared...) - intensity of sunlight is really at maximum ");
 }
 delay(1000);


  //dust
  lcd.setCursor(0, 0);
  lcd.clear();
  duration = pulseIn(8, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
    lcd.print("Conc : ");
    lcd.print(concentration);
    lcd.setCursor(0,1);
    lcd.print(" pcs/0.01cf");
    lcd.print("\n");
    lowpulseoccupancy = 0;
    starttime = millis();
  }
  delay(1000);
  
  //pulse sensor
  lcd.clear();
  lcd.display();
  lcd.setCursor(0,0);
  lcd.print("BPM = ");
  delay(500);
  pulsePin = (analogRead(analogPin)/5);
  if (pulsePin < 200 && pulsePin > 60)
     {
      lcd.setCursor(0, 1);
      lcd.print(pulsePin);
      delay(100);
      digitalWrite(12,0);
      digitalWrite(13,1);  // glow green LED
      
     }
  else
    {
      lcd.setCursor(0, 1);
      lcd.print("Invalid");
      delay(100);
      digitalWrite(13,0);
      digitalWrite(12,1);  // glow red LED
    }

    delay(1000);
    update();
}

void update(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field1=";   
  cmd += temp;
  cmd += "&field2=";   
  cmd += hum;
  cmd += "&field3=";
  cmd += light;
  cmd += "&field4=";
  cmd += UV_I;
  cmd += "&field5=";
  cmd += concentration;
  cmd += "&field6=";
  cmd += Pressure;
  cmd += "&field7=";
  cmd += pulsePin;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);

}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if(esp8266.find("OK")){
    Serial.println("OK");
    return true;    
  }else{
    return false;
  }
}  
