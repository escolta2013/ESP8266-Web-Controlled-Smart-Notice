
//web.h
#include "lib_wifi.h"
#include "NTPClient.h"
#include "handleroot.h"
#include <ESP8266WebServer.h>

//extern ESP8266WebServer server;
ESP8266WebServer server(80);




void handleRoot() {
  if (server.method() == HTTP_GET) {  
    server.send(200, "text/html", HTML_FORM);   
  }
  
  if (server.method() == HTTP_POST) {  
    String newDolar = server.arg("newDolar");  
    
    if (!newDolar.toFloat()) {
      server.send(400, "Bad Request");
      return;
    }
    
    BCVdolar = newDolar;  
    
    if (enviarData() != true) {
      server.send(500, "Error actualizando Sheets");
      return;   
    }
    
    server.send(200, "text/plain", "OK");   
  }  
}

void loadHTML() {
  HTML_FORM = handleroot::HTML_FORM;
}
