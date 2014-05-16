#ifndef _ORDERSRASPBERRY_H
#define _ORDERSRASPBERRY_H

#include "IO.h"
#include "Arduino.h"
#include <Coord.h>
//#include <stdio.h>
//
#include <Period.h>
#include <iterator>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

//3 fichiers a inclure pour influencer leur comportement
//#include "IO/IO.h"
//#include "Slave/Slave.h"
//#include "MAE/Actionneur.h"
		
class OrdersRaspberry {
	
    //Vitesse determinerVitesse(int v);

	public: 
	OrdersRaspberry(IO* io_);
	void run();
	void treatSerial();
	void execute();
	void executeinstr();

	private:
	Period period;
    int serial_count;
	
	std::string s;
	bool treated;
	std::istringstream stream;	
    IO* io;
	// caractere correspondant a une machine a état	
};
#endif
