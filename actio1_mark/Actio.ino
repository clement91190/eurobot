//#include "../lib/Period.h"
#include "pins.h"
#include "OrdersRaspberry.h"
#include <pnew.cpp>

IO* io;
OrdersRaspberry* com;

void setup()
{
  Serial.begin(9600) ; 
  attachInterrupt(PIN_COULEUR_D, inc_pulse_color_d, RISING);
  attachInterrupt(PIN_COULEUR_G, inc_pulse_color_g, RISING);
  io = new IO();
  com = new OrdersRaspberry(io);
  Serial.println("#ACTIO READY");

}



void loop(){

    com->run();
    io->run();
    delay(1);
}

