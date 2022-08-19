#include "internet.hpp"
//wifi
#include <WiFi.h>
//WiFiClient client;
const char *ssid = "Redmi9T"; // Enter your WiFi name
const char *password = "1234567890"; // Enter WiFi password
void connect_wifi(){
    // Connect or reconnect to WiFi
if(WiFi.status() != WL_CONNECTED){
Serial.print("Attempting to connect to SSID: ");
Serial.println(ssid);
while(WiFi.status() != WL_CONNECTED){
WiFi.begin(ssid, password); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
Serial.print(".");
delay(5000);
}
Serial.println("\nConnected.");
}
}