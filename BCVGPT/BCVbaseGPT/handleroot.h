//handleroot.h
#include "lib_wifi.h"
extern ESP8266WebServer server;

const char* HTML_FORM = "<html>\n"  
                        "<body>\n"
                        "<h2>Actualizar el precio de dólar</h2> \n"
                        "<form>\n"
                        "Ingresa el nuevo valor del dólar:\n"     
                        "<input name=\"newDolar\" type=\"text\">\n" 
                        "<input type=\"submit\" value=\"Actualizar\">\n" 
                        "</form>\n"
                        "</body>\n"  
                        "</html>";

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
  String html = "<html>\n"  
                "<body>\n"
                "<h2>Actualizar el precio de dólar</h2> \n"
                "<form>\n"
                "Ingresa el nuevo valor del dólar:\n"     
                "<input name=\"newDolar\" type=\"text\">\n" 
                "<input type=\"submit\" value=\"Actualizar\">\n" 
                "</form>\n"
                "</body>\n"  
                "</html>";
  
  HTML_FORM = html;
}