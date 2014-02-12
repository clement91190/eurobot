#include "Communication.h"

Communication::Communication(Autom* slave_):period_com(50), slave(slave_){
}

void Communication::treat_orders(){
// read the serial port and call ... depending on what it received
    
    if (Serial.available() > 0) {
        int inByte = Serial.read();
        if (inByte == 'g'){
        } 
    }
}

void Communication::run(){
     if (period_com.is_over())
    {
        period_com.reset();
        treat_orders();
        // add the rest here
    }
    
}
