#ifndef CONTROL_H
#define CONTROL_H

#include "Coord.h"
#include "PID.h"



// type of BF
#define STOP 0
#define BFFW 1
#define BFCAP 2
#define BFXYCAP 3
     
//#define GAIN_KP_DEP 3.45
//#define GAIN_KD_DEP 4.6
//#define GAIN_KI_DEP 0.6468

#define GAIN_KP_DEP 3
#define GAIN_KI_DEP 0. //0.35
#define GAIN_KD_DEP 1. //0.25 //0.2
#define NEAR_ERROR_DEP 70.0
#define DONE_ERROR_DEP  10.0


#define GAIN_KP_CAP 120.0  //Ku = 260
#define GAIN_KI_CAP 0. //52.0 //70.0
#define GAIN_KD_CAP 0.5 //18.0 //40.0
#define NEAR_ERROR_CAP 8.0 * 3.14 / 180.0 
#define DONE_ERROR_CAP  4.0 * 3.14 / 180.0 


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
        int asserv_state; // see define in PID -> FAR, NEAR
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
        void next_asserv_state();
};



#endif
