#include <mem.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <OSCMessage.h>
//#include <OSCBundle.h>
long sendCount = 0;
const char* ssid     = “____”;
const char* password = “____”;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(192, 168, 0, 13);
const unsigned int outPort = 5000;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {
    sendCount ++;
  if (sendCount > 100000)
  {
  OSCMessage msg("/test/");
  msg.add("salut c'est BEV");
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
  sendCount = 0;
  }
}

