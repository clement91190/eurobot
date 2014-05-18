//#include "../lib/Period.h"
#include "pins.h"
#include "OrdersRaspberry.h"
#include <pnew.cpp>

IO* io;
OrdersRaspberry* com;

void setup()
{ 
  Serial.begin(9600) ; 
  Serial.println("#ACTIO2 READY");
  io = new IO();
  Serial.println("init IO");
  com = new OrdersRaspberry(io);
  Serial.println("init COM");

}



void loop(){
  
   delay(1);
   com->run();
   io->run();
      
}

