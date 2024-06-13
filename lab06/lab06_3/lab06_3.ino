#include <SPI.h>
#include "board.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define CAN_ID 0x123

MCP_CAN CAN4(CAN1_CS_PIN);
byte dataMsg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
long unsigned int mId;
unsigned long prevTime = 0;

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
  pinMode(CAN_OK, INPUT);
  unsigned long actualTime = millis();

  if(digitalRead(CAN_OK) && ((actualTime - prevTime) >= 500)) {
    CAN4.readMsgBuf(&mId, 0, 8, dataMsg);

    short gear = dataMsg[1];
    int rpm = (dataMsg[2] << 8) | dataMsg[3];
    unsigned short vel = dataMsg[4];

    Serial.print("Rotação = ");
    Serial.print(rpm);
    Serial.print(", Velocidade = ");
    Serial.print(vel);
    Serial.print(", Marcha Atual = ");
    Serial.println(gear);

    prevTime = actualTime;
  }
}
