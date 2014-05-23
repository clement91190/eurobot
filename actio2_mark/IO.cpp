#include "IO.h"
#include <string.h>
// IO
// IO Mark Zero
//


//constructeur des piles de stockages de feux latéraux
Pile::Pile(int cote):time_out_on(false), state(ETAT_ATTENTE), period_run(50)
{
	this->pile_cote = cote;
	if(pile_cote == GAUCHE)
	{
		pinMode(PIN_IRG,INPUT);
		servo_pile_av.attach(PIN_SERVO_PILE_AVG);
		servo_pile_ar.attach(PIN_SERVO_PILE_ARG);
		servo_pile_av.writeMicroseconds(PILE_AVG_CLOSE);
		servo_pile_ar.writeMicroseconds(PILE_ARG_CLOSE);
	}
	else
	{
		pinMode(PIN_IRD,INPUT);
		servo_pile_av.attach(PIN_SERVO_PILE_AVD);
        servo_pile_ar.attach(PIN_SERVO_PILE_ARD);
        servo_pile_av.writeMicroseconds(PILE_AVD_CLOSE);
		servo_pile_ar.writeMicroseconds(PILE_ARD_CLOSE);
	}
	val_ir = 0;
	
}

//regarde le cote de la pile
int Pile::getCote()
{
	return this->pile_cote;
}



//fermeture des taclettes de piles
void Pile::close()
{
	if(pile_cote == GAUCHE)
	{
		servo_pile_av.writeMicroseconds(PILE_AVG_CLOSE);
		servo_pile_ar.writeMicroseconds(PILE_ARG_CLOSE);
	}
	else
	{
        servo_pile_av.writeMicroseconds(PILE_AVD_CLOSE);
		servo_pile_ar.writeMicroseconds(PILE_ARD_CLOSE);
	}
}

//ouverture des taclettes de piles
void Pile::open()
{
	if(pile_cote == GAUCHE)
	{
		servo_pile_av.writeMicroseconds(PILE_AVG_OPEN);
		servo_pile_ar.writeMicroseconds(PILE_ARG_OPEN);
	}
	else
	{
        servo_pile_av.writeMicroseconds(PILE_AVD_OPEN);
		servo_pile_ar.writeMicroseconds(PILE_ARD_OPEN);
	}
}


//regarde si il reste qqchose dans la pile
int Pile::estPleine()
{
	if(pile_cote == GAUCHE)
	{
		val_ir = analogRead(PIN_IRG);
	}
	else
	{
        val_ir = analogRead(PIN_IRG);
	}
	int k = 0;
	if(val_ir > SEUIL_IR)
	{
		k = 1;
	}
	return k;
}


void Pile::etatMaster()
{
	String debut = "* PILE ";
	String cote;
	String fin ;
	if(pile_cote == GAUCHE)
	{
		cote = "G ";
	}
	else
	{
		cote = "D ";
	}
	if(this->estPleine())
	{
		fin = "1";
	}
	else {
		fin = "0";
	}
	//String fin =num2str(this->estPleine());
	String message  = debut + cote + fin;
	Serial.println(message);
}


//transition d'etats
void Pile::trigger(int transition)
{
 // add things here for MAE
 //
 //
 //
     if (transition == EJECTION || transition == VIDANGE )
    {
         Serial.print("new trigger to be ");
         Serial.println(transition);
        trigger_to_be = transition;
    }


  int old_state;
  old_state = state;
   switch(state)
    {
		//etat d'attente/repos  avant transistion
        case ETAT_ATTENTE :
           if (transition == EJECTION)
           {
            state = ETAT_EJECTION;
           }
           else if (transition == VIDANGE)
           {
            state = ETAT_VIDANGE_STANDBY;
           }
           break;
           
         //etat d'ejection simple
         case ETAT_EJECTION :
           if (transition == TIME_OUT)
           {
            state = ETAT_ATTENTE;
           }
           break;
           
           //etat de check de pile
        case ETAT_VIDANGE_STANDBY :
            if (transition == PILE_VIDE)
            {
                state = ETAT_ATTENTE;
            }
            else if (transition == PILE_PLEINE)
            {
                state = ETAT_VIDANGE_OUVERTURE;
            }
            break;
            
            //etat d'ejection programme
        case ETAT_VIDANGE_OUVERTURE :
            if (transition == TIME_OUT)
            {
                 state = ETAT_VIDANGE_FERMETURE;
            }
            break;
            
            //etat de refermeture
        case ETAT_VIDANGE_FERMETURE :
            if (transition == TIME_OUT)
                {
                    state = ETAT_VIDANGE_STANDBY;
                }
            break;
          
    }
   
   if (old_state != state)
    {
        Serial.print("new state: ");
        Serial.println(state);

        reset_time_out();
        in_state_func();
    }
}

void Pile::set_time_out(int dt_, int trigger)
{
    t_over = millis() + dt_;
    time_out_on = true;
    trigger_to_be = trigger;
    Serial.print("time_out for trigger ");
    Serial.println(trigger);
}

//reset les times out
void Pile::reset_time_out()
{
    time_out_on = false;
}

