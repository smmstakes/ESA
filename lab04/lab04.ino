#include "board.h"

void setup() {
  Serial.begin(115200);

  pinMode(ECU1_AIN1,INPUT); // POTENCIOMETRO ENTRADA
}

int reference = 0;
char serialOption = 118;

void loop() {
  readSerial();
  convertSignals();
  delay(250);
}

void readSerial(){
  if(Serial.available() > 0){
    serialOption = Serial.read();
  }

  if(serialOption == 118){
    reference = map(analogRead(ECU1_AIN1), 0, 1023, 0, 220);
  } else {
    reference = map(analogRead(ECU1_AIN1), 0, 1023, 0, 8000);
  }
}

void convertSignals(){
  Serial.println(reference);
}