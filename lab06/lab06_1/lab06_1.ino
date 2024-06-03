// Módulo de Controle de Transmissão (TCM) #ECU1
// Ler a marcha

#include <SPI.h>
#include "board.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define CAN_ID 0x123

MCP_CAN CAN1(ECU1_CAN1_CS);
byte can_gear[3] = {0x00, 0x00, 0x00};

char gear = 1;
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
  delay(500);
  send_data();
}

void read_din(){
  DIN1 = digitalRead(ECU1_DIN1);
  if(DIN1 != HIGH){
    gear++;
    while(DIN1 == LOW){
      DIN1 = digitalRead(ECU1_DIN1);
    }
  }

  DIN2 = digitalRead(ECU1_DIN2);
  if(DIN2 != HIGH){
    gear--;
    while(DIN2 == LOW){
      DIN2 = digitalRead(ECU1_DIN2);
    }
  }
}

void send_data(){
  can_gear[1] = gear;
  byte sndStat = CAN1.sendMsgBuf(CAN_ID, 0, 3, can_gear);
  if (sndStat == CAN_OK) {
    Serial.println("Mensagem 1 enviada com sucesso!");
  } else {
    Serial.println("Erro para enviar a mensagem 1...");
  }
}