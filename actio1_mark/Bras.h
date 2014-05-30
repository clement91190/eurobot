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


#define NEAR 10
//distance to target in steps
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
        int signe;

    public:
        Ascenseur(int pin_step, int pin_dir, int pin_bumper, int signe);
        bool is_up();
        void monte();
        void go_to(float pos_cm);
        bool is_near();
        bool is_arrive();
        void write_debug();
        bool run();
        void stop();
};

//position de l'ascenseur
#define ASC0 0. 
#define ASC1 4.  
#define ASC2 8.6
#define ASC3 19
#define ASC4 22.8


class Bras
{
    private:
        Ascenseur asc;
        Servo servo_rot;
        Servo servo_retourne;
        ColorSensor col;
        //SwitchAnalog pression; // ? 
        bool pression_on;
        double pression
        SwitchAnalog ir;
        int pin_pompe;
        int cote;
        int couleur;

        //state machine
        int state;
        int trigger_attente;
        bool trigger_attente_on;
        int trigger_to_be;
        bool time_out_on;
        long t_over;
        Period period_run;
        bool mon_ir_actif;

        // master order color
        bool coul_to_be_on; // next time in attente -> next_coul<- next_coul_to_be
        bool next_coul_on;
        bool next_coul;
        bool next_coul_to_be;

        //trigger autre bras
        bool trigger_autre_on;
        int trigger_autre;



    public:
        Bras(int cote_, int pin_pap_step, int pin_pap_dir, int pin_bump_asc, int pin_ir, int seuil_ir, long* pulse_color, int pin_pression);
        void run();
        void trigger(int transition);
        void write_debug();
        void set_autre_bras(Bras* autre_bras_);

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
        void scl(); //ranger
        void scv(); //prise verticale
        void scn(); //prise normale
        void sce(); // echange
        void sct(); //torche = prise normale ?
        
        void po(); //pompe ouverte
        void pf(); //pompe fermee

        void stop();
        bool is_in_attente();
        void set_to_be_done(int trigger_attente);
        void active_ir();
        void desactive_ir();
        void call_for_help();
        void prise_copain();

        void set_to_be_next_coul(bool macoul);
        void set_next_coul(bool macoul);
		void set_time_out(int, int);
        void set_couleur(int couleur_);

        void reset_time_out();
        bool is_time_out();
        void in_state_func();

        void trigger_autre_bras(int trigger);
        bool is_trigger_autre_on();
        int get_trigger_autre();
};


// STATES OF MAE
//
#define RANGE_DEPART 0
#define INT_RANGE 1
#define INT2_RANGE 2

#define GO_ATTENTE 24
#define ATTENTE_ACTIF 10
#define DESCENTE_LAT 11
#define DESCENTE_POMPE_LAT 12
#define PRISE_LAT 13
#define MONTE_ECH 14
#define RETOURNE_ECH 15
#define REPLACE_APRES_ECH 16
#define MONTE 17
#define MONTE_VERT 26
#define RANGE_PRISE 18
#define LACHE 19

#define SEND_MINE 20
#define SENDMASTER_PRET 21
#define PRISE_VERT 22
#define MONTE_ECH_VERT 25

#define PRISE_COPAIN 23


//transition
#define TIME_OUT 0
#define T_RANGE 1 
#define T_ACTIF_NOMINAL 2 

#define T_COUL_NOT_OK 3
#define T_COUL_NOT_OK_MASTER 4
#define T_COUL_OK_MASTER 5
#define T_COUL_OK 6
#define T_MON_IR 7
#define T_ASC_ARRIVE 8
#define T_ASC_PRESQUE_ARRIVE 9
#define T_BUMP_HAUT 10
#define T_CAPT_PRESSION_ON 11 // ou master commande
#define T_PRISE_COPAIN 12
#define T_CALL_FOR_HELP 13 
#define T_PRISE_VERT 14
//#define POSE 1 



#endif
