/******************************************************************
 * author: Balázs Simon
 * e-mail: simon.balazs.1992@gmail.com
 * 
 * This code controls a robot arm. An Arduino 101 is used in this project. 
 * If you plan to use this code on another Arduino you might need to use Serial
 * instead of Serial
 * More info:
 * https://www.hackster.io/Abysmal/robot-arm-controlled-through-ethernet-5ce9ce
 ******************************************************************/
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <WiFi.h>
#include <SPI.h>

#include <SoftwareSerial.h>
#define RX 12
#define TX 13
String AP = "";       // CHANGE ME
String PASS = ""; // CHANGE ME
String API = "FMUZZLMYQ2O8SNMR";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 


#include <Servo.h>

// Used pins
#define GRIPPER_SERVO 9
#define UPPER_JOINT_SERVO 6
#define LOWER_JOINT_SERVO 5
#define BASE_ROTATOR_JOINT_SERVO 3

// Gripper states
#define GRIPPER_OPEN 0
#define GRIPPER_CLOSED_SOFT 1
#define GRIPPER_CLOSED_MEDIUM 2
#define GRIPPER_CLOSED_HARD 3

// Gripper configuration, specific to your robot
#define GRIPPER_OPEN_ANGLE 150            // The angle of the servo in different positions
#define GRIPPER_CLOSED_SOFT_ANGLE 75      // Closing with low force (less stressful on the servo)
#define GRIPPER_CLOSED_MEDIUM_ANGLE 65    // Closing with medium force (slightly stressful on the servo)
#define GRIPPER_CLOSED_HARD_ANGLE 0       // Closing with high force (quite stressful on the servo, it will also heat up)

// Limits, specific to your robot
#define SERVO_ANGLE_UPPER_LIMIT 170
#define SERVO_ANGLE_LOWER_LIMIT 0
#define SERVO_ANGLE_RESTING 90
#define MIN_MOVEMENT_DELAY 2

// Commands
#define MOVE_GRIPPER 'G'
#define MOVE_ARM 'A'

Servo gripper, upperJoint, lowerJoint, baseRotator;

unsigned int movementDelay = 5;

int gripperState;
int targetUpperJointAngle;
int targetLowerJointAngle;
int targetBaseRotatorAngle;

float currentUpperJointAngle, currentLowerJointAngle, currentBaseRotatorAngle;
float upperJointStep, lowerJointStep, baseRotatorStep;

unsigned long lastStepTime = 0;


// Start a TCP Server on port 5045
WiFiServer server(5045);

void setup() {
  Serial.begin(115200);

  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  
  Serial.print("Setup:" + gripperState + targetUpperJointAngle + targetLowerJointAngle + targetBaseRotatorAngle);

  gripper.attach(GRIPPER_SERVO);
  Serial.println("Attach gripper");
  upperJoint.attach(UPPER_JOINT_SERVO);
  Serial.println("Attach upperJoint");
  lowerJoint.attach(LOWER_JOINT_SERVO);
   Serial.println("Attach lowerJoint");
  baseRotator.attach(BASE_ROTATOR_JOINT_SERVO);
   Serial.println("Attach baseRotator");

  //Setting default position on starting. Specific for your robot, so you 
  //might need to adjust it to yours
  gripperState = GRIPPER_CLOSED_SOFT;
  targetUpperJointAngle = currentUpperJointAngle = 140;
  targetLowerJointAngle = currentLowerJointAngle = 45;
  targetBaseRotatorAngle = currentBaseRotatorAngle = 85;

  moveGripper(gripperState);
  moveTo(targetUpperJointAngle, targetLowerJointAngle, targetBaseRotatorAngle);

  Serial.println("Setup:" + gripperState + targetUpperJointAngle + targetLowerJointAngle + targetBaseRotatorAngle);

    server.begin();
    Serial.print("Connected to wifi. My address:");
    IPAddress myAddress = WiFi.localIP();
    Serial.println(myAddress);
}

