import socket, traceback,tweepy
from time import sleep

consumer_key ="kVrZPunNty9GVvTxNxNoWdIZ0"
consumer_secret ="e7gW91gpPXmsP7oYj7J5D49u3Z9eVoMN7ALPbicrI5T4CTXw9h"
access_token ="111261196-dSuyVaRryrNh8t84gOaGjXMt0htTAHo5tmrZillu"
access_token_secret ="ym9De0yMCv24uiwypXHpipRyZeRQ0z1zzGtCt3xPudljz"
auth = tweepy.OAuthHandler(consumer_key, consumer_secret)   
auth.set_access_token(access_token, access_token_secret) 
api = tweepy.API(auth) 
  

def getUdp(port):
    host = ''
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) 
    s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) 
    s.bind((host, port)) 
    try:
        message, address = s.recvfrom(8192)           
        return message
    except (KeyboardInterrupt, SystemExit): 
        raise 
    except: 
        traceback.print_exc()
def sendUdp(ip,port,msg):
    UDP_IP = ip
    UDP_PORT = port
    MESSAGE = msg
    print ("UDP target IP:", ip)
    print ("UDP target port:", port)
    print ("message:", msg)
    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
    sock.sendto(msg, (ip, port))
#camera = picamera.PiCamera()
#camera.resolution = (800, 600)
while (1):
    try:
        ip = '';port = 5005;BUFFER_SIZE = 20;s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);s.bind((ip, port));s.listen(3)
        conn, addr = s.accept();
        print 'Connection address:', addr
        inData = conn.recv(BUFFER_SIZE)
        print ("Data Recevied from Raspberry Pi: ",inData)
        if ("Tweet" in inData) or ("tweet" in inData):
            try:
                api.update_status(status =inData.replace("Tweet:","")) 
                print ("Tweet Successful")
                conn.send("Tweet success")
            except:
                print ("Error: not posted")
                conn.send("Error: not posted")

        # elif ("Img" in inData) or ("img" in inData):
            # camera.capture('snapshot.jpg')
            # tweet ="Tweet from RaspberryPi Camera" 
            # image_path ="snapshot.jpg" 
            # status = api.update_with_media(image_path, tweet)  
            # api.update_status(status = tweet)
            # print ("Image tweeted Successfully")

    except:
        print ("Some error occured.. program continue")

sendUdp("192.168.43.203",5555,"helloTest")
# twitter link https://twitter.com/Zeal18052398
