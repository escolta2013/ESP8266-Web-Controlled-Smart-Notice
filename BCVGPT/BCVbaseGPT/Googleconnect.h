//googleconnect.h
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

const int httpsPort = 443;
const char* host = "script.google.com";  
const char* fingerprint = "";
const char* GScriptId = "AKfycbyoxK_VFQQ8rEawt0ALtMEibf7AvcuoENOt7eOzwvr6Vxse9ipHu1Wx2aJBXfrVfrN1dw";   
const char* GScriptId1 = "AKfycby8V1BkddaL30LjlpuSObsaWD-MGOF2zJZ_5ypPwWJzRt4PBakqQtLmmnZZDcY3Zp07GA";   

extern const char* cellAddress1;  
String BCVdolar = "";

bool enviarData();

HTTPSRedirect* client = nullptr;

String url = String("/macros/s/") + GScriptId + "/exec";
String url1 = String("/macros/s/") + GScriptId1 + "/exec?read=" + cellAddress1;   

String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"data\", \"values\": ";

bool enviarData();

void obtenerTasa();