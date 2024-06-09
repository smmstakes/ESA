// Módulo de Controle do Motor (ECM) #ECU3

#include <SPI.h>
#include "board.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#define CAN_ID 0x123

MCP_CAN CAN3(ECU3_CAN1_CS);
byte can_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
long unsigned int mId;

unsigned long prevGearTime1 = 0;
unsigned long prevRpmTime1 = 0;

unsigned long prevGearTime2 = 0;
unsigned long prevRpmTime2 = 0;
unsigned long prevVelTime2 = 0;

void setup() {
  Serial.begin(115200);

  pinMode(ECU3_AIN1, INPUT); // POTENCIOMETRO ENTRADA RPM

  while (CAN3.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("Erro para inicializar o controlador CAN3!");
    delay(500);
  }
  Serial.println("CAN3 inicializado com sucesso!");

  CAN3.setMode(MCP_NORMAL);
  pinMode(CAN_OK, INPUT);
}

void loop() {
  pinMode(CAN_OK, INPUT);
  char gear;
  int rpm_max, rpm;

  unsigned long actualGearTime = millis();
  if(actualGearTime - prevGearTime1 >= 200) gear = read_gear();

  unsigned long actualRpmTime = millis();
  if(actualRpmTime - prevRpmTime1 >= 100) {
    rpm_max = calc_rpm(gear);
    rpm = map(analogRead(ECU3_AIN1), 0, 1023, 0, rpm_max);
  }

  unsigned short vel = calc_vel(gear, rpm);

  send_msg(gear, rpm, vel);
}

char read_gear() {
  if(digitalRead(CAN_OK)){
    CAN3.readMsgBuf(&mId, 0, 3, can_data);
   return can_data[1];
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
    
    case 2:
      gear_rate = 2.36;
      break;
    
    case 3:
      gear_rate = 1.69;
      break;
    
    case 4:
      gear_rate = 1.31;
      break;

    case 5:
      gear_rate = 1.00;
      break;

    default:
      break;
  }

  return (unsigned short) ((tire_radius * rpm) / (differential_rate * gear_rate));
}

void send_msg(char gear, int rpm, char vel){
  // pinMode(CAN_OK, OUTPUT);

  unsigned long actualGearTime = millis();
  if(actualGearTime - prevGearTime2 >= 200){
    can_data[1] = gear;

    byte sndStat = CAN3.sendMsgBuf(CAN_ID, 0, 3, can_data);
    if (sndStat == CAN_OK) {
      Serial.println("Mensagem 1 enviada com sucesso!");
    } else {
      Serial.println("Erro para enviar a mensagem 1...");
    } 
  }

  unsigned long actualRpmTime = millis();
  if(actualRpmTime - prevRpmTime2 >= 100){
    can_data[2] = (rpm >> 8) & 0xFF;
    can_data[3] = rpm & 0xFF;

    byte sndStat = CAN3.sendMsgBuf(CAN_ID, 0, 4, can_data);
    if (sndStat == CAN_OK) {
      Serial.println("Mensagem 2 enviada com sucesso!");
    } else {
      Serial.println("Erro para enviar a mensagem 2...");
    }
  }
  
  unsigned long actualVelTime = millis();
  if(actualVelTime - prevVelTime2 >= 250){
    can_data[4] = vel;

    byte sndStat = CAN3.sendMsgBuf(CAN_ID, 0, 5, can_data);
    if (sndStat == CAN_OK) {
      Serial.println("Mensagem 3 enviada com sucesso!");
    } else {
      Serial.println("Erro para enviar a mensagem 3...");
    }
  }
}
