#include <NRF905.h>
#include <domotica.h>
#include <SPI.h>

Domotica controller;
char msg[32];

void setup() {
  Serial.begin(9600);
  controller = Domotica();
  controller.setDebug(true);
  controller.init(2);  
  msg[0] = 1;
}

void loop() {
  char* result;
  Serial.println("waiting for msg");
  while(!controller.checkNewMsg()) {
    //delay(10);
  }
  Serial.println("msg received");
  result = controller.getMsg();
  for(int i = 0; i < BUF_LEN; i++) 
    Serial.print(result[i]);
  Serial.print("\n\r");
  for(int i = 0; i < BUF_LEN; i++) 
    Serial.print(result[i],HEX);
  Serial.print("\n\r"); 
    
  
  //controller.sendToNode(1,msg);
  
}
