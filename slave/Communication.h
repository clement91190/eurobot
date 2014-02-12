#ifndef COM_H
#define COM_H

#include "Period.h"
#include "Arduino.h"
#include "Autom.h"

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

