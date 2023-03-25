#include "DHT.h"

#define DHTPIN 4  //Pin de datos del sensor DHT11/22

#define DHTTYPE DHT11  //Especificamos el tipo de sensor DHT11 DHT22 DHT21
#include <Arduino.h>
#include "libwifi.h"
#include "Wifi.h"
DHT dht(DHTPIN, DHTTYPE);  //Creamos el objeto que representa al sensor
const char * ssid = "ASUSDCGG";
const char * password = "3164173706Cc";
const char * host = "dweet.io";
const int puerto = 80;
//String server="dweet.io"; es redundante con host 
//String url="/dweet.io/dweet/for/uceva02?temperatura=32.5";

void setup(){  
      Serial.begin(115200);
      Serial.println("Inicializando el dispositivo");
      pinMode(2,OUTPUT); //Coloco el pin 2 como salida
      conectarWifi(ssid, password);
      dht.begin();

}
/**
 * @brief Esta funcion con bucle infinito
 prueba
*/
void loop(){
   delay(2000);
   float humedad = dht.readHumidity(); //Leemos la humedad en % de humedad relativa (0 al 100%)
   float temperatura = dht.readTemperature(); //Leemos la temperatura en °C   
   //Verificamos las lecturas del sensor
   if(isnan(humedad) || isnan(temperatura)){
     Serial.println("Fallo la lectura del sensor");
     return;
   }   

   Serial.print(F("Humedad: ")); //La F() siginifica que lo que esta dentro del parentesis se escribe en la FLASH
   Serial.print(humedad);
   Serial.print(F("%  Temperatura: ")); //La F() siginifica que lo que esta dentro del parentesis se escribe en la FLASH
   Serial.print(temperatura);
   String url = "/dweet/for/uceva02?temperatura=" + String(temperatura) + "&humedad=" + String(humedad);
   WiFiClient cliente;//creamos cliente TCP por wifi 
  
  //BLOQUE que raliza la conexion al servidor
  if(!cliente.connect(host, puerto)){
    Serial.println("Error conexion al host fallida");
    delay(2000);
    return;
  } 
    //Peticion (request) GET al servidor HTTP
  cliente.print("GET " +url+" HTTP/1.1\r\nHost: "+String(host)+"\r\n"+"Connection: close\r\n\r\n");
  //Tiempo al servidor a que responda la peticion(response)
  //delay(5000);// no funciona aqui porque bloquea wifi 
  unsigned long milisegundos = millis();//hora de inicio
  while (cliente.available()== 0){//preguntamos si no hay datos recibidos disponibles
      if(millis()-milisegundos > 5000){//millis hora actual  menos el instante anterior
      Serial.println("Se expiro el tiempo de la conexion");
      cliente.stop();
      }
   
   }
  while (cliente.available()){
    String linea = cliente.readStringUntil('\r');//lea string hasta que encuentre lo que esta en comilla simple
    Serial.println(linea);
  }
    Serial.println("Fin de conexion");
    cliente.stop();
    
    delay(2000);//espero 2s
}

