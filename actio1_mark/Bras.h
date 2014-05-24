#ifndef BRAS_H
#define BRAS_H

#include <Period.h>
#include "Arduino.h"
#include <Servo.h>
#include "interrupts.h"
#include "pins.h"
#include <Switch.h>
#include <ColorSensor.h>

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
        void monte(); //A1
        void descend(); //A0
        void run();
        void send_monte();  
        void send_maintien_p();
        void send_zeros();
        void send_desc();
        void write_debug();
};


// STATES OF MAE
//
#define RANGE_DEPART 0
#define TAPE_VERTICAL 1
#define INTERMEDIAIRE_SORTIE 2
#define INT_RANGE 3
#define INT2_RANGE 4

#define HORIZ_VERT 10
#define NOMINAL_VIDE 11 
#define ACTIF_TORCHE 12
#define PRISE_TORCHE 13
#define ACTIF_FEU 14
#define PRISE_FEU 15
#define WAIT_COOL_OK 16
#define MONTE_COOL 17
#define MONTE_COOL2 24
#define RETOURNE_COOL_N 18
#define DESCEND_COOL_N 19
#define POSE_COOL_N 20
#define INT1_COOL_N 21 
#define INT2_COOL_N 22 
#define INT3_COOL_N 23

//transition
#define TIME_OUT 0
#define POSE 1 
#define COOL_NOT_OK 2
#define COOL_OK 3
#define T_BAS 4
#define T_IR 5
#define TAPE 6
#define RANGE 7 
#define T_ACTIF_FEU 8  
#define T_ACTIF_TORCHE 9
#define T_BUMP_HAUT 10


#define ROUGE 0
#define JAUNE 0

class Pince
{
    private:
        Servo servo_pince_g;
        Servo servo_pince_d;
        Servo servo_pince_rot;
        Switch ir_feu; 
        Ascenseur asc;
        ColorSensor col;
        long t_over;
        bool time_out_on;
        int trigger_to_be;
        int state;
        int couleur;
        Period period_run;
    public:
        Pince();

        void slo(); // servo lateral ouvert
        void slr(); // serve lateral ranger
        void slf(); // servo lateral ferme
        void slt(); // servo lateral tape

        void scv(); //servo central vertical
        void scn(); // servo central normal
        void scr(); // servo central retourne

        void a0();
        void a1();

        void run();
        void in_state_func();
        void trigger(int transition);
        void set_time_out(int dt_, int trigger);
        void reset_time_out();
        bool is_time_out();
        void write_debug();



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
        int couleur;

    public:
        IO();
        void poser_fresque();
        void ranger_servo_fresque();
        void envoi_filet();
        void ranger_servo_filet();
        void write_debug();

        void run();
        void trigger(int transition);

};

#
