#ifndef CONTROL_H
#define CONTROL_H

#include "Coord.h"
#include "PID.h"

class ControlLoop
{
    private:
        PID pidcap;
        PID piddep;


    public:
        void set_target(Coord coord);
        void update_error(Coord coord);
        int get_cmd_fw();
        int get_cmd_rot();
};



#endif
