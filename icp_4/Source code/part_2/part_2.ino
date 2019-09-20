#include <DHT.h>
#include <SoftwareSerial.h>
SoftwareSerial esp8266(9,10); 

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

#define SSID "a"     // "SSID-WiFiname" 
#define PASS "11111111"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=7W28KD9DORC47OJT";

//Variables
float temp; //Stores temperature value

void setup()
{
  pinMode(A1,INPUT);
  pinMode(8,INPUT);
  dht.begin();
  delay(100);
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
  //Temperature
  temp= dht.readTemperature();
  
  delay(2000);
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
