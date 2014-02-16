#ifndef _ORDERSRASPBERRY_H
#define _ORDERSRASPBERRY_H
#include "Period.h"
#include "Autom.h"
#include "Arduino.h"
//#include <stdio.h>
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
	OrdersRaspberry(Autom* slave_);
	void run();
	void treatSerial();
	void execute();
	void executeinstr();

	private:
	Period period;
	
	std::string s;
	bool treated;
	std::istringstream stream;	
    Autom* slave;
	// caractere correspondant a une machine a état	
};
#endif
