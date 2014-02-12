#ifndef PID_H
#define PID_H

#define MANUAL 0
#define AUTOMATIC 1

class PID
{
    private: 
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
        void setTarget(float target_);
        void addToTarget(float dep);
        float compute(float input);
        void setOutputLimit(int max, int min);
        void turn_on(bool on_off, float input);
        void init(float input);

};
#endif
