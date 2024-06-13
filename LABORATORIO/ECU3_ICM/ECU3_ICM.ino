// Laboratorio 6 - Processamento distribuido
// ECU3 (ICM)

#include "mcp_can.h"
#include "mcp_can_dfs.h"
#include <SPI.h>
#include "Board.h"

// Set CS to pin 10
MCP_CAN CAN0(ECU2_CAN1_CS);  

// Pino 2 interrupt
#define CAN0_INT 2

// ID das mensagens
#define CAN_ID_MARCHA 0x101
#define CAN_ID_ROTACAO 0x102
#define CAN_ID_VELOCIDADE 0x103

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

// Iniciando variaveis
unsigned char marcha = 0;
unsigned char velocidade = 0;
unsigned int rotacao = 0;

// Variaveis contadoras
int interMsg = 0;
unsigned long ult_Msg = 0;

void setup()
{
	Serial.begin(115200);
  
  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
   while((CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK))
  {
	Serial.println("Erro para inicializar o controlador CAN!");
	delay (200); //Aguarda 200ms
  }  
  
  Serial.println("Can iniciado com sucesso!");
  
  CAN0.setMode(MCP_NORMAL);    // Configuracao para modo normal

  pinMode(CAN0_INT, INPUT);    // Configurando PIN para Interrupt como Input
  
  Serial.println("MCP2515 Library Receive Example...");
  
}

void loop(){
	
	if(!digitalRead(CAN0_INT)){                   // If CAN0_INT pin is low, read receive buffer
	
		// Lendo mensagens na CAN
		CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    }
	
	// ------------------- DESENCAPSULANDO M1, M2 E M3 ------------------- //
		
	if(rxId == CAN_ID_ROTACAO){
		rotacao = (rxBuf[0] << 8) | rxBuf[1];
	}else if(rxId == CAN_ID_VELOCIDADE){
		velocidade = rxBuf[0];
	}else if(rxId == CAN_ID_MARCHA){
		marcha = rxBuf[0];
	}
	// ------------------- MOSTRANDO MENSAGENS M1, M2 E M3 NA SERIAL A CADA 500MS ------------------- //
	
	interMsg = millis() - ult_Msg;
	
	if(interMsg >= 500){
		Serial.print("Rotacao: ");
		Serial.print(rotacao);
		Serial.print(" rpm, ");
	
		Serial.print("Velocidade: ");
		Serial.print(velocidade);
		Serial.print(" km/h, ");
	
		Serial.print("Marcha Atual: ");
		Serial.print(marcha);
		Serial.println("");
		
		// Serial.println(interMsg); // Printando intervalo de tempo que a mensagem foi enviada
		
		ult_Msg = millis();
		
	}
}

