import Adafruit_DHT  
import time  
import RPi.GPIO as GPIO
channel_DHT11 = 4
channel_Hum = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(channel_Hum,GPIO.IN)

def medirHumedad(channel_Hum):
	
	if GPIO.input(channel_Hum):
		print("seco")
	else:
		print("humedo")


while True:
  sensor = Adafruit_DHT.DHT11 
  
  humedad, temperatura = Adafruit_DHT.read_retry(sensor, channel_DHT11)
  print("\nTemperatura: {}*C   Humedad: {}% ".format(temperatura, humedad))
  medirHumedad(channel_Hum)
  time.sleep(1)
