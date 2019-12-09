import speech_recognition as sr
r = sr.Recognizer()
keywords = ['disgusting','awesome','hello']
file_in=sr.AudioFile('test.wav')
with file_in as source:
    audio = r.record(source)
try:
    s = r.recognize_google(audio)
    print("Text: "+s)
    count = [s.count('disgusting'),s.count('awesome'),s.count('hello'),]
    for i,j in zip(count,keywords):
        print j,':',i
except Exception as e:
    print("Exception: "+str(e))


#hello everyone, this file is test file to count
    #keywords like hello disgusting and awesome, isnt it awesome
