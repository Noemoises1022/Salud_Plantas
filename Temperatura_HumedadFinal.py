import time
from datetime import datetime
import RPi.GPIO as GPIO
import time
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn


status_Hum = ''

i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
ads.gain = 1
chan0 = AnalogIn(ads, ADS.P0)
chan1 = AnalogIn(ads, ADS.P1)
chan_Hum = 23
GPIO.setmode(GPIO.BCM)
GPIO.setup(chan_Hum,GPIO.IN)

f = open ('humedadT.txt','w')
f.close()
f2 = open ('UV.txt','w')
f2.close()

def medirHumedad(chan_Hum):
    global status
    if GPIO.input(chan_Hum):
        status_Hum= 'seco,'
        print("seco,")
    else:
        status_Hum = 'Humedo,'
        print("humedo,")

while True:
  now = datetime.now()
  medirHumedad(chan_Hum)
  vol = str(chan0.voltage)
  f = open ('humedadT.txt','a')
  f2 = open ('UV.txt','a')
  date_time = now.strftime("%m/%d/%Y,%H:%M:%S\n")
  print(date_time)
  f.write(status_Hum)
  f.write(date_time)
  f2.write(vol)
  print(vol)
  f2.write(",")
  f2.write(date_time)
  f.close()
  f2.close()
  time.sleep(5)
