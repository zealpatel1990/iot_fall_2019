import paho.mqtt.client as paho
import time
output = "stop";temp=""
def on_publish(client, userdata, mid):
    return mid

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, msg):
    global output
    output = str(msg.payload)
    #print(msg.topic+" "+str(msg.qos)+": "+str(msg.payload))    

def distance():
    GPIO.output(GPIO_TRIGGER, True)
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER, False) 
    StartTime = time.time()
    StopTime = time.time()
    while GPIO.input(GPIO_ECHO) == 0:
        StartTime = time.time()
    while GPIO.input(GPIO_ECHO) == 1:
        StopTime = time.time()
    TimeElapsed = StopTime - StartTime
    distance = (TimeElapsed * 34300) / 2
    return distance
 
client = paho.Client()
client.on_publish = on_publish
client.on_subscribe = on_subscribe
client.on_message = on_message
client.connect("broker.mqttdashboard.com", 1883)
client.subscribe("iot_class_icp4", qos=2)
client.loop_start()

# https://tutorials-raspberrypi.com/raspberry-pi-ultrasonic-sensor-hc-sr04/
while 1:
    distance = 20 #distance is in Centimeter
    if distance > 10:
        if temp != output: print output;temp = output;(rc, mid) = client.publish("iot_class_icp4/recv", "Goooo roos", qos=1)
        if output == "forward":
            a=1
        elif output == "backward":
            a=1
        elif output == "left":
            a=1
        elif output == "right":
            a=1
        elif output == "stop":
            a=1
    else:
        print ("Obstacle within 10 cm")
        (rc, mid) = client.publish("iot_class_icp4/recv", "obstacle", qos=1)
    
