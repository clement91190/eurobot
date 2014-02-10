//#include "../lib/Period.h"
#include "Autom.h"
#include "pins.h"


Autom* slave;
int dir = 1;
int vitesse = 100;
void print_tic_odo()
{
    Serial.print(" odoG :");
    Serial.print(ticG);
    Serial.print(" odoD :");
    Serial.println(ticD);
}

void setup()
{
  attachInterrupt(PIN_ODO_INCG, inc_ticG, RISING);
  attachInterrupt(PIN_ODO_INCD, inc_ticD, RISING);
  Serial.begin(9600) ; 
  pinMode(PIN_MOT_DIRG, OUTPUT);
  pinMode(PIN_MOT_DIRD, OUTPUT);
  pinMode(PIN_MOT_CMDG, OUTPUT);
  pinMode(PIN_MOT_CMDD, OUTPUT);
  int prescalerVal = 0x07;
  //TCCR0B = xxxxx010, frequency is 8 kHz
  TCCR0B &= ~prescalerVal;
  //prescalerVal = 1;
  TCCR0B |= 1;
  slave = new Autom();
}

void write_cmd(){
 if (dir == 1)
    {
       digitalWrite(PIN_MOT_DIRG, LOW);
       digitalWrite(PIN_MOT_DIRD, HIGH);
   
    }
    else
    {
     digitalWrite(PIN_MOT_DIRG, HIGH);
     digitalWrite(PIN_MOT_DIRD, LOW);
    
    } 
 analogWrite(PIN_MOT_CMDG, vitesse);
 analogWrite(PIN_MOT_CMDD, vitesse);
  
}

void loop(){
  
  /*if (ticD <= -30){
    dir = 1; 
  }
  if (ticD >=0){
    dir = -1;
  }
  print_tic_odo();
  */
  
 //write_cmd();   
 //Serial.println(vitesse);
 // Serial.print(dir); 
  //delay(800);
    slave->run();
      
}

