#include <NRF905.h>
#include <domotica.h>
#include <SPI.h>

Domotica controller;
char msg[32];
const int switchport = A0;
boolean on = false;
char* result;

void setup() {
  Serial.begin(9600);
  controller = Domotica();
  controller.setDebug(true);
  controller.init(2);  
  msg[0] = 1;
}

void loop() {
  
  Serial.println("waiting for msg");
  while(!controller.checkNewMsg()) {
    delay(100); //have some damn' patience
  }
  Serial.println("msg received");
  result = controller.getMsg();
  for(int i = 0; i < BUF_LEN; i++) 
    Serial.print(result[i]);
  Serial.print("\n\r");
  for(int i = 0; i < BUF_LEN; i++) 
    Serial.print(result[i],HEX);
  Serial.print("\n\r"); 
  command();
    
  
  //controller.sendToNode(1,msg);
  
}

void command() {
  if(result[4] == 's') {
    on = !on;
    digitalWrite(switchport, on);
  }
  
}
