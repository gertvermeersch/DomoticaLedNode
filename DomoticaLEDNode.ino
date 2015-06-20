#include <SoftwareSerial.h>
#include "ESP8266.h"

#define SSID        "Dovy Keukens"
#define PASSWORD    "S3cur1ty"
#define ESP8266_USE_SOFTWARE_SERIAL
#define RED          10
#define BLUE         5
#define GREEN        6

SoftwareSerial wifiSerial(3,2);
ESP8266 wifi(wifiSerial);


void setup() {
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  //start the normal serial port 
  Serial.begin(115200);
  Serial.println("Arduino started");
  Serial.println("self test");
  testLED();
  Serial.println("Starting ESP8266...");
  
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());
  while(!startWifiServer());
}

void loop() {
  String strBuffer;
  uint8_t buffer[128] = {0};
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    if (len > 0) {
        
        strBuffer = String((char*)buffer);
        Serial.print("]\r\n");
        
        parseBuffer(mux_id, strBuffer);
       
    }

}

void testLED() {
  analogWrite(RED, 0);
  analogWrite(BLUE, 0);
  analogWrite(GREEN, 0);
  for(int i = 0; i<255; i++) {
    analogWrite(RED, i);
    delay(50);
  }  
  analogWrite(RED, 0);
  for(int i = 0; i<255; i++) {
    analogWrite(BLUE, i);
    delay(50);
  } 
  analogWrite(BLUE, 0);
  for(int i = 0; i<255; i++) {
    analogWrite(GREEN, i);
    delay(50);
  } 
  analogWrite(GREEN, 0);
  for(int i = 0; i<255; i++) {
    analogWrite(RED, i);
    analogWrite(GREEN, i);
    analogWrite(BLUE, i);
    delay(50);
  } 
  analogWrite(RED, 0);
  analogWrite(BLUE, 0);
  analogWrite(GREEN, 0);
}

bool parseBuffer(uint8_t mux_id, String strBuffer) {
  String msg;
  
  Serial.println("Received: ");
  Serial.println(strBuffer);
  if(strBuffer.indexOf("hello") > -1) {
     sendString(mux_id, String("Hi there, I'm a LED controller\r"));
  }  
  if(strBuffer.indexOf("exit") > -1) {
     sendString(mux_id, String("Bye!"));    
     wifi.releaseTCP(mux_id); 
  }  
  if(strBuffer.indexOf("Set") > -1) {
    
  }
  
}

void sendString(uint8_t mux_id, String msg) {
    char txtbuffer[128];
    msg.toCharArray(txtbuffer, msg.length());
    wifi.send(mux_id, (uint8_t*)txtbuffer, (uint32_t)msg.length()+1);
}

bool startWifiServer() {
  bool result = true;
  if (wifi.setOprToStation()) {
        Serial.print("Chip was set to station mode\r\n");
        Serial.println("Connecting to hard coded Wifi");
        if (wifi.joinAP(SSID, PASSWORD)) {
              Serial.print("Join AP - success\r\n");
              Serial.print("IP: ");       
              Serial.println(wifi.getLocalIP().c_str());
              if (wifi.enableMUX()) {
                Serial.print("multiple connections ok\r\n");
                if (wifi.startTCPServer(23)) {
                  Serial.print("start telnet tcp server ok\r\n");
                } else {
                  Serial.print("start telnet tcp server error\r\n");
                  result = false;
                }
              } else {
                Serial.print("multiple err\r\n");
                result = false;
              } 
        } else {
           Serial.println("failed to join AP"); 
           result = false;
        }  
  } else {
        Serial.print("Chip was not set to station mode - malfunction\r\n");
        result = false;
  }
    
  
  return result;
  
}
