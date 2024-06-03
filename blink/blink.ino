////////////////////////////////////////////////////////////////////////////////
// Automotive Network Evaluation Board v1.0 (Arquivo Blink.h)
// Pisca um led a cada 1 segundo
// Exemplo de domínio público
// https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
////////////////////////////////////////////////////////////////////////////////
#include "board.h"
// Executa somente uma vez após pressionar o botão reset ou ligar
void setup() {
// Inicializa o pino digital como entrada.
pinMode(LED_BUILTIN, OUTPUT);
}
//Executa continuamente
void loop() {
//Liga o led
//digitalWrite (LED_BUILTIN, HIGH);
//Aguarda 500ms
//delay(500);
//Desliga o led
digitalWrite (LED_BUILTIN, LOW);
//Aguarda 500ms
//delay(500);
}