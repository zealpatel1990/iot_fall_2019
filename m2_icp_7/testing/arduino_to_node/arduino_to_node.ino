#include <SoftwareSerial.h>
SoftwareSerial s(3, 2); //rx tx
char temp_char;
void setup() {
  s.begin(115200);
  Serial.begin(115200);
}

void loop() {
  String data_from_node = "";
  while (s.available() > 0)
  {
    temp_char = s.read();
    data_from_node = data_from_node + temp_char;
  }
  if (data_from_node!=""){
    if (data_from_node.indexOf("app/rot:160")>0) {
      s.write(1);
      Serial.println(data_from_node);
    }
    else{
      s.write(2);
      Serial.println("Error in data");
    }
  }
}
