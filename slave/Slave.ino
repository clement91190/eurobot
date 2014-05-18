
//#include "../lib/Period.h"
#include "Autom.h"
#include "pins.h"
#include "OrdersRaspberry.h"
#include <pnew.cpp>


#define ALLUMAGE 0
#define STARTMIS 1
#define GAME 2
#define END 3

Autom* slave;
OrdersRaspberry* com;
int state;
long timer;

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
  //int prescalerVal = 0x07;
  //TCCR0B = xxxxx010, frequency is 8 kHz
  //TCCR0B &= ~prescalerVal;
  //prescalerVal = 1;
  //TCCR0B |= 1;
  slave = new Autom();
  com = new OrdersRaspberry(slave);
  Serial.println("#SLAVE READY");
  write_serial_strat();
  state = ALLUMAGE; 
  timer = 0;
}

void loop(){
  
  /*if (ticD <= -30){
    dir = 1; 
  }
  if (ticD >=0){
    dir = -1;
  }*/
  //print_tic_odo();
  
 //write_cmd();   
 //Serial.println(vitesse);
 // Serial.print(dir); 
  //delay(800);
   //int sensorValue = analogRead(A5);
   //Serial.print(int(sensorValue * 300.0 / 1023 + 100.0));
   //Serial.println( " mm");
   if (state == ALLUMAGE && digitalRead(PIN_AN_START) == 0)
    {
        state = STARTMIS;
        Serial.println("# STARTIN");
    }
    if (state == STARTMIS && digitalRead(PIN_AN_START) == 1)
    {
        state = GAME ;
        Serial.println("# START");
        timer = millis(); 
    }

    if (state == END || (state == GAME && timer > 90000 + millis()))
    {
        Serial.println("# ENDG");
        state = END;
        slave->stop();
        
        }
    else 
        {
        com->run();
        slave->run();
        }

   

   delay(1);
      
}

