#ifndef COORD_H
#define COORD_H


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
};

#endif
