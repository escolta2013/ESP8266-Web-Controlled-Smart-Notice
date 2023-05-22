//setting.h
#include <Arduino.h>
#include "HTTPSRedirect.h"
#include <ESP8266TimeAlarms.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266Ping.h>
#include <Ticker.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "lib_wifi.h"
#include "Googleconnect.h"
#include "handleroot.h"
#include "web.h"


// Comentario: Alarm ID para la alarma de actualización horaria
AlarmId id;

// Comentario: Variables de la matriz de puntos
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   D5
#define DATA_PIN  D7
#define CS_PIN    D8
MD_Parola DotMatrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

uint8_t scrollSpeed = 50; // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_RIGHT; //left cuando no se quiere invertido
textPosition_t scrollAlign = PA_RIGHT; //left cuando no se quiere invertido

// Variables para mostrar hora/fecha
uint16_t scrollPause = 1000; // in milliseconds
String formattedDate;

volatile unsigned long timeLocal;

// Comentario: Credenciales WiFi globales
String user;    
String password;

// Comentario: Credenciales de red mesh  
String meshID;
String meshPassword;

// Comentario: Función para establecer hora local   
void setLocalTime(void); 

// Comentario: Actualizar el valor del dólar desde la web   
void webActualizarDolar(void);

// Comentario: Actualizar la red mesh  
void actualizarMesh(void);

// Comentario: Parámetros para obtener y parsear el tiempo
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const IPAddress remote_ip(8, 8, 8, 8); //google ip
enum {
  DOLAR, 
  DATE
} displayMode;

boolean displayMode = DOLAR;
long currentMillis;
long previousMillis;
int interval = 1000;

String timeStamp, hour, minute, second;
String dateStamp, year, month, date, DiaSemana;
char dateBuffer[] = "";
char daysOfTheWeek[7][12] = {"dom", "lun", "mar", "mie", "jue", "vie", "sab"};
String monthArray[12] = {
  " Ene ", " Feb ", " Mar ", " Abr ", " May ", " Jun ",
  " Jul ", " Ago ", " Sep ", " Oct ", " Nov ", " Dic "
};







