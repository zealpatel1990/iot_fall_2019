import speech_recognition as sr 

mic_name = "USB Device 0x46d:0x825: Audio (hw:1, 0)"
sample_rate = 48000
chunk_size = 2048
r = sr.Recognizer() 
mic_list = sr.Microphone.list_microphone_names() 
device_id = 1
'''for i, microphone_name in enumerate(mic_list):
    print i, microphone_name
    if microphone_name == mic_name: 
        device_id = i 
'''
while (1):
    with sr.Microphone(device_index = device_id, sample_rate = sample_rate, chunk_size = chunk_size) as source: 
        #r.adjust_for_ambient_noise(source)
        print "Say Something"
        #listens for the user's input 
        audio = r.listen(source)
        try: 
            text = r.recognize_google(audio) 
            print "you said: " + text 
        #error occurs when google could not understand what was said 
        except sr.UnknownValueError: 
            print("Speech Recognition could not understand audio")    
        except sr.RequestError as e:
            print("Could not request results from Speech Recognition service; {0}".format(e))


