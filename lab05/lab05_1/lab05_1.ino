#include <mcp_can.h>  
#include <mcp_can_dfs.h> 

#include <SPI.h>
#include "board.h"

// Set CS to pin 10
MCP_CAN CAN1(ECU1_CAN1_CS);

#define CAN_ID 0x123
byte can_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int input1;
unsigned char input2;

void setup() {
  Serial.begin(115200);

  pinMode(ECU1_AIN1, INPUT); // POTENCIOMETRO ENTRADA M1 (RPM)
  pinMode(ECU1_AIN2, INPUT); // POTENCIOMETRO ENTRADA M2 (VEL)

  while (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("Erro para inicializar o controlador CAN1!");
    delay(500);
  }
  Serial.println("Can send inicializado com sucesso!");

  // 2- Configuracao para transmissao e recepcao de mensagens
  CAN1.setMode(MCP_NORMAL);
}

void loop() {
  // Lendo os valores de rotação e velocidade dos potenciometros
  input1 = map(analogRead(ECU1_AIN1), 0, 1023, 0, 8000); // 2 bytes
  input2 = map(analogRead(ECU1_AIN2), 0, 1023, 0, 250); // 1 byte

  sendRotacao();
  sendVel();
}

void sendRotacao(){
  can_data[2] = (input1 >> 8) & 0xFF;  // Byte mais significativo
  can_data[3] = input1 & 0xFF;          // Byte menos significativo

  byte sndStat = CAN1.sendMsgBuf(CAN_ID, 0, 8, can_data);
  if (sndStat == CAN_OK) {
    Serial.println("Mensagem 1 enviada com sucesso!");
  } else {
    Serial.println("Erro para enviar a mensagem 1...");
  }
  delay(500);
}

void sendVel(){
  can_data[4] = input2;
  byte sndStat = CAN1.sendMsgBuf(CAN_ID, 0, 8, can_data);
  if (sndStat == CAN_OK) {
    Serial.println("Mensagem 2 enviada com sucesso!");
  } else {
    Serial.println("Erro para enviar a mensagem 2...");
  }  
  delay(500);
}
