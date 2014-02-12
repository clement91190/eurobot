#include "Coord.h"

float Coord::get_x(){return x;}

float Coord::get_y(){return y;}

float Coord::get_cap(){return cap;}

void Coord::set_x(float x_){x = x_;}

void Coord::set_y(float y_){y = y_;}

void Coord::set_cap(float cap_){cap = cap_;}

void Coord::set_x_y_cap(float x_, float y_, float cap_){
 x = x_;
 y = y_;
 cap = cap_;
}

void Coord::forward_translation(float d){
    x = x + d * cos(cap);
    y = y + d * sin(cap);
}

void Coord::write_serial()
{
    Serial.print("ticG  :");
    Serial.print(ticG);
    Serial.print("  ticD  :");
    Serial.print(ticD);
    Serial.print("  x  :");
    Serial.print(int(x));
    Serial.print("  y :");
    Serial.print(int(y));
    Serial.print("  cap:  ");
    Serial.println(cap * 360 / 3.14);
}

Coord::Coord():x(0.0), y(0.0), cap(0.0){}

Coord::Coord(float x_, float y_, float cap_):x(x_), y(y_), cap(cap_){}

