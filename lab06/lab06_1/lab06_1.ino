// Módulo de Controle de Transmissão (TCM) #ECU1
// Ler a marcha

#include <SPI.h>
#include "board.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define CAN_ID 0x123

MCP_CAN CAN1(ECU1_CAN1_CS);
byte can_gear[3] = {0x00, 0x00, 0x00};

unsigned long prevTime = 0;

char gear = 0;
char DIN1, DIN2;

void setup() {
  Serial.begin(115200);
  pinMode(ECU1_DIN1, INPUT);
  pinMode(ECU1_DIN2, INPUT);

  while (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("Erro para inicializar o controlador CAN1!");
    delay(500);
  }
  Serial.println("Can send inicializado com sucesso!");

  // 2- Configuracao para transmissao e recepcao de mensagens
  CAN1.setMode(MCP_NORMAL);
}

void loop() {
  read_din();
  send_data();
}

void read_din(){
  DIN1 = digitalRead(ECU1_DIN1);
  DIN2 = digitalRead(ECU1_DIN2);

  if(DIN1 != HIGH){
    if(gear < 5) gear++;
    while(digitalRead(ECU1_DIN1) == LOW){
      delay(10);
    }
  }

  if(DIN2 != HIGH){
    if(gear > 0) gear--;
    while(digitalRead(ECU1_DIN2) == LOW){
      delay(10);
    }
  }
}

void send_data(){
  unsigned long actualTime = millis();
  can_gear[1] = gear;
  if(actualTime - prevTime >= 200) {
    prevTime = actualTime;
    byte sndStat = CAN1.sendMsgBuf(CAN_ID, 0, 3, can_gear);
    if (sndStat == CAN_OK) {
      Serial.println("Mensagem 1 enviada com sucesso!");
    } else {
      Serial.println("Erro para enviar a mensagem 1...");
    }
  }
}