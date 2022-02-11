 #include "DHT.h"
//Definimos en que pin se recibira la informacion
 #define DHTPIN 12   
 #define HUMPIN 13 
//definimos el tipo de sensor DTH usaremos
 #define DHTTYPE DHT11   

//llamamos la funcion dth y mandamos el tipo de sensor y el pin
 DHT dht(DHTPIN, DHTTYPE);
//en la funcion setup asigaremos la frecuencia con la que se trabajara
// y se llevara acabo una prueba de coneccion del sensor
  void setup() {
    Serial.begin(115200);
    Serial.println(F("Prueba de coenxion"));
    dht.begin();
  }
//en la funcion loop determinaremos que haremos con la lectura de informacion
    void loop() {
//con delay determinaremos cada cunto tiempo se ejecutara
    delay(5000);
//Recibiremos las señales del sensor
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    int humedadT = analogRead(HUMPIN);
//En caso de no recibir alguna de las señales
//se notificara que no hay coneccion
  if ( isnan(t) || isnan(f)) {
    Serial.println(F("No hay conexion"));
    return;
  }
//Mostraremos los niveles de la temperatura
 
  Serial.print(F("\nTemperatura en °C "));
  Serial.print(t);
  Serial.print("  Humedad Ambiental: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("\nHumedad de tierra: ");
  Serial.println(humedadT);
  //Dependiendo del nivel de humedad en la tierra mostrara un mensaje informativo
  if(humedadT >= 0 & humedadT <= 300){
        Serial.println("Exceso de Agua");
     } else if(humedadT > 301 & humedadT <= 700){
        Serial.println("Suelo húmedo");
    }else if(humedadT >= 701){
        Serial.println("Suelo seco"); 
    }
  
}