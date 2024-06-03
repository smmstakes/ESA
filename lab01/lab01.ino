///////////////////////////////////////////////////////////////////////////////////
// Automotive Network Evaluation Board v1.0.0
// PID Controller Lab
// This lab is largely based on a lab written by Bret Comnes and A. La Rosa at
// at Portland State University
///////////////////////////////////////////////////////////////////////////////////
#include <PID_v1.h>
#include  <TimerOne.h>
#include "board.h"
///////////////////////////////////////////////////////////////////////////////////
/// Macro definitions
///////////////////////////////////////////////////////////////////////////////////
#define  SERIAL_DELAY     500
#define  SAMPLE_RATE      1 
#define  TIME_50MS        50000

///////////////////////////////////////////////////////////////////////////////////
/// PID tunning parameters 
///////////////////////////////////////////////////////////////////////////////////
//Initial Proportional Gain
float Kp=1; 
//Initial Integral Gain
float Ki=1;
//Initial Differential Gain 
float Kd=1; 
/////////////////////////////////////////////////////////////////////////
/// Global variables
/////////////////////////////////////////////////////////////////////////
int    ldrHighLimit   = 900;
int    ldrLowLimit    = 0;
double Reference      = 0;
double Input          = 0;
double Output         = 0;
bool   pidCtrState    = true;
unsigned char         ledState    = HIGH;
unsigned char         pbCurrState;

//Miscellaneous
unsigned  long now = 0;
unsigned  long lastMessage = 0; 

/////////////////////////////////////////////////////////////////////////
/// Declare and initialize PID Controller
/////////////////////////////////////////////////////////////////////////  
PID PID_CONTROLLER(&Input, &Output, &Reference, Kp, Ki, Kd, DIRECT);

void setup()
{
  Serial.begin(115200);
  //Setting pins
  pinMode(ECU1_LDR_LED, OUTPUT);  
  //Potentiometer input
  pinMode(ECU1_AIN1,INPUT);
  //LDR pin input
  pinMode(ECU1_LDR_AIN,INPUT);  
  //Pushbutton DIN#1
  pinMode(ECU1_DIN1,INPUT);
  //PID Controller Led state
  //ON-PID_CTL_ON, OFF-PID_CTR_OFF
  pinMode(ECU1_DOUT1, OUTPUT);
  //Set initial LED state
  digitalWrite(ECU1_DOUT1,HIGH);  
  //Getting LDR Min and Max values
  setLDR_Limits();  

  //Getting initial reference value
  Reference = map(analogRead(ECU1_AIN1),0,1023,0,255);  
  //Getting initial input value
  Input     = map(analogRead(ECU1_LDR_AIN),ldrLowLimit,ldrHighLimit,0,255); 

  //Turn on the PID loop
  PID_CONTROLLER.SetMode(AUTOMATIC);
  //Sets the sample rate
  PID_CONTROLLER.SetSampleTime(SAMPLE_RATE);
  //initialize timestamp
  lastMessage = millis();
  
  Timer1.initialize(TIME_50MS); 
  Timer1.attachInterrupt(timer1_callback);   
  
}
void loop()
{  
  ///Reading reference value
  Reference = map(analogRead(ECU1_AIN1),0,1023,0,255);
  //Get the light level
  Input    = map(analogRead(ECU1_LDR_AIN),ldrLowLimit,ldrHighLimit,0,255); 
  //Reading DIN#1 
  pbCurrState = digitalRead(ECU1_DIN1);
  //Checking buttonState  
  if(pidCtrState)
  {
    //Run the PID loop
    PID_CONTROLLER.Compute();  
  }
  else
  {
    Output=Reference;
  }  
  //Write out the output from the PID loop to our LED pin
  analogWrite(ECU1_LDR_LED, Output);

  print_output(); 
  
}

void print_output()
{
  //Keep track of time
  now = millis();
  if(now - lastMessage > SERIAL_DELAY) 
  { 
   
    Serial.print("Reference:");    
    Serial.print(Reference);
    Serial.print(",");    
    Serial.print("Input:");
    Serial.print(Input);
    Serial.print(",");
    Serial.print("Output:");    
    Serial.println(Output);     
    
    //update the time stamp.
    lastMessage = now;
    
  } 
}
/////////////////////////////////////////////////////////////////////////
/// Function to measure LDR Limits
//  ldrHighLimit - Maximum LDR value
//  ldrLowLimit  - Minimum LDR value
/////////////////////////////////////////////////////////////////////////

void setLDR_Limits()
{
  short i;
  short lightOutLevel=0;
  int   lightInLevel=0;

  //Increasing light Output Level
  for (i=0; i<=255;i++){
    //Writting lightLevel    
    analogWrite(ECU1_LDR_LED,lightOutLevel);
    delay(20);
    //Reading lightLevel (LDR)
    lightInLevel = analogRead(ECU1_LDR_AIN);
    //Checking ldrHighLimit
    if(lightInLevel>ldrHighLimit)
    {
        ldrHighLimit = lightInLevel;             
    }    
    //Increment lightLevel
    lightOutLevel = lightOutLevel+1; 
  }
  //Maximum output light level
  lightOutLevel = 255;
  
  for (i=0; i<=255;i++){
    //Writting lightLevel    
    analogWrite(ECU1_LDR_LED,lightOutLevel);
    delay(20);
    //Reading lightLevel
    lightInLevel = analogRead(ECU1_LDR_AIN);
    //Checking ldrLowLimit
    if(Input<ldrLowLimit){
        ldrLowLimit = lightInLevel;            
    }    
    lightOutLevel = lightOutLevel-1; 
  }  
 
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/  
void serialEvent(void)
{
  
  if (Serial.available() > 0) 
    {
      //If we sent the program a command dealwith it
      for (int x = 0; x < 4; x++) 
      {
        switch (x) 
        {
          case 0:
          {
            Kp = Serial.parseFloat();
          }
          break;
          case 1:
          {
            Ki = Serial.parseFloat();
          }
          break;
          case 2:
          {
            Kd = Serial.parseFloat();
          }
          break;
          case 3:
          {
            for (int y = Serial.available(); y == 0; y--) 
            {
              Serial.read(); //Clear out any residual junk
            }
          }
        break;
        }
      } 
      Serial.println("Updating PID Controller");
      Serial.print("[Kp]=");
      Serial.print("[");
      Serial.print(Kp);
      Serial.print("]");
      Serial.print("[Ki]=");
      Serial.print("[");
      Serial.print(Ki);
      Serial.print("]");
      Serial.print("[Kd]=");
      Serial.print("[");
      Serial.print(Kd);
      Serial.println("]");
      
      //Setting PID Controller
      PID_CONTROLLER.SetTunings(Kp, Ki, Kd);
    }
}

void timer1_callback()
{
  
  static short i=0;
  static unsigned char countPBPress   =0; 
  
  pbCurrState = digitalRead(ECU1_DIN1);

  if(pbCurrState==LOW)
  {
      //Increment contPressButton
      countPBPress++;      
      //Check button up was pressed (100ms)
      if (countPBPress>=6)
      {     
         countPBPress=0;
         if (pbCurrState == LOW)
         {
            pidCtrState = !pidCtrState;
            digitalWrite(ECU1_DOUT1,pidCtrState);
         }
      }   
  }
  else
  {
        if(countPBPress>0)
        {
           countPBPress--;        
      }     
    }  
 }
