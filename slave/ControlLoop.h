#ifndef CONTROL_H
#define CONTROL_H

#include "Coord.h"
#include "PID.h"



#define STOP 0
#define BFFW 1
#define BFCAP 2
#define BFXYCAP 3

    
#define GAIN KP_DEP 3.45
#define GAIN KD_DEP 4.6
#define GAIN KI_DEP 0.6468

#define GAIN KP_CAP 1.0
#define GAIN KD_CAP 0.
#define GAIN KI_CAP 0.

class ControlLoop
/* This class gets the updated coordinate, and talk to the 2 PID
 * loops to tell the robot where to go.
 * This class also talk to the Raspberry Pi to say when the movement is finished*/
{
    private:
        Coord real_coord;
        Coord target_position;
        Vector dir;
        int bf_type; // see define 
        PID pidcap;
        PID piddep;
        int cmd_g;
        int cmd_d;
        int cmd_dep;
        int cmd_cap;
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
        void run(Coord real_coord_);
        void compute_pids();
        void compute_cmds();
        void set_BF(int bf_type_, Coord target_position_);
};



#endif
