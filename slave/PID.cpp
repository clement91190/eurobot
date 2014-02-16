#include "PID.h"

PID::PID():last_input(0.),
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

PID::PID(float Kp_, float Ki_, float Kd_):
    last_input(0.),
    I_sum(0.),
    target(0.),
    Kp(Kp_),  //Ku = 5.75
    Kd(Ki_),
    Ki(Kd_),
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
    Serial.print("error");
    Serial.print(error);
    Serial.print(" target ");
    Serial.print(target);
    Serial.print(" input ");
    Serial.print(input);
 
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
  init(input);

}

void PID::init(float input)
{
    last_input = input;
    I_sum = 0;
}
