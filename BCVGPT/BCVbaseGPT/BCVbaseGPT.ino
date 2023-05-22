//based.ino
#include "setting.h"
#include "handleroot.h"
#include "web.h"
#include "googleconnect.cpp"
//#include <ESP8266WiFi.h>
//#include <WiFiManager.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include "lib_wifi.h"
#include "googleconnect.h"
#include "string.h"


void setup()
{
  conectarWiFi(); // de la liberia lib_wifi.h
  timeClient.begin();
  timeClient.setTimeOffset(-14400); // Set offset time in seconds, GMT-4 = 14400

  server.on("/", handleRoot);
  server.begin();


  Serial.begin();
  DotMatrix.begin(1);  // 0 cuando no se quiere zona
  DotMatrix.setZone(0, 0, 3); // comentar cuando no zonas
  DotMatrix.setIntensity(0);
  DotMatrix.setZoneEffect(0, true, PA_FLIP_LR);  // comentar cuando no zonas para invertir pantalla
  DotMatrix.displayText("Tipo de Cambio Referencial: ", scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  displayMode = DATE;

// Actualiza hora inicial de NTP
  timeClient.update();
  timeLocal = timeClient.getEpochTime();

  setupMeshing();

  setupWeb();
  
  

  loggingtasa();
  enviarData();

  // create the alarms, to trigger functions at specific times
  Alarm.timerOnce(5, loggingtasa);        //carga el precio del dolar al arrancar 5 seg
  Alarm.timerRepeat(0,2,0, loggingtasa);     // se reporta a la sheet logging cada 2 min
  Alarm.timerRepeat(0,15,0, enviardata);     // se conecta con la sheet tasa cada 15 min

  cellAddres1 = BCVdolar.c_str();
}

void loop(){
  
  // Actualizar hora de NTP 
  timeClient.update();

  server.handleClient();  

  // Intentar obtener valor del dólar de Google Sheets
  if(loggingtasa()){
    // Mostrar valor actualizado del dólar    
    showDollar();
  }
  
  else {
   //Conexión fallida, mostrar IP de red
    
   DotMatrix.print("ERR");
   DotMatrix.print(WiFi.softAPIP());
   DotMatrix.write();   // Muestra IP
   
   // Mostrar último valor del dólar conocido
   showDollar();  
  }
}

void loggingtasa(){
  // Intentar obtener valor del dólar de Google Sheets  
  ...
  
  if(client->GET(url1, host)){
    return true; 
  }  
  return false;
}

void obtainTime()
{
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  if (displayMode == DATE && DotMatrix.displayAnimate()) {
    DotMatrix.displayReset();
    displayMode = DOLAR;
  }

  currentMillis = millis();
  if (currentMillis - previousMillis > interval &&
      displayMode == DOLAR) {
    previousMillis = millis();

    // The formattedDate comes with the following format:
    // 2018-05-28T16:00:13Z
    formattedDate = timeClient.getFormattedDate();

    // Extract date
    DiaSemana = daysOfTheWeek[timeClient.getDay()];
    year = formattedDate.substring(0, 4);
    month = formattedDate.substring(5, 7);
    month = monthArray[month.toInt() - 1];
    date = formattedDate.substring(8, 10);
    date = String(date.toInt());
    dateStamp = DiaSemana + ", " + date + " de" + month + "del " + year + " || " + timeStamp + " || ";
    dateStamp.toCharArray(dateBuffer, dateStamp.length()+1);
    
    // Extract time
    hour = formattedDate.substring(11, 13);
    minute = formattedDate.substring(14, 16);
    second = formattedDate.substring(17, 19);

    if (hour.toInt() == 0) {
      hour = String(hour.toInt() + 12);
    }
    else if (hour.toInt() < 13) {
      hour = String(hour.toInt());
    }
    else {
      hour = String(hour.toInt() - 12);
    }

    if (second.toInt() == 0) {
      displayMode = DATE;
      DotMatrix.displayClear();
     
      DotMatrix.displayText(dateBuffer, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
      return;
    }
    
    timeStamp = hour + ":" + minute;
    
    DotMatrix.setTextAlignment(PA_CENTER);
    DotMatrix.print(BCVdolar);  //muestra precio dolar
  }  
}

void webActualizarDolar(){
  
  // Capturar el valor del dólar enviado por la página web
  String newDolarValue = ""; // Capturar del request de la página web
  
  // Actualizar timeLocal con la nueva hora 
  timeLocal = millis(); 
 
  // Actualizar BCVdolar con el nuevo valor  
  BCVdolar = newDolarValue;

  // Actualizar pantalla 
  DotMatrix.print(BCVdolar);
  DotMatrix.write();

  // Actualizar nodos de la red mesh  
  actualizarMesh();  
} {
  // Captura el nuevo valor del dólar  
  // Actualiza timeLocal y BCVdolar    
}

// Implementación de las funciones definidas en settings.h
void actualizarMesh(){
  
  // Verificar si hay cambios que necesiten propagarse  
  if( BCVdolar != BCVdolarPrevio ){
    
    // Actualizar valor previo para comparación futura    
    BCVdolarPrevio = BCVdolar;
    
    // Enviar actualización a través de la red mesh 
    mesh.sendMeshData(BCVdolar);  
  }
  
  // Verificar si se recibieron actualizaciones de otros nodos
  while(mesh.available()){
    
    // Leer nuevo valor de dólar de la red mesh
    String newDolarMesh = mesh.readMeshData();
    
    // Almacenar el valor
    BCVdolar = newDolarMesh;
    
    // Actualizar pantalla 
    DotMatrix.print(BCVdolar);
    DotMatrix.write();     
  }
}

// Funciones para mostrar fecha y precio en la matriz
void showDate() {
  
  // Obtener fecha actualizada de NTP
  timeClient.update(); 
  formattedDate = timeClient.getFormattedDate();
  
  // Dividir fecha en partes  
  int split = formattedDate.indexOf(" ");
  year = formattedDate.substring(0, split); 
  month = formattedDate.substring(split + 1, split + 4); 
  date = formattedDate.substring(split + 5);
  
  // Obtener día de la semana 
  int dayNumber = timeClient.getDay();  
  DiaSemana = daysOfTheWeek[dayNumber];
  
  DotMatrix.print(DiaSemana);
  DotMatrix.print(monthArray[month.toInt() - 1]);  
  DotMatrix.print(date);    
  DotMatrix.print(year);
  
  DotMatrix.write();   // Renderizar
}


void showDollar(){
  
  // Obtener hora actualizada de NTP 
  timeClient.update();
  
  // Mostrar valor del dólar 
  DotMatrix.print(BCVdolar);

  //Mostrar hora local  
  DotMatrix.print(hour(timeLocal));  
  DotMatrix.print(":");  
  DotMatrix.print(minute(timeLocal));  
  DotMatrix.print(":"); 
  DotMatrix.print(second(timeLocal));  
  
  DotMatrix.write();  // Renderizar
}
  