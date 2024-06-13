// Laboratorio 6 - Processamento distribuido
// ECU2 (ECM)

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
float nd = 3.55; 	// Taxa de transmissao diferencial
float ng = 0.0; 	// Taxa de trasminssao da transmissao
float pi = 3.14159265; 
float Rw = 0.326;	// Raio da Roda
int pot1; // Guardar posicao do potenciometro

// Variaveis contadoras
int interM2 = 0;
unsigned long ult_M2 = 0;
int interM3 = 0;
unsigned long ult_M3 = 0;

byte data_rotacao[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 	// Array para rotacao
byte data_velocidade[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Array para velocidade

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
  
  pinMode(ECU2_AIN1,INPUT); 	// Configurando pino do potenciomêtro como entradada				
  
}

void loop()
{
	if(!digitalRead(CAN0_INT)){                        // If CAN0_INT pin is low, read receive buffer
	
		// Lendo mensagens na CAN
		CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    }
	
	// Armazenando valor da marcha atraves do rxBuf
	unsigned char marcha = rxBuf[0];
	
	// Definindo ROTACAO com base na marcha e posicao do potenciometro
	if(marcha == 0) {
		rotacao = (int)((7.82013)*analogRead(ECU2_AIN1));
		ng = 0.0;
	}else if(marcha == 1) {
		rotacao = (int)((3.91006)*analogRead(ECU2_AIN1));
		ng = 3.83;
	}else if(marcha == 2) {
		rotacao = (int)((4.69208)*analogRead(ECU2_AIN1)); 
		ng = 2.36;
	}else if(marcha == 3){
		rotacao = (int)((5.47409)*analogRead(ECU2_AIN1));
		ng = 1.69;
	}else if(marcha == 4) {
		rotacao = (int)((6.256109)*analogRead(ECU2_AIN1));
		ng = 1.31;
	}else if(marcha == 5) {
		rotacao = (int)((7.03812)*analogRead(ECU2_AIN1));
		ng = 1.00;
	}

	/*////// Para debug	
	Serial.print("Marcha: ");
	Serial.println(marcha);
	*/
	
	// Armazenando a rotacao no array data rotacao
	data_rotacao[0] = (rotacao >> 8);
	data_rotacao[1] = (rotacao & 0x00FF);
	
	// Calculo VELOCIDADE
	if (marcha == 0){
		velocidade = 0;
	}else if(marcha > 0){
		// Velocidade me km/h - Fator de conversao 0.001*2*pi*60
		velocidade  = (Rw*rotacao*0.001*2*pi*60)/(ng*nd);
	}
	
	// Armazenando valor da velocidade 
	data_velocidade[0] = velocidade;
	
	// ------------------- TRANSIMITINDO MENSAGEM M2: ROTACAO ------------------- //
	
	/* Se o intervalo de envio entre o tempo atual 
	e a última mensagem for >= que 100ms, enviar mensagem na CAN */
	interM2 = millis()-ult_M2; 
	
	if(interM2 >= 100){
		byte envio_rotacao = CAN0.sendMsgBuf(CAN_ID_ROTACAO, 0, 8,data_rotacao);
		
		if(envio_rotacao == CAN_OK){
			/* Para debug
			Serial.println("Mensagem ROTACAO: Transmitida!");
			Serial.print("ROTACAO: ");
			Serial.print(data_rotacao[0] << 8 | data_rotacao[1]);
			Serial.println(" rpm");
			Serial.println("-------------");*/
			
			ult_M2 = millis(); // Atualizando tempo de envio da mensagem
		} 
		else{
			Serial.println("Erro na transmissao da ROTACAO!");
		}
	}
	
	// ------------------- TRANSIMITINDO MENSAGEM M3: VELOCIDADE ------------------- //
	
	/* Se o intervalo de envio entre o tempo atual 
	e a última mensagem for >= que 250ms, enviar mensagem na CAN */
	interM3 = millis()-ult_M3; 
	
	if(interM3 >= 250){
		byte envio_velocidade = CAN0.sendMsgBuf(CAN_ID_VELOCIDADE, 0, 8,data_velocidade);
		
		if(envio_velocidade == CAN_OK){
			/* Para debug
			Serial.println("Mensagem VELOCIDADE: Transmitida!");
			Serial.print("VELOCIDADE: ");
			Serial.print(data_velocidade[0]);
			Serial.println(" km/h");
			Serial.println("-------------");*/
			
			ult_M3 = millis(); // Atualizando tempo de envio da mensagem
		} 
		else{
			Serial.println("Erro na transmissao da VELOCIDADE!");
		}
	}
}

