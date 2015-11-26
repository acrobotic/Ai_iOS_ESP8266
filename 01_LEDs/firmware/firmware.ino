#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
const char* ssid = "BEARBEAR";
const char* password = "isachinesebear";
MDNSResponder mdns;

ESP8266WebServer server(80);

const int led1_pin = D1;
const int led2_pin = D2;

void handleRoot() {
  for (uint8_t i=0; i<server.args(); i++){
   Serial.println(server.arg(i));
  }  
  server.send(200, "text/plain", "Hello from esp8266! Usage: navigate to /setleds?led1=XX&led2=YY changing XX/YY to ON or OFF.");
}

void handleLeds() {
  String led1_status = server.arg("led1");
  String led2_status = server.arg("led2");
  
  if(led1_status == "ON")
    digitalWrite(led1_pin, HIGH);
  else if(led1_status == "OFF")  
    digitalWrite(led1_pin, LOW);
  if(led2_status == "ON")
    digitalWrite(led2_pin, HIGH);
  else if(led2_status == "OFF")  
    digitalWrite(led2_pin, LOW);
    
  server.send(200, "text/plain", "LEDs' status changed! ("+String(millis())+")");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void){
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  digitalWrite(led1_pin, 0);  
  digitalWrite(led2_pin, 0);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);
  server.on("/setleds", handleLeds);

  
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
}
