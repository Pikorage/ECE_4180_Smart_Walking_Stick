import RPi.GPIO as GPIO
import time
import pyttsx3

engine = pyttsx3.init()
GPIO.setmode(GPIO.BCM)

TRIG = 23
ECHO = 24

print ("Started TTS")
while 1:
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(TRIG,GPIO.OUT)
    GPIO.setup(ECHO,GPIO.IN)

    GPIO.output(TRIG, False)
    time.sleep(2)

    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    while GPIO.input(ECHO)==0:
        pulse_start = time.time()

    while GPIO.input(ECHO)==1:
        pulse_end = time.time()
    pulse_duration = pulse_end - pulse_start

    distance = pulse_duration * 17150

    distance = round(distance, 2)

    alert_str = "Distance is " + str(distance)
    print("Distance:",distance,"cm")        
    engine.say(alert_str)
    engine.runAndWait()
    time.sleep(2)
    GPIO.cleanup()