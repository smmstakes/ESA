#include "board.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(ECU1_DIN1, INPUT);  // BOTAO DIN 1
  pinMode(ECU1_DIN2, INPUT);  // BOTAO DIN 2

  pinMode(ECU1_DOUT1, OUTPUT); // LED Amarelo 1
  pinMode(ECU1_DOUT2, OUTPUT); // LED Amarelo 2
}

unsigned char DIN1;
unsigned char DIN2;
int value = 0, pB1 = 0, pB2 = 0;

void loop() {
  read_din();
  delay(250);
}

void read_din(){
  DIN1 = digitalRead(ECU1_DIN1);
  if(DIN1 != HIGH){
    value++;
    pB1++;
    printSerial(1);
    digitalWrite(ECU1_DOUT1, HIGH); // Acende o LED
    while(DIN1 == LOW){
      DIN1 = digitalRead(ECU1_DIN1);
    }
    digitalWrite(ECU1_DOUT1, LOW); // Apaga o LED
  }


  DIN2 = digitalRead(ECU1_DIN2);
  if(DIN2 != HIGH){
    value--;
    pB2++;
    printSerial(2);
    digitalWrite(ECU1_DOUT2, HIGH); // Acende o LED
    while(DIN2 == LOW){
      DIN2 = digitalRead(ECU1_DIN2);
    }
    digitalWrite(ECU1_DOUT2, LOW); // Apaga o LED
  }
}

void printSerial(int buttom){
  if(buttom == 1){
    Serial.print("O botão 1 foi apertado ");
    Serial.print(pB1);
    Serial.println(" vezes.");
  } else{
    Serial.print("O botão 2 foi apertado ");
    Serial.print(pB2);
    Serial.println(" vezes.");
  }
  
  Serial.print("O valor total está em: ");
  Serial.println(value);
}
