/*-----------------------------------------------------------------------
  ACROBOTIC - 11/12/2015
  Author: MakerBro
  Platforms: ESP8266 Development Board (ESP-12E)
  File: firmware.ino
  ------------------------------------------------------------------------
  Description:
  This program connects the ESP9266 module to the web, and starts a simple
  webserver that listens for specific arguments passed by a client to
  control the status of LEDs.
  ------------------------------------------------------------------------
  Please consider buying products from Acrobotic to help fund future
  Open-Source projects like this! We’ll always put our best effort in every
  project, and release all our design files and code for you to use.

  https://acrobotic.com/
  ------------------------------------------------------------------------
  License:
  Released under the Creative Commons Attribution, Share-Alike license.
  Please check LICENSE.txt for more information. All text above must be
  included in any redistribution.
------------------------------------------------------------------------*/
// Include the libraries that we'll be using throughout the code
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Define the ID and password of your Wi-Fi network
const char* ssid = "Network ID";
const char* password = "Password";

// Instantiate the ESP8266WebServer class, passing the argument 80 for the
// port that the server will be listening.
ESP8266WebServer server(80);

// Define the variables with the pin values where the LEDs are connected
const int led1_pin = D1;
const int led2_pin = D2;

// User-defined function that will be called when a client accesses the root
// directory path of the ESP8266 host
void handleRoot() {
  // Simply sends an 'OK' (200) response to the client, and a plain text
  // string with usage.
  server.send(200, "text/plain", String("Hello from esp8266! Usage: navigate to") +
              String(" /setleds?led1=XX&led2=YY changing XX/YY to ON or OFF."));
}

// User-defined function that will be called when a client accesses the /setleds
// path of the ESP8266 host
void handleSetLeds() {
  // We can read the desired status of the LEDs from the expected parameters that
  // should be passed in the URL.  We expect two parameters "led1" and "led2".
  String led1_status = server.arg("led1");
  String led2_status = server.arg("led2");
  
  // Check if the URL include a change of the LED status
  bool url_check = false;
  if((led1_status == "ON")||(led1_status == "OFF")||(led2_status == "ON")||(led2_status == "OFF"))
    url_check = true;

  // It's not required to pass them both, so we check that they're exactly equal to
  // the strings ON or OFF by our design choice (this can be changed if you prefer
  // a different behavior)
  if (led1_status == "ON")
    digitalWrite(led1_pin, HIGH);
  else if (led1_status == "OFF")
    digitalWrite(led1_pin, LOW);
  if (led2_status == "ON")
    digitalWrite(led2_pin, HIGH);
  else if (led2_status == "OFF")
    digitalWrite(led2_pin, LOW);
  if (url_check)
    // If we've set the LEDs to the requested status, we have the webserver
    // return an "OK" (200) response.  We also include the number of milliseconds
    // since the program started running.
    // Note: This number will overflow (go back to zero), after approximately 50 days.
    server.send(200, "text/plain", "LEDs' status changed! (" + String(millis()) + ")");
  else
    server.send(200, "text/plain", "LEDs' status unchanged! (" + String(millis()) + ")");
}

// If the client requests any other URL than the root directory or the /setled path:
void handleNotFound() {
  // We construct a message to be returned to the client
  String message = "File Not Found\n\n";
  // which includes what URI was requested
  message += "URI: ";
  message += server.uri();
  // what method was used
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  // and what parameters were passed
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  // the response, as expected, is a "Not Found" (404) error
  server.send(404, "text/plain", message);
}

// In the setup function we initialize the different things
// that will be needed in our program, as well as set up the hardware
void setup(void) {
  // Set the LED pins to act as digital outputs and set them to a LOW 
  // state initially.
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  digitalWrite(led1_pin, LOW);
  digitalWrite(led2_pin, LOW);

  // Start the Serial communication for debugging purposes
  Serial.begin(115200);
  //  Initialize the WiFi client and try to connect to our Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for a successful connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // For debugging purposes print the network ID and the assigned IP address
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Associate the URLs with the functions that will be handling the requests
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setleds", HTTP_GET, handleSetLeds);
  server.onNotFound(handleNotFound);

  // Start running the webserver
  server.begin();
  Serial.println("HTTP server started");
}

// The loop function is straight-forward, simply handle any incoming requests to the
// our ESP8266 host!
void loop(void) {
  server.handleClient();
}
