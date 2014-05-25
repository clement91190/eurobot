#ifndef PID_H
#define PID_H

#define MANUAL 0
#define AUTOMATIC 1
#include "Arduino.h"
#include "Coord.h"

//distance to target
#define NEAR 1 
#define FAR 2
#define DONE 3

#define CAP true
#define DEP false

class PID
{
    private: 
        float last_error;
        float I_sum; 
        float target;
        float Kp;
        float Ki;
        float Kd;
        float near_error_value; // pid is in state near if error under near_error_value
        float done_error_value; //pid is finished if stayed k time with error under done_error_value

        int minV;
        int maxV;
        bool inAuto;
        bool type_cap; // specific pid for angles (always within[0: 2*PI])
        int arrival_count; // to see when it's over
        int pid_state; //see define distance to target
        
    public:
        PID();
        PID(bool type_cap_, float Kp_, float Ki_, float Kd_, float near_error_value_, float done_error_value_);
        void setTuning(float Kp_, float Ki_, float Kd_);
        void setTarget(float target_);
        void addToTarget(float dep);
        float compute(float input);
        void setOutputLimit(int max, int min);
        void turn_on(bool on_off, float input);
        void init(float input);
        bool check_if_over(int pid_state);
        void update_pid_state();
        void set_pid_state(int state);
        void reinit();
        void setMinMax(int v);
        float get_target();
        void write_debug();

};
#endif
