/*
 *
 *************************************************************************************************************************************************************
 * ************************************** DETECCION DE HUMEDAD TIERRA y LUZ SOLAR DE UNA PLANTA **************************************
 *************************************************************************************************************************************************************
 * 
 * 
 ***********************************
 ************* AUTORES *************
 ***********************************
 * -Noe Moises Baca Reyes
 * -Ivan Baltazar Camacho
 * -Carlos Adolfo Gonzales Torres
 * 
 ***********************************
 ************* FECHA ***************
 ***********************************
 * 10 de enero del 2022
 * 
 * 
 ***********************************
 ************ DESCRIPCION **********
 ***********************************
 * Este programa se encarga de tomar la lectura de los sensores de humedad de tierra y UV 
 *por medio de la Rasperry
 * 
 * 
 


//**************************************
//*********** LIBRERIAS ****************
//**************************************
*/
import time
from datetime import datetime
import RPi.GPIO as GPIO
import time
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

//**************************************
//****** CONFIGURACION DE PINES Y VARIABLES ********
//**************************************

status_Hum = ''

i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
ads.gain = 1
chan0 = AnalogIn(ads, ADS.P0)
chan1 = AnalogIn(ads, ADS.P1)
chan_Hum = 23                  //PIN DEL HIGROMETRO
GPIO.setmode(GPIO.BCM)
GPIO.setup(chan_Hum,GPIO.IN)
//*************************************************************
//*********** ARCHIVOS QUE GURADARAN LOS DATOS ****************
//*************************************************************
f = open ('humedadT.txt','w') 
f.close()
f2 = open ('UV.txt','w')
f2.close()
//*****************************************************************************
//*********** FUNCION QUE DETERMINARA EL ESTADO DEL HIGROMETRO ****************
//*****************************************************************************
def medirHumedad(chan_Hum):
    global status_Hum
    if GPIO.input(chan_Hum):
        status_Hum= 'seco'
        print("seco")
    else:
        status_Hum = 'Humedo'
        print("humedo")
//***********************************************
//*********** LECTURA DE VALORES ****************
//***********************************************
while True:
  now = datetime.now()
  medirHumedad(chan_Hum)
  vol = str(chan0.voltage)
  f = open ('humedadT.txt','a')
  f2 = open ('UV.txt','a')
  date_time = now.strftime('%d/%m/%Y","Hora":"%H:%M:%S"')
  print(date_time)
//**********************************************************
//***********eSCRITURA DE VALORES EN ARCHIVO ****************
//**********************************************************
  cadena = '{"mensaje":"datos_sensores","servicio":{"Humedad_tierra":"'+status_Hum+'","Fecha":"'+date_time+'"}}\n'
  f.write(cadena)
  cadena2 = '{"mensaje":"datos_sensores","servicio":{"UV":"'+vol+'","Fecha":"'+date_time+'"}}\n'
  print(vol)
  f2.write(cadena2)
  f.close()
  f2.close()
  time.sleep(5)
