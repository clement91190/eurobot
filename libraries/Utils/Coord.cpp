#include "Coord.h"


float Coord::get_x(){return x;}

float Coord::get_y(){return y;}

float Coord::get_cap(){return cap;}

void Coord::set_x(float x_){x = x_;}

void Coord::set_y(float y_){y = y_;}

void Coord::set_cap(float cap_){
    cap = cap_;

    // cap always within [0: 2* pi]    
   if (cap > 2 * PI)
   {
        cap = cap - 2 * PI;
   }
   if (cap < 0)
   {
        cap = cap + 2 * PI;
   }
}

void Coord::set_x_y_cap(float x_, float y_, float cap_){
 x = x_;
 y = y_;
 cap = cap_;
}

bool Coord::is_on_map(){
    // check if the detected thing is on the map...
    if (x < 0 || x > 3000 || y < 0 || y > 1000)
    {
        return false;
    }
    return true;
}


void Coord::forward_translation(float d){
    x = x + d * cos(cap);
    y = y + d * sin(cap);
}

void Coord::write_serial()
{
    //Serial.print("ticG  :");
   // Serial.print(ticG);
   // Serial.print("  ticD  :");
   // Serial.print(ticD);
    Serial.print(int(x));
    Serial.print(" ");
    Serial.print(int(y));
    Serial.print(" ");
    Serial.println(cap * 180 / PI);
}

Coord::Coord():x(0.0), y(0.0), cap(0.0){}

Coord::Coord(float x_, float y_, float cap_):x(x_), y(y_), cap(cap_){}

void Coord::barycentre(Coord coord2, float rapport){
    // do a barycenter with point coord2
    x = x * (1 - rapport) + coord2.get_x() * rapport;
    y = y * (1 - rapport) + coord2.get_y() * rapport;
    cap = cap * (1 - rapport) + coord2.get_cap() * rapport;

    
}

Vector::Vector():x(0.), y(0.){}

Vector::Vector(Coord u, Coord v)
{
    x = v.get_x() - u.get_x();
    y = v.get_y() - u.get_y();
}

float Vector::norm(){
    return sqrt(x*x + y*y);
}

void Vector::neg(){
    x = -x;
    y = -y;
}

void Vector::normalize(){
    float len = norm();
    x = x / len;
    y = y / len;
}

Vector::Vector(Coord cap):x(cos(cap.get_cap())), y(sin(cap.get_cap())){}

Vector::Vector(float x_, float y_):x(x_), y(y_){}


float Vector::get_x(){return x;}
float Vector::get_y(){return y;}

float Vector::scalar(Vector u){
   return x * u.get_x() + y * u.get_y(); 
}

void Vector::write_serial(){
    Serial.print("vect" );
    Serial.print(x );
    Serial.print(" , " );
    Serial.print(y);
}

float Vector::get_angle(){
    if (x > 0)
    {
        return atan(y / x);
    }
    else if (x < 0 )
    {
        return PI + atan(y/x);
    }
    else if (y > 0)
    {
        return PI / 2;
    }
    else
    {
        return - PI / 2;
    }
}

float diff_cap(float cap1, float cap2)
{
    // un cap est entre -Pi et Pi
    float diff = cap1 - cap2;
    if (diff > PI) 
    {
        return diff - 2 * PI;
    }
    else if (diff < -PI)
    {
        return diff + 2 * PI;
    }
    else
    {
        return diff;
    }
}