void loop() {
  Serial.println("Start");
  valSensor = getSensorData();
  String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);
  delay(3500);
  countTrueCommand++;
  //sendCommand("AT+CIPCLOSE=0",5,"OK");
  
  if (Serial.available() > 0) {
    // The first character means the received command type.
    char command = Serial.read();
    
    Serial.print(command);
    if (command == MOVE_GRIPPER) {
      updateGripperState();
      moveGripper(gripperState);
      delay(250);
    }
    else if (command == MOVE_ARM) {
      updateArmMovement();
    }
  }

  if (millis() - lastStepTime >= movementDelay) {
    if (targetUpperJointAngle != round(currentUpperJointAngle)) currentUpperJointAngle -= upperJointStep;
    if (targetLowerJointAngle != round(currentLowerJointAngle)) currentLowerJointAngle -= lowerJointStep;
    if (targetBaseRotatorAngle != round(currentBaseRotatorAngle)) currentBaseRotatorAngle -= baseRotatorStep;

    moveTo(currentUpperJointAngle, currentLowerJointAngle, currentBaseRotatorAngle);

    printPosition();
    lastStepTime = millis();
  }
}

void updateGripperState() {
  if (Serial.available() > 0)
    gripperState = Serial.parseInt();
  else
    return;

  clearSerial();
}

void updateArmMovement() {
  int mDelay;
  if (Serial.available() > 0)
    mDelay = Serial.parseInt();
  else
    return;

  int upperJointAngle;
  if (Serial.available() > 0)
    upperJointAngle = Serial.parseInt();
  else
    return;

  int lowerJointAngle;
  if (Serial.available() > 0)
    lowerJointAngle = Serial.parseInt();
  else
    return;

  int baseRotatorAngle;
  if (Serial.available() > 0)
    baseRotatorAngle = Serial.parseInt();
  else
    return;

  clearSerial();

  // This calculation is used to slow down a movement of the robot arm segments.
  // mDelay is used as speed. Instantious movement would be too fast and it could
  // cause hardware issues.
  movementDelay = mDelay >= MIN_MOVEMENT_DELAY ? mDelay : MIN_MOVEMENT_DELAY;
  targetUpperJointAngle = upperJointAngle;
  targetLowerJointAngle = lowerJointAngle;
  targetBaseRotatorAngle = baseRotatorAngle;
  upperJointStep = (currentUpperJointAngle - targetUpperJointAngle) / 100;
  lowerJointStep = (currentLowerJointAngle - targetLowerJointAngle) / 100;
  baseRotatorStep = (currentBaseRotatorAngle - targetBaseRotatorAngle) / 100;
}

void clearSerial() {
  // Clearing the serial buffer. New line and/or carriage return characters
  // are expected to be in the buffer.
  while (Serial.available() > 0)
    Serial.read();
}

void moveGripper(int state) {
  switch (state) {
    case GRIPPER_OPEN:
      gripper.write(GRIPPER_OPEN_ANGLE);
      break;
    case GRIPPER_CLOSED_SOFT:
      gripper.write(GRIPPER_CLOSED_SOFT_ANGLE);
      break;
    case GRIPPER_CLOSED_MEDIUM:
      gripper.write(GRIPPER_CLOSED_MEDIUM_ANGLE);
      break;
    case GRIPPER_CLOSED_HARD:
      gripper.write(GRIPPER_CLOSED_HARD_ANGLE);
      break;
  }
}

void moveTo(int upperJointAngle, int lowerJointAngle, int baseRotatorAngle) {
  // Moving the robot arm segments with updating the servos
  upperJoint.write(upperJointAngle);
  lowerJoint.write(lowerJointAngle);
  baseRotator.write(baseRotatorAngle);
}

void printPosition() {
  // Sending back feedback
  Serial.print(movementDelay);
  Serial.print(" ");
  Serial.print(gripperState);
  Serial.print(" ");
  Serial.print(round(currentUpperJointAngle));
  Serial.print(" ");
  Serial.print(round(currentLowerJointAngle));
  Serial.print(" ");
  Serial.println(round(currentBaseRotatorAngle));
}


int getSensorData(){
  return random(1000); // Replace with 
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
