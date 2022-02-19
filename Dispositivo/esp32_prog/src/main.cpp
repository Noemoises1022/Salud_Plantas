/*
 *
 *************************************************************************************************************************************************************
 * ************************************** DETECCION DE TEMPERATURA, HUMEDAD, LUZ SOLAR Y CAPTURA DE FOTOS DE UNA PLANTA **************************************
 *************************************************************************************************************************************************************
 * 
 * 
 ***********************************
 ************* AUTORES *************
 ***********************************
 * -Noe Moises
 * -Ivan Baltazar
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
 * Este programa se encarga de tomar
 * 
 * 
 * 
 ************************************
 **** CONFIGURACION DEL HARDWARE ****
 ***********************************
 * 
 * 
 * 
 */


//**************************************
//*********** LIBRERIAS ****************
//**************************************

#include <Arduino.h> //Libreria para que platfomio trabaje con Arduino
#include <WiFi.h>  //Libreria para manejar el WiFi del ESP32CAM
#include <PubSubClient.h> //Libreria para manejar MQTT
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "soc/soc.h"           // Desabilitar problemas de caida de tension
#include "soc/rtc_cntl_reg.h"  // Desabilitar problemas de caida de tension
#include "driver/rtc_io.h"
#include <SPIFFS.h>  //libreria para SPIFFS
#include <FS.h>
#include <ESPAsyncWebServer.h> //Libreria para web server
#include <StringArray.h>
#include <DHT.h> //Libreria para el sensor de temepratura



//**************************************
//****** CONFIGURACION DE PINES ********
//**************************************

#define LED_STATUS 33 // Definido para marcar la conexion con el wi-fi
#define FLASH 4 // Usado para el flash y para indicar la conexion con MQTT


//**************************************
//***** CONFIG. PINES DE LA CAMARA *****
//**************************************

#define FOTO "/planta1.jpg" //nombre de la foto 
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22



//**************************************
//***** CONFIG. PINES DE SENSORES ******
//**************************************

//Definimos en que pin se recibira la informacion
 #define DHTPIN 12   
 #define HUMPIN 13 
//definimos el tipo de sensor DTH usaremos
 #define DHTTYPE DHT11  



//**************************************
//******* CONFIGURACION DEL WIFI *******
//**************************************

#define ssid "Cambio-tacos-por-wifi"  // Nombre de la red a conectar
#define password "ADELAMANDA-9305"  // Contraseña de la red



//**************************************
//******* CONFIGURACION DEL MQTT *******
//**************************************

#define mqtt_servidor "3.126.191.185"  //ip del broker a conectar
#define mqtt_puerto 1883 //Puerto al que se conectara
#define clientId "Planta01" //Id del cliente que se conectara
#define topic "SaludPlantas/Esp32/SistemaDeteccion/Planta01"  //Tema a suscribirse



//**************************************
//*********** OBJETOS ******************
//**************************************

WiFiClient espClient; //Maneja los datos de conexion WiFi
PubSubClient client(espClient); //Maneja los datos de conexion al broker
AsyncWebServer server(80); //Se crea el servidor web en el peurto 80
DHT dht(DHTPIN, DHTTYPE); //llamamos al objeto y mandamos el tipo de sensor y el pin


//**************************************
//******** VARIABLES GLOBALES **********
//**************************************

double timeLast, timeNow; // Variables para el control de tiempo no bloqueante
int wait = 5000;//tiempo de espera
bool primera = false; //variable que nos indica si es la primera vez que nos conectamos al broker
//const char* mqtt_servidorN = "127.0.0.1"; //Servidor que va cambiar con una señal del MQTT
String localip = ""; // variable apra guardar la direccion ip de la tarjeta
int tipo_envio= 0; // variable que nos indica el tipo de envio que se va realizar
//variable utilziadas apra guardar datos de los sensores
float h = 0.0;
float t = 0.0;
float f = 0.0;
int humedadT = 0; //humedad de la tierra


//************************
//****** FUNCIONES *******
//************************

void conectar_wifi();  //Funcion encargada de conectarse al WiFi
void enviar_datos_mqtt(); //Funcion que envia la suma y la cadena con el valor binario de la secuencia de leds
void conectar_broker();  //Funcion que se conecta al broker
void reconectar_broker(); //Funcion para reconectarse al broker
void mqtt_envio();//Funcion para enviar mensajes mqtt
void mqtt_llamada(char* topicRes, byte* message, unsigned int length); //Funcion apra recibir mensajes mqtt
bool verificar_foto( fs::FS &fs ); //Funcion para verificar la foto toamda
void tomar_foto(); //Funcion que toma una foto 
void enviar_foto(fs::FS &fs );//esta funcion envia la foto tomada
void leer_sensores(); //Funcion que toma los datos de los sensores conectados a la tarjeta






