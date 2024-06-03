#include "board.h"

double Reference = 0;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(ECU1_LDR_LED, OUTPUT);  // LDR LED SAÍDA (branco)

  pinMode(ECU1_AIN1,INPUT); // POTENCIOMETRO ENTRADA

  pinMode(ECU1_LDR_AIN,INPUT);  // LDR PIN ENTRADA

  Reference = map(analogRead(ECU1_AIN1),0,1023,0,10);
}

void printSerial(double Reference) {
  // Serial.print("Analogico está em: ");
  Serial.println(Reference);  //Implementar converesor para float com regra de 3
  delay(250);
}

void loop() {
  Reference = map(analogRead(ECU1_AIN1),0,1023,0,10);
  printSerial(Reference);

  if (Reference >= 8.5){
    digitalWrite(ECU1_LDR_LED, HIGH);
  } else {
    digitalWrite(ECU1_LDR_LED, LOW);
  }
}


