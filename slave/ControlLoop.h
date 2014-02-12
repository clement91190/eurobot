#ifndef CONTROL_H
#define CONTROL_H

#include "Coord.h"
#include "PID.h"

class ControlLoop
{
    private:
        PID pidcap;
        PID piddep;
        int cmd_g;
        int cmd_d;
        bool fw_g;
        bool fw_d;


    public:
        ControlLoop();
        void bf_avance(float d);
        void set_target(Coord coord);
        void update_error(Coord coord);
        int get_cmd_g();
        bool get_fw_g();
        int get_cmd_d();
        bool get_fw_d();
        void run(Coord real_coord);
};



#endif
