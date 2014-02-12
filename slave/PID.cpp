#include "PID.h"

PID::PID():last_input(0.),
    I_sum(0.),
    target(0.),
    Kp(0.),
    Kd(0.),
    Ki(0.),
    minV(-128),
    maxV(128),
    inAuto(false)
{
}

void PID::setTarget(int target_){
    target = float(target_);
}

void PID::setTuning(float Kp_, float Ki_, float Kd_){
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
}

float PID::Compute(float input){
    if (!inAuto) {return 0.}
    float error = target - input;
    I_sum = I_sum + Ki * error;
    if (I_sum > maxV){
        I_sum = maxV;}
    else if (I_sum < minV){
        I_sum = minV;} 
    float dinput = input - last_input;

    float out = Kp * error + I_sum - Kd * dinput;
    if (out > maxV){
        out = maxV;}
    else if (out < minV){
        out = minV;} 
    last_input = input;
    return out;
} 


void PID::setOutputLimit(int max, int min){
    minV = min;
    maxV = max;
}

void PID::turn_on(bool on_off, float input)
{
  inAuto = on_off;
  init(input)

}

void PID::init(float input)
{
    last_input = input;
    I_sum = 0;
}
