import RPi.GPIO as GPIO
import time



channel = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(channel,GPIO.IN)

def medirHumedad(channel):
	
	if GPIO.input(channel):
		print("seco")
	else:
		print("humedo")


while True:
	medirHumedad(channel)
	time.sleep(1)

