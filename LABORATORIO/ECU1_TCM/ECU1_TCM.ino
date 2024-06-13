// Laboratorio 6 - Processamento distribuido
// ECU1 (TCM)

#include "mcp_can.h"
#include "mcp_can_dfs.h"
#include <SPI.h>
#include "Board.h"

// Set CS to pin 10
MCP_CAN CAN0(ECU1_CAN1_CS);  

// ID das mensagens
#define CAN_ID_MARCHA 0x101

// Iniciando campo de dados para rotacao e velocidade
byte data_marcha[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Array armazenar a marcha

// Iniciando contador de marcha
byte marcha = 0;

// Variavel controle de tempo
int inter_envio = 0;
unsigned long ultimo_envio = 0;

// Variaveis para controle dos botoes
int estado1;
int estado2;
int estadoant1;
int estadoant2;


void setup()
{
  Serial.begin(115200);

  // Inicializando o controlador CAN
  // CAN baudrate = 500kbps, MCP_8MHZ
  while((CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK))
  {
	Serial.println("Erro para inicializar o controlador CAN!");
	delay (200); //Aguarda 200ms
  }  
  
  Serial.println("Can send inicializado com sucesso!");
  
  // Configuracao para transmissao e recepcao de mensagens 
  CAN0.setMode(MCP_NORMAL);
  
  // Inicializar os potenciometros
  pinMode(ECU1_DIN1, INPUT); // Definindo Botao DIN#1 como entrada
  pinMode(ECU1_DIN2, INPUT); // Definindo Botao DIN#2 como entrada
  
  estadoant1 = HIGH; // Iniciando o estado do botao DIN#1 como desligado
  estadoant2 = HIGH; // Iniciando o estado do botao DIN#2 como desligado
  
  
}


void loop()
{
	estado1 = digitalRead(ECU1_DIN1); // Lendo estado do botao DIN#1
	estado2 = digitalRead(ECU1_DIN2); // Lendo estado do botao DIN#2
	
	// Botao DIN#1 descrementa a marcha
	if(estado1 == LOW && estadoant1 == HIGH){
		if(marcha > 0){
			marcha--;
		}
	}
	
	estadoant1 = estado1; // Atualizando ultimo estado
	
	// Botao DIN#2 incrementa a marcha
	if(estado2 == LOW && estadoant2 == HIGH){
		if(marcha < 5){
			marcha++;
		}
	}
	
	estadoant2 = estado2; // Atualizando ultimo estado
	
	// Atualizando campos de dados
	data_marcha[0] = marcha;
  
  // ------------------- TRANSIMITINDO MENSAGEM M1: MARCHA ------------------- //
  
  
  inter_envio = millis() - ultimo_envio; // Calculando intervalo de envio da mensagem 
  
  /* Se o intervalo de envio entre o tempo atual 
	e a última mensagem for >= que 200ms, enviar mensagem na CAN */
	
  if(inter_envio >= 200){
	
		byte envio_marcha = CAN0.sendMsgBuf(CAN_ID_MARCHA, 0, 8,data_marcha);
		if(envio_marcha == CAN_OK){
			Serial.println("Mensagem MARCHA: Transmitida!");
			/* Para debud
			Printando rotacao em decimal
			Serial.print("MARCHA: ");
			Serial.println(data_marcha[0]);*/
			
			ultimo_envio = millis(); // Atualizando tempo de envio da mensagem
		} 
		else{
			Serial.println("Erro na transmissao da mensagem!");
		}
  
  }
}

