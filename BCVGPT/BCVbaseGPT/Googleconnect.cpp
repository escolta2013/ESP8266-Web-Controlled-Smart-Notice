//googleconnect.cpp
#include "googleconnect.h"
#include <WiFiClientSecure.h>

const char* cellAddress1 = "E2";

bool enviarData() {

  if (!client) {
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");    
  }

  if (!client->connected()) {
    client->connect(host, httpsPort);    
  }
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");    
  }

  if (!client->connected()) {
    client->connect(host, httpsPort);    
  }

  String payload = payload_base + "\"" + String(random(10000)) + "," +     
    String(random(10000)) + "," + BCVdolar + "\"}";

  if (client->POST(url, host, payload)) {  
    return true;   
  } else {    
    Serial.println("Error mientras se conectaba");
    return false;
  }      
}

void obtenerTasa() {

  if (!client->connected()) {
    client->connect(host, httpsPort);
  }

  if (client->GET(url1, host)) {
    String payload = client->getResponseBody();     
    float dolar = payload.toFloat();
    BCVdolar = String(dolar, 2); 
  } else {
    Serial.println("Error mientras se conectaba");
  }
}