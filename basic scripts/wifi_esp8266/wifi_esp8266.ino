#include <SoftwareSerial.h>
SoftwareSerial wifi(2,3); 
/*#define SSID "a"
#define PASS "11111111"
*/
//#define SSID "iDon'tKnow"
//#define PASS "pimpleshwar1@108"

void setup() {
  Serial.begin(115200);
  connectwifi("iDon'tKnow","pimpleshwar1@108" );
}

void loop() {
  //read_data_thingspeak("KW23TE4XW3D8M8UN", "field1");
  if (wifi.available()) {
    Serial.write(wifi.read());
  }
  if (Serial.available()) {
    wifi.write(Serial.read());
  }
  //send_data_thingspeak("7W28KD9DORC47OJT", "field1", "6");
  //read_data_thingspeak("KW23TE4XW3D8M8UN", "field1");
  //delay(300000);
}

/*TEMP
AT+CIPSTART="TCP","184.106.153.149",80
AT+CIPSEND=78
GET /channels/864273/fields/1.json?api_key=KW23TE4XW3D8M8UN&results=2 HTTP/1.1
AT+CIPRECVMODE?
AT+CIPRECVLEN?
AT+CIPRECVDATA=
 */

boolean read_data_thingspeak(String read_key, String field_name){
  String cmd = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
  wifi.println(cmd);
  Serial.println(wifi.readString());
  String msg = "GET /channels/864273/fields/1.json?api_key=KW23TE4XW3D8M8UN&results=2 HTTP/1.1";
  wifi.println("AT+CIPSEND="+String(msg.length()));
  Serial.print(msg);
  wifi.print(msg);delay(10000);
  
}

///////////////////////////////////////////// USE: connectwifi(ssid,password)
boolean connectwifi(String s, String p){
  wifi.begin(115200);
  wifi.write("AT\r\n");
  if (wifi.find("OK")) {
    Serial.println("wifi hardware found");
    wifi.println("AT+CWMODE=1");
    delay(500);
    wifi_try:
    wifi.println("AT+CWJAP?");
    delay(500);
    if (wifi.find("No AP")){
      Serial.println("No wifi connected.. trying to connect");
      String cmd="AT+CWJAP=\"";cmd+=s;cmd+="\",\"";cmd+=p;cmd+="\"";
      wifi.println(cmd);delay(5000);
      if (wifi.find("OK")){Serial.println("wifi Connected");}
    }
    else {
      Serial.println("Wifi Already Connected");
      }
  }
  else {Serial.println("Check connection. wifi device not found.");}
  }

////////////////////////////////////////


boolean send_data_thingspeak(String write_key, String field_name, String data){
  String cmd = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
  wifi.println(cmd);
  String msg = "GET /update?key="+write_key+"&field1="+data+"\r\n";
  //Serial.print("AT+CIPSEND=");
  wifi.println("AT+CIPSEND="+String(msg.length()));
  Serial.print(msg);
  wifi.print(msg);
  }


//////////////////////////////////////////////















/*if (wifi.available()) {
    Serial.write(wifi.read());
  }
  if (Serial.available()) {
    wifi.write(Serial.read());
  }*///THIS IS FOR LOOP
  /*  BASIC COMMANDS : 
  DISCONNECT AP: AT+CWQAP
  CONNECT AP : AT+CWJAP="SSID","PASSWORD"
  GET IP AND SSID NAME OF CONNECTED DEVICE: AT+CWJAP?*/

