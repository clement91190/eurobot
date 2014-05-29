#ifndef AUTOM_H
#define AUTOM_H


#include "Period.h"
#include "Arduino.h"
#include "interrupts.h"
#include "pins.h"
#include "Coord.h"
#include "ControlLoop.h"
#include "Camera.h"


class Autom
{
    private:
		//Camera camera;
        Coord real_coord;
        ControlLoop control;
        Period period_update_coords; /* loop to update the position of the robot*/
        Period period_pid_loop; /* loop of the control loop */
        float gain_odo_g;
        float gain_odo_d;
        float gain_inter_odos; /* 1 / dist inter odos */
        int last_ticG;
        int last_ticD;
        
        
        int tic_total_g;
        int tic_total_d;
        float distance_g;
        float distance_d;

    public:
        Autom();
        Coord get_real_coord();
        //Camera* camera_control();
        void send_cmd(); 
        void write_cmd(int cmd_g, int cmd_d, bool fw_g, bool fw_d);
        void update_cap();
        void update_coords();
        void reset_tics_odos();
        void run();
        void stop();
        void setxycap(Coord real_coord);
        void setxycap_no_x(int y, float cap);
        void setxycap_no_y(int x, float cap);
        void setTuningCap(float Kp, float Ki, float Kd );
        void setTuningDep(float Kp, float Ki, float Kd );
        void turn_on_evit();
        ControlLoop* get_control();
        
        float debuggDistance_g();
		float debuggDistance_d();
		int debuggTic_g();
		int debuggTic_d();
		void debuggDistanceInit();
		void debuggTicInit();
        
};

void write_serial_strat();
#endif
