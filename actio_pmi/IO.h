#ifndef IO_H
#define IO_H

#include "Arduino.h"
#include "interrupts.h"
class Switch
{
    // class for switches
    // ie bumper + IR
    private:
        int pin;
        bool reversed;
    public:
        Switch();
        bool is_on();
        void reverse();
}

class Ascenseur
{
    private:
        int pin_cmd_mot;
        int pin_dir_mot;
        Switch bumper_asc_bas;
    public:
        Ascenseur(pin_cmd_mot_, pin_dir_mot_, pin_bumper);
        bool is_up();
        bool is_done();
        void monte();
        void descend();
}

class ColorSensor 
{
    private:
    public:
        CapteurCouleur();
        void is_red();
        void is_yellow();
    
}

class IO
/** (x,y,cap) objects **/
{
    private:
        Servo servo_pince_g;
        Servo servo_pince_d;
        Servo servo_pince_rot;
        Servo servo_fresque_g;
        Servo servo_fresque_d;
        Servo servo_filet;

        Switch ir_feu; 
        Ascenseur asc;
        ColorSensor col;


    public:
        IO();
        void monter_asc();
        void descendre_asc();

        void ouvrir_pince();
        void serrer_feu_pince();
        void rotation_pince_milieu(); //position de depart
        void rotation_pince_gauche();
        void rotation_pince_droite();


        void poser_fresque();
        void envoi_filet();

};

#endif
