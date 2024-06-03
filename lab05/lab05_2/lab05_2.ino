#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include "board.h"

unsigned char mDLC = 0, mDATA[8];
long unsigned int mID;
int value;
char msgString[128];

MCP_CAN CAN3(ECU3_CAN1_CS);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  if(CAN3.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("Erro para inicializar o controlador CAN3!");
    delay(500);
  }
  Serial.println("Can recive inicializado com sucesso!");

  CAN3.setMode(MCP_NORMAL);
  pinMode(CAN_OK, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(CAN_OK)) {
    CAN3.readMsgBuf(&mID, 0, 8, mDATA);

    value = (mDATA[2] << 8) | mDATA[3];

    Serial.print("Rotação do motor: ");
    Serial.print(value);
    Serial.print(", Velocidade do veículo: ");
    Serial.println(mDATA[4]);

    delay(500);
  }
}
