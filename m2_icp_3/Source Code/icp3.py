import socket, traceback,tweepy#,picamera
from time import sleep

consumer_key ="QkWOL7X4Ni1v41sE62recwUsm"
consumer_secret ="6kdmEInXFtHebTudOy9MxaOEFfTTctmsQ8nD3PvkctRy9OaNKb"
access_token ="1010252295441825792-en4dt7Qgy3JCl38L4JglKp6yRDGMHp"
access_token_secret ="8ASK_FOR _IT"
auth = tweepy.OAuthHandler(consumer_key, consumer_secret)   
auth.set_access_token(access_token, access_token_secret) 
api = tweepy.API(auth) 

def init_server_Tcp(port):
    TCP_IP = ''
    TCP_PORT = port
    BUFFER_SIZE = 20  # Normally 1024, but we want fast response
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)
    conn, addr = s.accept()
    print 'Connection address:', addr
    while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        #conn.send(data)  # echo
##    return data
##    conn.close()


#camera = picamera.PiCamera()
#camera.resolution = (800, 600)
while (1):
    try:
        TCP_IP = ''
        TCP_PORT = port
        BUFFER_SIZE = 20  # Normally 1024, but we want fast response
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((TCP_IP, TCP_PORT))
        s.listen(1)
        conn, addr = s.accept()
        print 'Connection address:', addr
        while 1:
            data = conn.recv(BUFFER_SIZE)
            if not data: break

        inData = data
        print ("Data Recevied from Raspberry Pi: ",inData)
        if ("Tweet" in inData) or ("tweet" in inData):
            try:
                api.update_status(status =inData.replace("Tweet:","")) 
                print ("Tweet Successful")
                conn.send("Tweet Successful")
            except:
                print "Error: not posted"
                conn.send("Error: not posted")

        elif ("Img" in inData) or ("img" in inData):
            camera.capture('snapshot.jpg')
            tweet ="Tweet from RaspberryPi Camera" 
            image_path ="snapshot.jpg" 
            status = api.update_with_media(image_path, tweet)  
            api.update_status(status = tweet)
            print ("Image tweeted Successfully")
            conn.send("Image tweeted Successfully")
        conn.close()
    except:
        print ("Some error occured.. program continue")

# twitter link https://twitter.com/Zeal18052398
