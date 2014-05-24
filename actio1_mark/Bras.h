#ifndef BRAS_H
#define BRAS_H

#include <Period.h>
#include "Arduino.h"
#include <Servo.h>
#include <AccelStepper.h>
#include "interrupts.h"
#include "pins.h"
#include <Switch.h>
#include <SwitchAnalog.h>
#include <ColorSensor.h>


#define NEAR 10 //distance to target in steps
#define GAUCHE -1 
#define DROITE 1 
#define MICROSTEPPING 2
#define RAPPORTCM 28.571
#define SEUIL_IR_G 220
#define SEUIL_IR_D 220
#define SEUIL_IR_C 200

class Ascenseur
{
    private:
        AccelStepper pap;
        Switch bumper_asc_haut;
        bool recal_up;
        bool recal_up_to_be;

    public:
        Ascenseur(int pin_step, int pin_dir, int pin_bumper)
        is_up();
        monte();
        go_to(float pos_cm);
        void write_debug();
        bool run();
}

//position de l'ascenseur
#define ASC0 0. 
#define ASC1 4.  
#define ASC2 7.
#define ASC3 19
#define ASC4 22.8


class Bras
{
    private:
        Ascenseur asc;
        Servo servo_rot;
        Servo servo_retourne;
        ColorSensor color_sensor;
        SwitchAnalog pression; // ? 
        SwitchAnalog ir;
        int pin_pompe;
        int cote;

    public:
        Bras(int cote);
        void run();
        void trigger(int transition);
        void write_debug();
        void set_autre_bras(Bras* autre_bras);

        //all movement methods

        void a0(); // asc  haut = ranger
        void a1(); // attente
        void a2(); // echange_haut
        void a3(); // echange_bas
        void a4(); // chooppe bas
    
        void spb(); //bas
        void spr(); //retourne
        void spv(); // vertical

        void scr(); //ranger
        void scv(); //prise verticale
        void scn(); //prise normale
        void sce(); // echange
        void sct(); //torche = prise normale ?
        
        void po(); //pompe ouverte
        void pf(); //pompe fermee

};


// STATES OF MAE
//
#define RANGE_DEPART 0
#define INT_RANGE 1
#define INT2_RANGE 2
#define ATTENTE_ACTIF 3

#define DESCENTE_LAT 4
#define DESCENTE_POMPE_LAT 4
#define PRISE_LAT 5
#define MONTE_ECH 6
#define RETOURNE_ECH 7
#define REPLACE_APRES_ECH 8
#define MONTE 9
#define RANGE_PRISE 10
#define LACHE 11

#define SEND_MINE 11
#define SENDMASTER_PRET 12
#define PRISE_VERT 14

#define PRISE_COPAIN 15


//transition
#define TIME_OUT 0
#define T_RANGE 1 
#define T_ACTIF_NOMINAL 2 

#define T_PRISE_VERTICAL 3
#define T_COOL_NOT_OK 4
#define T_COOL_NOT_OK_MASTER 4
#define T_COOL_OK_MASTER 5
#define T_MON_IR 6
#define T_ASC_ARRIVE 7
#define T_ASC_PRESQUE_ARRIVE 8
#define T_BUMP_HAUT 9
#define T_CAPT_PRESSION_ON 10 
#define T_PRISE_COPAIN 11 
//#define POSE 1 


#define ROUGE 0
#define JAUNE 0

#endif
