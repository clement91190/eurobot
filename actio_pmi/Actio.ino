//#include "../lib/Period.h"
#include "pins.h"
#include "OrdersRaspberry.h"
#include <pnew.cpp>

IO* io;
OrdersRaspberry* com;

void setup()
{

  attachInterrupt(PIN_ODO_INC, inc_tic_odo, RISING);         
  attachInterrupt(PIN_COULEUR, inc_pulse_color, RISING);         
  Serial.begin(9600);
  io = new IO();
  com = new OrdersRaspberry(io);
  Serial.println("#ACTIO READY");

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
    io->run();
      
}

