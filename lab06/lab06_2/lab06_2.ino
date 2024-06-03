// Módulo de Controle do Motor (ECM) #ECU3

#include <SPI.h>
#include "board.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define CAN_ID 0x123

MCP_CAN CAN3(ECU3_CAN1_CS);
byte can_datas[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
long unsigned int mId;

void setup() {
  Serial.begin(115200);

  pinMode(ECU3_AIN1, INPUT); // POTENCIOMETRO ENTRADA RPM

  while (CAN3.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("Erro para inicializar o controlador CAN3!");
    delay(500);
  }
  Serial.println("CAN inicializado com sucesso!");

  // 2- Configuracao para transmissao e recepcao de mensagens
  CAN3.setMode(MCP_NORMAL);
  pinMode(CAN_OK, INPUT);
}

void loop() {
  char gear = read_gear(); // Mudar no lab06_1 o gear para 0
  int rpm_max = calc_rpm(gear);
  
  rpm = map(analogRead(ECU3_AIN1), 0, 1023, 0, rpm_max);

  char vel = calc_vel();
}

char read_gear() {
  if(digitalRead(CAN_OK)){
    CAN3.readMsgBuf(&mId, 0, 3, can_datas);
   return can_datas[1];
  }
}

int calc_rpm(char gear) {
  switch (gear) {
    case 0:
      return 8000;
      
    case 1:
      return 4000;
          
    case 2:
      return 4800;
          
    case 3:
      return 5600;
          
    case 4:
      return 6400;
          
    case 5:
      return 7200;
          
    default:
      return 0;
  }
}

int calc_vel(char gear, int rpm) {
  float tire_radius = 0.326;
  float differential_rate = 3.55;
  float gear_rate;

  switch (gear){
    case 1:
      gear_rate = 3.83;
      break;
    
    default:
      break;
  }

  return ;
}
