#ifndef IO_H
#define IO_H

#include "Period.h"
#include "Arduino.h"
#include <Servo.h>
#include "interrupts.h"
#include "pins.h"
class Switch
{
    // class for switches
    // ie bumper + IR
    private:
        int pin;
        bool reversed;
    public:
        Switch(int pin);
        bool is_on();
        bool is_off();
        void reverse();
};

#define HAUT -1
#define BAS 0
#define TIC_HAUT 100
#define TIC_BAS 0

class Ascenseur
{
    private:
        int pin_cmd_mot;
        int pin_dir_mot;
        Switch bumper_asc_bas;
        Switch bumper_asc_haut;
        bool in_asserv;
        int target;

    public:
        Ascenseur();
        bool is_up();
        bool is_low();
        void start_asserv(int target_);
        void monte();
        void descend();
        void run();
        void send_monte();
        void send_maintien_p();
        void send_zeros();
        void send_desc();
};

class ColorSensor 
{
    private:
        Period period_reset; 
        int last_count;
    public:
        ColorSensor();
        bool is_red();
        bool is_yellow();
        void run();
    
};


class Pince
{
    private:
        Servo servo_pince_g;
        Servo servo_pince_d;
        Servo servo_pince_rot;
        Switch ir_feu; 
        Ascenseur asc;
        ColorSensor col;
    public:
        Pince();
        void trigger(int transition);

        void ouvrir_pince();
        void ranger_lateral();
        void serrer_feu_pince();
        void rotation_pince_milieu(); //position de depart
        void rotation_pince_normal();
        void rotation_pince_retourne();

        void run();
        void trigger();

};

class IO
/** (x,y,cap) objects **/
{
    private:
        Servo servo_fresque_g;
        Servo servo_fresque_d;
        Servo servo_filet_g;
        Servo servo_filet_d;
        Pince pince;

    public:
        IO();
        void poser_fresque();
        void ranger_servo_fresque();
        void envoi_filet();
        void ranger_servo_filet();

        void run();
        void trigger(int transition);

};

#endif
