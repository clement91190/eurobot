#ifndef COORD_H
#define COORD_H

#include "Arduino.h"

//#define PI 3.1415926

float diff_cap(float cap1, float cap2);

class Coord
/** (x,y,cap) objects **/
{
    private:
        float x;
        float y;
        float cap; /*radians !*/
    public:
        Coord();
        Coord(float x_, float y_, float cap_);
        float get_x();
        float get_y();
        float get_cap();
        void set_x(float x_);
        void set_y(float y_);
        void set_cap(float cap_);
        void set_x_y_cap(float x_, float y_, float cap_);
        void forward_translation(float d);
        void write_serial();
        void barycentre(Coord coord2, float rapport);
        bool is_on_map();
};

class Vector
{
    private:
        float x;
        float y;
    public:
        Vector();
        Vector(float x, float y);
        Vector(Coord u, Coord v);
        Vector(Coord cap); // use the cap to construct a vector
        float norm();
        void neg();
        void normalize();
        float scalar(Vector u);
        float get_x();
        float get_y();
        void write_serial();
        float get_angle();
};


#endif
