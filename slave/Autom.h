
class Coord
/** (x,y,cap) objects **/
{
    private:
    float x;
    float y;
    float cap;
    public:
    float get_x();
    float get_y();
    float get_cap();
    void set_x(float x_);
    void set_y(float y_);
    void set_cap(float cap_);
    void set_x_y_cap(float x_, float y_, float cap_);

};

class Autom
{
    private:
        Coord real_coord;

    public:
        Coord get_real_coord();
        void send_cmd(cmdG, cmdD); 

};
