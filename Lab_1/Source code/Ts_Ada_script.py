import urllib
import json
import time
from pprint import pprint

# import Adafruit IO REST client
from Adafruit_IO import Client

ADAFRUIT_IO_KEY = "0b71349489ea4d90b0414737d9aedbfe"
ADAFRUIT_IO_USERNAME = "preranasam"

# Create an instance of the REST client
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# set up Adafruit IO feeds
temperature = aio.feeds('temperature')
humidity = aio.feeds('humidity')

while True:
  TS = urllib.urlopen("https://api.thingspeak.com/channels/867331/feeds.json?api_key=J9IKAMZ3KK7G37KO&results=1")

  response = TS.read()
  data=json.loads(response)
  pprint(data)
  #b = data['channel']['field1']
  temperature_data = data['feeds'][0]['field1']
  #e = data['feeds'][1]['field1']
  print ("Temperature: " + temperature_data)
  aio.send(temperature.key, temperature_data)

  humidity_data = data['feeds'][0]['field2']
  print ("Humidity:" + humidity_data)
  aio.send(humidity.key, humidity_data)


  time.sleep(30)
  TS.close()

