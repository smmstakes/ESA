#include <SPI.h>
#include "board.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define CAN_ID 0x123

MCP_CAN CAN4(ECU4_CAN1_CS);
byte dataMsg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
long unsigned int mId;

void setup() {
  Serial.begin(115200);

  while (CAN4.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("Erro para inicializar o controlador CAN4!");
    delay(500);
  }
  Serial.println("CAN4 inicializado com sucesso!");

  CAN4.setMode(MCP_NORMAL);
  pinMode(CAN_OK, INPUT);
}

void loop() {
  if(digitalRead(CAN_OK)) {
    CAN4.readMsgBuf(&mId, 0, 8, dataMsg);

    char gear = dataMsg[1];
    int rpm = (dataMsg[2] << 8) | dataMsg[3];
    char vel = dataMsg[4];

    String msg = "Rotação = " + String(rpm) + ", Velocidade = " + String(vel) + ", Marcha Atual = " + String(gear);
    Serial.println(msg);
  }

  delay(500);
}
