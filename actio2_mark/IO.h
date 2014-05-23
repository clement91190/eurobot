#ifndef IO_H
#define IO_H

#include <Period.h>
#include <Switch.h>
#include "Arduino.h"
#include <Servo.h>
#include "pins.h"


#define PILE_AVD_OPEN 1100
#define PILE_AVD_CLOSE 1540  
#define PILE_ARD_OPEN 860
#define PILE_ARD_CLOSE  530

#define PILE_AVG_OPEN 1040
#define PILE_AVG_CLOSE 480
#define PILE_ARG_OPEN 1100
#define PILE_ARG_CLOSE 1460

#define TACLE_G_OPEN 1500
#define TACLE_G_CLOSE 600
#define TACLE_G_HALF 900

#define TACLE_D_OPEN 1020
#define TACLE_D_CLOSE 1800
#define TACLE_D_HALF 1500

#define CANON_D_OPEN 1600
#define CANON_D_CLOSE 1840
#define CANON_D_HALF 1750

#define CANON_G_OPEN 1500
#define CANON_G_CLOSE 1700
#define CANON_G_HALF 1600

#define BENNE_G_OPEN 1500
#define BENNE_G_CLOSE 1930
#define BENNE_G_HALF 1720

#define BENNE_D_OPEN 1400
#define BENNE_D_CLOSE 930
#define BENNE_D_HALF 1180

#define BENNE_C_OPEN 1900
#define BENNE_C_CLOSE 1300

#define GAUCHE 0
#define DROITE 1

#define SEUIL_IR 350 //truc si sup a


// STATES OF MAE PILE
#define ETAT_ATTENTE 0
#define ETAT_EJECTION 1
#define ETAT_VIDANGE_STANDBY 2
#define ETAT_VIDANGE_OUVERTURE 3
#define ETAT_VIDANGE_FERMETURE 4

//transition MAE PILE
#define TIME_OUT 0
#define EJECTION 1
#define VIDANGE 2
#define PILE_VIDE 3
#define PILE_PLEINE 4
#define STOP 5


class Pile
/** pile de stockage objects **/
{
	private:
		Servo servo_pile_av;
        Servo servo_pile_ar;
        
        int val_ir;
        int pile_cote;
        long t_over;
        bool time_out_on;
        int state;
        Period period_run;
        int getCote();
        void open();
        void close();
        

	public:

		Pile(int cote);
        int estPleine(); 	//1 si qqchose sinon 0
        void etatMaster();	//P4 G P5 D
        //void ejection(); 	//P0 G P1 D
        //void vidange();		//P2 G P3 D
        void in_state_func();
        void run();
        void trigger(int transition);
        void set_time_out(int dt_);
        void reset_time_out();
        bool is_time_out();
	
};

class IO
/** (x,y,cap) objects **/
{
    private:
		Pile pile_gauche;
		Pile pile_droite;
    
        Servo servo_benne_c;
        Servo servo_benne_g;
        Servo servo_benne_d;

        Servo servo_rampe_g;
        Servo servo_rampe_d;

        Servo servo_tacle_g;
        Servo servo_tacle_d;

        
    public:
        IO();
        void run();
        void pile_gauche_trigger(int transition);
        void pile_droite_trigger(int transition);
        void pile_gauche_etatMaster();
        void pile_droite_etatMaster();
        
        //tacle droite
        void tacle_droite_fermeture();		//T0
        void tacle_droite_miOuverture();	//T1
        void tacle_droite_ouverture();		//T2
        
        //tacle gauche
        void tacle_gauche_fermeture();		//T3
        void tacle_gauche_miOuverture();	//T4
        void tacle_gauche_ouverture();		//T5
        
        //Canon droit
        void canon_droite_enclenche();		//C0
        void canon_droite_tir();			//C1
        void canon_droite_repos();			//C2
        
        //Canon gauche
        void canon_gauche_enclenche();		//C3
        void canon_gauche_tir();			//C4
        void canon_gauche_repos();			//C5
        
        //Benne & rateaux
        void benne_centrale_close();		//R0
        void benne_centrale_gerbe();		//R1
        void benne_rateau_bas();			//R2
        void benne_rateau_mileu();			//R3	
        void benne_rateau_haut();			//R4
        
         

};

#endif
