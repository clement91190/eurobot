#ifndef CONTROL_H
#define CONTROL_H

#include "Coord.h"
#include "Sonar.h"
#include "PID.h"
#include "pins.h"



// type of BF
#define STOP 0
#define BFFW 1
#define BFCAP 2
#define BFXYCAP 3
     
//#define GAIN_KP_DEP 3.45
//#define GAIN_KD_DEP 4.6
//#define GAIN_KI_DEP 0.6468
//

#define SLOW 0
#define MEDIUM 1
#define FAST 2


#ifdef PMI

#define GAIN_KP_DEP 3 * 0.66 
#define GAIN_KI_DEP 0.15 * 0.66 //0.35
#define GAIN_KD_DEP 0.1 * 0.66//10. //95 //0.25 //0.2
#define NEAR_ERROR_DEP 70.0
#define DONE_ERROR_DEP  10.0


#define GAIN_KP_CAP 250.0 * 0.66  //Ku = 260
#define GAIN_KI_CAP 3 * 0.66 //52.0 //70.0
#define GAIN_KD_CAP 5 * 0.66 //18.0 //40.0
#define NEAR_ERROR_CAP 8.0 * 3.14 / 180.0 
#define DONE_ERROR_CAP  4.0 * 3.14 / 180.0 

#else

#define GAIN_KP_DEP 0.75 
#define GAIN_KI_DEP 0.03 
#define GAIN_KD_DEP 0.06 
#define NEAR_ERROR_DEP 70.0
#define DONE_ERROR_DEP  20.0


#define GAIN_KP_CAP 300  //Ku = 260
#define GAIN_KI_CAP 0. //52.0 //70.0
#define GAIN_KD_CAP 0.5 //18.0 //40.0
#define NEAR_ERROR_CAP 8.0 * 3.14 / 180.0 
#define DONE_ERROR_CAP  4.0 * 3.14 / 180.0 

#endif

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
        int count_not_moving;
        Coord late_pos; // to check how much we moved lately
        Sonar sonarg; //et le droit ??
#ifndef PMI        
        Sonar sonard;
#endif


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
        void check_blockage();
        void check_adversary();
        void setxycap(Coord new_coord);
        void write_real_coords();
        void write_debug();
        void set_speed(int speed);
        void recaler();
        void setTuningCap(float Kp, float Ki, float Kd );
        void setTuningDep(float Kp, float Ki, float Kd );
       
};



#endif
