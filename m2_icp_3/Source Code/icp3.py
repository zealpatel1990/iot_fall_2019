import socket, traceback,tweepy,picamera
from time import sleep

consumer_key ="QkWOL7X4Ni1v41sE62recwUsm"
consumer_secret ="6kdmEInXFtHebTudOy9MxaOEFfTTctmsQ8nD3PvkctRy9OaNKb"
access_token ="1010252295441825792-en4dt7Qgy3JCl38L4JglKp6yRDGMHp"
access_token_secret ="8ASK_FOR _IT"
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
    print "UDP target IP:", ip
    print "UDP target port:", port
    print "message:", msg
    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
    sock.sendto(msg, (ip, port))
camera = picamera.PiCamera()
camera.resolution = (800, 600)
while (1):
    try:
        inData = getUdp(5555)
        print ("Data Recevied from Raspberry Pi: ",inData)
        if ("Tweet" in inData) or ("tweet" in inData):
            try:
                api.update_status(status =inData.replace("Tweet:","")) 
                print ("Tweet Successful")
                sendUdp("192.168.43.203",5555,"Tweet Successful")
            except:
                print "Error: not posted"
                sendUdp("192.168.43.203",5555,"Error: not posted")

        elif ("Img" in inData) or ("img" in inData):
            camera.capture('snapshot.jpg')
            tweet ="Tweet from RaspberryPi Camera" 
            image_path ="snapshot.jpg" 
            status = api.update_with_media(image_path, tweet)  
            api.update_status(status = tweet)
            print ("Image tweeted Successfully")

    except:
        print ("Some error occured.. program continue")

sendUdp("192.168.43.203",5555,"helloTesgfchbjt msg")
# twitter link https://twitter.com/Zeal18052398
