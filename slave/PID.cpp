#include "PID.h"

PID::PID():last_error(0.),
    I_sum(0.),
    target(0.),
    Kp(3.45),  //Ku = 5.75
    Kd(4.6),
    Ki(0.6468),
    minV(-255),
    maxV(255),
    inAuto(true)
{
}

PID::PID(float Kp_, float Ki_, float Kd_, float near_error_value_, float done_error_value_):
    last_error(0.),
    I_sum(0.),
    target(0.),
    Kp(Kp_),  //Ku = 5.75
    Kd(Ki_),
    Ki(Kd_),
    near_error_value(near_error_value_),
    done_error_value(done_error_value_),
    minV(-255),
    maxV(255),
    inAuto(true)
{
}

void PID::setTarget(float target_){
    target = target_;
}

void PID::addToTarget(float dep){
    target = target + dep;
}

void PID::setTuning(float Kp_, float Ki_, float Kd_){
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
}

float PID::compute(float input){
    if (!inAuto) {return 0.;}
    float error = target - input;
    //Serial.print("error");
    //Serial.print(error);
   // Serial.print(" target ");
    //Serial.print(target);
    //Serial.print(" input ");
    //Serial.print(input);
 
    I_sum = I_sum + Ki * error;
    //saturation of I term
    //if (I_sum > maxV){
    //    I_sum = maxV;}
    //else if (I_sum < minV){
    //    I_sum = minV;} 
    // reset integrator in case of not agree with prop
    //
    //if (I_sum * error < 0)
    //{
    //    I_sum = 0.;
    //}
    float derror = error - last_error;
    last_error = error;
    float out = Kp * error + I_sum + Kd * derror;
    if (out > maxV){
        out = maxV;
        I_sum = I_sum - Ki * error;}
    else if (out < minV){
        out = minV;
        I_sum = I_sum - Ki * error;}
    return out;
} 

void PID::set_pid_state(int state)
{
    pid_state = state;
}

void PID::reinit()
{
    pid_state = FAR;
}

void PID::update_pid_state()
{
    switch(pid_state){
        case FAR:
            if (last_error < near_error_value)
            {
                pid_state = NEAR;
            }
            break;
        case NEAR:
            if (last_error < done_error_value)
            {
                arrival_count += 1;
            }
            else
            {
                if (last_error > near_error_value)
                {
                    arrival_count = 0;
                }
            }
            if (arrival_count> 10)
            {
                pid_state = DONE; 
            }
            break;
    }    

}

bool PID::check_if_over(int asserv_state)
{
    /* check if the pid is over or near ( for waypoints ) 
     *-> return true if pid_state is better than asserv_state
     * */
    
    return pid_state != asserv_state;

}

void PID::setOutputLimit(int max, int min){
    minV = min;
    maxV = max;
}

void PID::turn_on(bool on_off, float input)
{
  inAuto = on_off;
  init(input);

}

void PID::init(float input)
{
    last_error = 0;
    I_sum = 0;
    reinit();
}
