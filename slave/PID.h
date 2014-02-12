#ifndef CONTROL_H
#define CONTROL_H

#define MANUAL 0
#define AUTOMATIC 1

class PID
{
    private 
        float last_input;
        float I_sum; 
        float target;
        float Kp;
        float Ki;
        float Kd;
        int minV;
        int maxV;
        bool inAuto;
        
    public:
        PID();
        void setTuning(float Kp_, float Ki_, float Kd_);
        void setTarget(int target_);
        float Compute(float input);
        void setOutputLimit(int max, int min);
        void turn_on(bool on_off, input=input);
        void init(float input);

}
#endif