bool Pile::is_time_out()
{
   if (time_out_on && t_over < millis())
   {
     time_out_on = false;
     return true;
   }
   return false;
}

//execution des etats
void Pile::in_state_func()
{
    switch (state)
    {
        case ETAT_ATTENTE :
            close();
            break;
            
        case ETAT_EJECTION :
			set_time_out(500, TIME_OUT);
            open();
            break;
            
        case ETAT_VIDANGE_STANDBY :
            close();
            //check ir
            break;
            
        case ETAT_VIDANGE_OUVERTURE :
			set_time_out(500, TIME_OUT);
            open();
            break;
            
        case ETAT_VIDANGE_FERMETURE : 
            set_time_out(300, TIME_OUT);
            close();
            break;
    }
}

void Pile::run(){
 
    if (period_run.is_over())
    {
        period_run.reset();
        if (estPleine() == 1)
        {
             trigger(PILE_PLEINE);
        }
            
        if (estPleine() == 0)
        {
             trigger(PILE_VIDE);
        }
        
        if (is_time_out())
        {
            trigger(trigger_to_be);
        }
        
		//depuis instruction PI, a verif
        //trigger(EJECTION);
        //trigger(VIDANGE);
        
            
      
        
    }
}



//definition des IO
IO::IO(): pile_gauche(GAUCHE), pile_droite(DROITE)
{

    servo_benne_c.attach(PIN_SERVO_BENNE_C);
    servo_benne_g.attach(PIN_SERVO_BENNE_G);
    servo_benne_d.attach(PIN_SERVO_BENNE_D);

    servo_rampe_g.attach(PIN_SERVO_RAMPE_G);
    servo_rampe_d.attach(PIN_SERVO_RAMPE_D);

    servo_tacle_g.attach(PIN_SERVO_TACLE_G);
    servo_tacle_d.attach(PIN_SERVO_TACLE_D);
    
    tacle_gauche_fermeture();
    tacle_droite_fermeture();
    canon_droite_enclenche();
    canon_gauche_enclenche();
    benne_centrale_close();
    benne_rateau_bas();

}


//tacle droite
void IO::tacle_droite_fermeture(){
	servo_tacle_d.writeMicroseconds(TACLE_D_CLOSE);
}
void IO::tacle_droite_miOuverture(){
	servo_tacle_d.writeMicroseconds(TACLE_D_HALF);
}
void IO::tacle_droite_ouverture(){
	servo_tacle_d.writeMicroseconds(TACLE_D_OPEN);
}
        
//tacle gauche
void IO::tacle_gauche_fermeture(){
	servo_tacle_g.writeMicroseconds(TACLE_G_CLOSE);
}
void IO::tacle_gauche_miOuverture(){
	servo_tacle_g.writeMicroseconds(TACLE_G_HALF);
}
void IO::tacle_gauche_ouverture(){
	servo_tacle_g.writeMicroseconds(TACLE_G_OPEN);
}
        
//Canon droit
void IO::canon_droite_enclenche()
{
	servo_rampe_d.writeMicroseconds(CANON_D_CLOSE);
}
void IO::canon_droite_tir(){
	servo_rampe_d.writeMicroseconds(CANON_D_OPEN);
}
void IO::canon_droite_repos(){
	servo_rampe_d.writeMicroseconds(CANON_D_HALF);
}
        
//Canon gauche
void IO::canon_gauche_enclenche()
{
	servo_rampe_g.writeMicroseconds(CANON_G_CLOSE);
}
void IO::canon_gauche_tir(){
	servo_rampe_g.writeMicroseconds(CANON_G_OPEN);
}
void IO::canon_gauche_repos(){
	servo_rampe_g.writeMicroseconds(CANON_G_HALF);
}


//Benne & rateaux
void IO::benne_centrale_close()
{
	servo_benne_c.writeMicroseconds(BENNE_C_CLOSE);
}
void IO::benne_centrale_gerbe()
{
	servo_benne_c.writeMicroseconds(BENNE_C_CLOSE);
}
void IO::benne_rateau_bas()
{
	servo_benne_g.writeMicroseconds(BENNE_G_CLOSE);
	servo_benne_d.writeMicroseconds(BENNE_D_CLOSE);
}
void IO::benne_rateau_mileu()
{
	servo_benne_g.writeMicroseconds(BENNE_G_HALF);
	servo_benne_d.writeMicroseconds(BENNE_D_HALF);
}
void IO::benne_rateau_haut()
{
	servo_benne_g.writeMicroseconds(BENNE_G_OPEN);
	servo_benne_d.writeMicroseconds(BENNE_D_OPEN);
}


//run 
void IO::run()
{
	pile_gauche.run();
	pile_droite.run();
	
}

//trigger pour le MAE pile gauche
void IO::pile_gauche_trigger(int transition)
{
	pile_gauche.trigger(transition);
}

//trigger pour le MAE pile droite
void IO::pile_droite_trigger(int transition)
{
	pile_droite.trigger(transition);
}


void IO::pile_gauche_etatMaster()
{
	pile_gauche.etatMaster();
}
void IO::pile_droite_etatMaster()
{
	pile_droite.etatMaster();
}