//**************************************************************************
//*********************** INICIALIZACION DEL PROGRAMA **********************
//**** ESTAS INSTRUCCIONES SE EJECUTAN UNA VEZ AL ENERGIZAR EL SISTEMA *****
//**************************************************************************


void setup() {
  
  Serial.begin (115200); // Iniciar comunicación serial
  pinMode (FLASH, OUTPUT); //Definimos el led como salida
  pinMode (LED_STATUS, OUTPUT); //Definimos el led como salida
  //apagamos los leds
  digitalWrite (FLASH, LOW); 
  digitalWrite (LED_STATUS, HIGH);  
  
  //************************
  //*** CONFIGURACION  E INICIO DEL WIFI Y MQTT
  //************************

  conectar_wifi(); //nos conectamos al wi-fi
  delay (1000); // Espera para iniciar el broker
  conectar_broker(); //Nos conectamos al broker

  
  //************************
  //*** CONFIGURACION E INICIO  DE LA CAMARA
  //************************
  
  //iniciar el SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Un error ocurrio al montar el SPIFFS (҂◡_◡)");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS montado exitosamente (◠﹏◠)");
  }
  
  // Apagar el 'detector de caídas de tensión'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Modulo de la camara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Iniciar camara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error de camara 0x%x", err);
    ESP.restart();
  }

  //respuesta del servidor
    server.on("/enviar-foto", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FOTO, FOTO, false);
  });

  // Iniciamos el servidor
  server.begin();


  //iniciamos el sensor DTH
  Serial.println(F("Prueba de coenxion"));
  dht.begin();
  
  timeLast = millis (); // Inicia el control de tiempo

}//Fin setup()




//**************************************************************************
//*********************** CUERPO DEL PROGRAMA ******************************
//************************* BUCLE PRINCIPAL *********************************
//***************************************************************************

void loop(){
  //Verificar siempre que haya conexión al broker
  if (!client.connected()) {
    reconectar_broker();  // En caso de que no haya conexión, ejecutar la función de reconexión
  }
  client.loop(); // Ejecuta las funciones necesarias para la comunicación con el broker

}//Fin loop()





//**************************************************************************
//*********************** FUNCIONES DEL PROGRAMA ***************************
//**************************************************************************
//**************************************************************************



//************************
//**** CONECTAR WI-FI ****
//************************

void conectar_wifi(){
  
  //Mostramos a que red nos estamos conectando
  Serial.print("\n\nConectar a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password); // Función que realiza la conexión a WiFi
 
  while (WiFi.status() != WL_CONNECTED) { // Este bucle espera a que se realice la conexión
    digitalWrite (LED_STATUS, LOW); //Se enciende el led de status
    delay(1000); //Espera a que se realice la conexion
    digitalWrite (LED_STATUS, HIGH); //Se apaga el led de status
    Serial.print(" (´･_･`) ");  // Indicador de progreso
    delay (5);
  }
  
  // Cuando se haya logrado la conexión, el programa avanzará, por lo tanto, puede informarse lo siguiente
  Serial.println("\n\nWiFi conectado ♥‿♥");
  Serial.println("Direccion IP: ");
  localip+=WiFi.localIP().toString();
  Serial.println(localip);// imprime al direccion IP de la tajeta

  // Si se logro la conexión, encender led
  if (WiFi.status () > 0){
  digitalWrite (LED_STATUS, LOW);//Apagamos el led
  }
  
}//Fin conectar_wifi()


//************************
//*** CONECTAR BROKER ****
//************************

void conectar_broker(){
  if (!primera){
    client.setServer(mqtt_servidor, mqtt_puerto); // Conectarse a la IP del broker en el puerto indicado
  }else{
    //IPAddress server(127,0,0,1);
    //client.setServer(mqtt_servidorN, mqtt_puerto); // Conectarse a la IP del broker en el puerto indicado
  }
  
  client.setCallback(mqtt_llamada); // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);  // Esta espera es preventiva, espera a la conexión para no perder información

}//Fin conectar_broker()


//************************
//*** RECONEXION MQTT ****
//************************

void reconectar_broker(){
    // Bucle hasta lograr conexión
  while (!client.connected()) { // Pregunta si hay conexión
    digitalWrite (FLASH, HIGH);//Encendemos el flash 
    Serial.print("Tratando de contectarse...");
    // Intentar reconexión
    if (client.connect(clientId)) { //Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe(topic); // Esta función realiza la suscripción al tema
    }// fin del  if (client.connect("ESP32CAMClient"))
    else {  //en caso de que la conexión no se logre
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state()); // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      
      delay(5000); // Espera de 5 segundos bloqueante
      Serial.println (client.connected ()); // Muestra estatus de conexión
    }
    digitalWrite (FLASH, LOW);//Apagamos el flash 
  }
}//Fin reconectar_broker()


