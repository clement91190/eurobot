//#include "../lib/Period.h"
#include "pins.h"
#include "OrdersRaspberry.h"
#include <pnew.cpp>

IO* io;
OrdersRaspberry* com;

void setup()
{
  /*attachInterrupt(PIN_ODO_INCG, inc_ticG, RISING);
  attachInterrupt(PIN_ODO_INCD, inc_ticD, RISING);
  Serial.begin(9600) ; 
  pinMode(PIN_MOT_DIRG, OUTPUT);
  pinMode(PIN_MOT_DIRD, OUTPUT);
  pinMode(PIN_MOT_CMDG, OUTPUT);
  pinMode(PIN_MOT_CMDD, OUTPUT);
 */ 
  io = new IO();
  com = new OrdersRaspberry(io);
  Serial.println("ACTIO1 READY");

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
   delay(1);
   //int sensorValue = analogRead(A5);
   //Serial.print(int(sensorValue * 300.0 / 1023 + 100.0));
   //Serial.println( " mm");
   
    com->run();
      
}

