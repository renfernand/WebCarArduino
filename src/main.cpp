#include <Arduino.h>
#include "main.h"
#include "SoftwareSerial.h"

// DEFINE PINAGENS SENSORES E LEDS
int Led_BUILTIN  = 13;
int Led_DIR      = 3;
int Led_ESQ      = 4;
int Led_CIMA     = 5;
int Led_BAIXO    = 6;

int PIN_TX = 11;
int PIN_RX = 10;

uint8_t ledcomm=0;

uint8_t comando; 
bool vardir;
bool varesq;
bool varcima;
bool varbaixo;
bool toogle;

SoftwareSerial espserial(PIN_RX,PIN_TX);
uint8_t rxdata[2];
//=============================
// SETUP CONFIG

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  

  espserial.begin(9600);

  pinMode(Led_BUILTIN, OUTPUT); 
  pinMode(Led_DIR, OUTPUT); 
  pinMode(Led_ESQ, OUTPUT); 
  pinMode(Led_CIMA, OUTPUT); 
  pinMode(Led_BAIXO, OUTPUT); 

}

uint8_t comunicaweb(){

  uint8_t ret=0;
  uint8_t readbyte;

  //espera uma pergunta da ESP
  if (espserial.available() ) {
    readbyte = espserial.read();
    if (readbyte < 5){
      comando = readbyte;

      ret = 1;
    }
  }
    
  //envia resposta
  if (ret){
    Serial.print("cmd=");
    Serial.println(comando);
    ledcomm = !ledcomm;
    digitalWrite(Led_BUILTIN, ledcomm);

    rxdata[0] = comando;
    espserial.write(rxdata[0]);
  }
  return ret;
}


void loop() {
  uint8_t ret=0;
  ret = comunicaweb();

  if (ret){
      switch (comando) {
        case CMD_DIR:
          vardir = !vardir;
          digitalWrite(Led_DIR, vardir);
          break;
        case CMD_ESQ:
          varesq = !varesq;
          digitalWrite(Led_ESQ, varesq);
          break;
        case CMD_CIMA:
          varcima = !varcima;
          digitalWrite(Led_CIMA, varcima);
          break;
        case CMD_BAIXO:
          varbaixo = !varbaixo;
          digitalWrite(Led_BAIXO, varbaixo);
          break;
        default:
          break;
      }
  }
}	




 