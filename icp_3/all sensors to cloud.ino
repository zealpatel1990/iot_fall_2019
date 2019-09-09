
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
String msg = "GET /update?key=BK3YJQUG7548CWCQ";

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
float light;
int UV_I;
float Pressure,Altitude;
float ratio = 0;
float concentration = 0;
unsigned long starttime;
unsigned long duration;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;


void setup()
{
  lcd.setBacklight(255);
  Serial.begin(9600);
  pinMode(A1,INPUT);
  pinMode(8,INPUT);
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
  
  //Light
  light=analogRead(A1);
  delay(2000);
  lcd.clear();
  lcd.print("Light: ");
  lcd.print(light);
  
  //UV
  for(int i=0;i<1024;i++){
    sensorValue=analogRead(A0);
    sum=sensorValue+sum;
    delay(2);
  }   
  sum = sum >> 10;
  Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
  lcd.setCursor(0, 1);
  if (Vsig < 50) {
    lcd.print("UV Index: 0 ");
    UV_I=0;
  }
  if (Vsig > 50 && Vsig < 227) {
    lcd.print("UV Index: 1 ");
    UV_I=1;
  }
  if (Vsig > 227 && Vsig < 318) {
    lcd.print("UV Index: 2 ");
    UV_I=2;
  }
  if (Vsig > 318 && Vsig < 408) {
    lcd.print("UV Index: 3 ");
    UV_I=3;
  }
  if (Vsig > 408 && Vsig < 503) {
    lcd.print("UV Index: 4 ");
    UV_I=4;
 }
if (Vsig > 503 && Vsig < 606) {
    lcd.print("UV Index: 5 ");
    UV_I=5;
 }
if (Vsig > 606 && Vsig < 696) {
    lcd.print("UV Index: 6 ");
    UV_I=6;
 }
if (Vsig > 696 && Vsig < 795) {
    lcd.print("UV Index: 7 ");
    UV_I=7;
 }
if (Vsig > 795 && Vsig < 881) {
    lcd.print("UV Index: 8 ");
    UV_I=8;
 }
if (Vsig > 881 && Vsig < 976) {
    lcd.print("UV Index: 9 ");
    UV_I=9;
 }
if (Vsig > 976 && Vsig < 1079) {
    lcd.print("UV Index: 10 ");
    UV_I=10;
 }
if (Vsig > 1079 && Vsig < 1170) {
    lcd.print("UV Index: 11 ");
    UV_I=11;
 }
if (Vsig > 1170) {
    lcd.print("UV Index: 11+ ");
    UV_I=12;
 }

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
  delay(2000);
    update();

  //Barometer
  lcd.clear();
  lcd.display();
  lcd.print(F("P :"));
  Pressure = bmp.readPressure();
  lcd.print(Pressure);
  lcd.print(" Pa");
  lcd.setCursor(0, 1);
  lcd.print(F("Alt "));
  Altitude = bmp.readAltitude(1013.25);
  lcd.print(Altitude); // this should be adjusted to your local forcase
  lcd.print(" m");  
  lcd.setCursor(0, 0);
  delay(2000);
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
  cmd += hum;
  cmd += "&field2=";   
  cmd += temp;
  cmd += "&field3=";
  cmd += light;
  cmd += "&field4=";
  cmd += UV_I;
  cmd += "&field5=";
  cmd += concentration;
  cmd += "\r\n";
  cmd += "&field6=";
  cmd += Pressure;
  cmd += "\r\n";
  cmd += "&field7=";
  cmd += Altitude;
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
