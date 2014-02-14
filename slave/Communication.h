#ifndef COM_H
#define COM_H

#include "Period.h"
#include "Arduino.h"
#include "Autom.h"
#include <iterator>
#include <string>

class Communication{

    private:
        Period period_com;
        Autom* slave;

    public:
        Communication(Autom* slave_);
        void treat_orders();
        void run();
};

#endif

