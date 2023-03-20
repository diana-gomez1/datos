//#include <Arduino
#include "libWifi.h"
#include "Wifi.h"


 void conectarWifi(const char * ssid, const char * password){

     WiFi.begin(ssid, password);

 Serial.println("Estableciendo vinculo con el AP.");
 while (WiFi.status() != WL_CONNECTED)
 {
  Serial.print(".");
  delay(1000);
 }
 Serial.println("\r\n Conexion establecida");
 Serial.print("Numero Ip asignado:");
 Serial.println(WiFi.localIP());//imprime numero ip 
}