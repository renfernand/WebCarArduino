#include <Arduino.h>
#include "main.h"
#include "SoftwareSerial.h"
#include "HCSR04.h"

//habilito ou nao o uso da serial para comounicacao com um servidor web
//neste caso o arduino trabalha somente como uma remota
#define WEB_CLIENT_SERIAL_ENABLE 0

// DEFINE PINAGENS SENSORES E LEDS
int Led_BUILTIN  = 13;
int Led_DIR      = 3;
int Led_ESQ      = 4;
int Led_CIMA     = 5;
int Led_BAIXO    = 6;

#define PIN_MD_IN1  11
#define PIN_MD_IN2  10
#define PIN_ME_IN3  5
#define PIN_ME_IN4  6

#define P_TRIG 13
#define P_ECHO 12

UltraSonicDistanceSensor distanceSensor(P_TRIG, P_ECHO);

int PIN_TX = 11;
int PIN_RX = 10;

uint8_t ledcomm=0;

uint8_t comando; 
bool vardir;
bool varesq;
bool varcima;
bool varbaixo;
bool toogle;

#if WEB_CLIENT_SERIAL_ENABLE
SoftwareSerial espserial(PIN_RX,PIN_TX);
uint8_t rxdata[2];
#endif
//=============================
// SETUP CONFIG

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  

#if WEB_CLIENT_SERIAL_ENABLE
  espserial.begin(9600);
#endif

  pinMode(Led_BUILTIN, OUTPUT); 
  pinMode(PIN_MD_IN1, OUTPUT); 
  pinMode(PIN_MD_IN2, OUTPUT); 
  pinMode(PIN_ME_IN3, OUTPUT); 
  pinMode(PIN_ME_IN4, OUTPUT); 

}

#if WEB_CLIENT_SERIAL_ENABLE
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
#endif

void loop() {
  uint8_t ret=0;
  uint16_t dist=0;
  
 #if WEB_CLIENT_SERIAL_ENABLE
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
          digitalWrite(PIN_ME_IN3, HIGH);
          digitalWrite(PIN_ME_IN4, LOW);
          break;
        case CMD_BAIXO:
          digitalWrite(PIN_ME_IN3, LOW);
          digitalWrite(PIN_ME_IN4, HIGH);
          break;
        default:
          break;
      }
  }
#endif

  dist = distanceSensor.measureDistanceCm();
  if (dist < 20){
    digitalWrite(PIN_MD_IN1, HIGH);
    digitalWrite(PIN_MD_IN2, LOW);
    comando = CMD_DIR;
  }
  else {
    if (dist < 40) {
      digitalWrite(PIN_ME_IN3, HIGH);
      digitalWrite(PIN_ME_IN4, LOW);
      comando = CMD_ESQ;
    }
    else{
      comando = 0;
      digitalWrite(PIN_MD_IN1, LOW);
      digitalWrite(PIN_MD_IN2, LOW);
      digitalWrite(PIN_ME_IN3, LOW);
      digitalWrite(PIN_ME_IN4, LOW);      
    }
  }
      
  delay(1000);

  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.println(" cm");
  Serial.print("Comando: ");
  Serial.println(comando);
  

}	




 