//************************
//*** ENVIO MSN MQTT *****
//************************

void mqtt_envio(){
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  // Si hay un tipo de envio 1 se envia el mensaje para pedir la foto
  if(tipo_envio == 1){
      
      // se notiica que la foto ya fue tomada
      client.publish(topic,"{\"mensaje\":\"foto_tomada\"}");
      
      // se envia al solicitud HTTP para recueprar la foto
      String solicitud="{\"mensaje\":\"http\",\"servicio\":\"http://";
      solicitud+=localip;
      solicitud+="/enviar-foto\"}";
      client.publish(topic,solicitud.c_str());
  }else if(tipo_envio == 2){
      
      // se notiica que la los datos fueron tomados
      client.publish(topic,"{\"mensaje\":\"datos_leidos\"}");
      delay (5000);
      // se envian los datos
      String solicitud="{\"mensaje\":\"datos_sensores\",\"servicio\":{\"Temperatura\":\"";
      solicitud += t;
      solicitud += "\",\"Humedad\":\"";
      solicitud += h;
      solicitud += "\",\"Humedad_tierra\":\"";
      solicitud += humedadT;
      solicitud += "\"}}";
      client.publish(topic,solicitud.c_str());
  }
  tipo_envio = 0;

}//Fin mqtt_envio()


//************************
//** RECEPCION MSN MQTT **
//************************

void mqtt_llamada(char* topicRes, byte* message, unsigned int length){
// Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);

  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp; // Se declara la variable en la cual se generará el mensaje completo  
  for (int i = 0; i < length; i++) {  // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);

  // En esta parte puedes agregar las funciones que requieras para actuar segun lo necesites al recibir un mensaje MQTT

  // Ejemplo, en caso de recibir el mensaje true - false, se cambiará el estado del led soldado en la placa.
  // El ESP323CAM está suscrito al tema esp/output
  ////-----------------------------------------AQUI---------------------------------
  if (String(topic) == "SaludPlantas/Esp32/SistemaDeteccion/Planta01") {  // En caso de recibirse mensaje en el tema esp32/
    if(messageTemp == "toma_foto"){// si el mensaje solicita toamr una foto
      tomar_foto(); // tomamos la foto
      tipo_envio = 1;
    }else if(messageTemp == "leer_sensores"){ // Si el mensaje solicita leer los sensores
      leer_sensores(); //leemos los sensores
      tipo_envio = 2;
    }

    mqtt_envio(); //enviamos los datos necesarios 
  }
}//Fin mqtt_llamada


//************************
//**** VERIFICAR FOTO ****
//************************

bool verificar_foto( fs::FS &fs ) {
  File foto = fs.open( FOTO ); //Abrimos el archivo
  unsigned int foto_zic = foto.size(); //verificamos el tamaño del archivo
  return ( foto_zic > 100 );//verificamso que sea mayor a 100
}//Fin verificar_foto


//************************
//***** TOMAR FOTO *******
//************************

 //Funcion para verificar la foto toamda
void tomar_foto(){
    camera_fb_t * fb = NULL; // pointer
    bool ok = 0; // Boolean indicating if the picture has been taken correctly

    do {
      // Tomar una foto con la camara
      Serial.println("Tomando foto...");

      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Fallo captura de camara");
        return;
      }
      //Nombre de la foto
      Serial.printf("Nombre de la foto: %s\n",FOTO);
      File file = SPIFFS.open(FOTO, FILE_WRITE);

      // insertamos los datos en el archivo
      if (!file) {
        Serial.println("Error en la escritura de la imagen");
      }else {
        file.write(fb->buf, fb->len); // guardamos
        Serial.print("La foto fue guardada");
        Serial.print(FOTO);
        Serial.print(" Tamaño: ");
        Serial.print(file.size());
        Serial.println(" bytes");
      }
      // cerramos el archivo
      file.close();
      esp_camera_fb_return(fb);

      // checamos si la foto fue guardada correctamente
      ok = verificar_foto(SPIFFS);
    }while(!ok);  
  }//Fin tomar foto



//**********************************
//***** ENVIAR DATOS SENSORES ******
//**********************************

void leer_sensores(){
    Serial.println("leee datos");
  do {
    // Obtenemos los datos del sensor
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);
    humedadT = analogRead(HUMPIN);
  }while( isnan(t) || isnan(f));

}//Fin leer_sensores