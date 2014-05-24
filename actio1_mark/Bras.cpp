#include "Bras.h"

Ascenseur::Ascenseur(int pin_step, int pin_dir, int pin_bumper, int signe):
    pap(1, pin_step, pin_dir);
    bumper_asc_haut(pin_bumper), recal_up(false)
{
    bumper_asc_haut.reverse();
    pap.setMaxSpeed(3000);
    pap.setAcceleration(200);
  
}

bool Ascenseur::is_up()
{
    return bumper_asc_haut.is_on();    
}

void Ascenseur::monte()
{
    recal_up = true;
}

void Ascenseur::stop()
{
    pap.setCurrentPosition(0);
    pap.moveTo(0);
    pap.run();

}

bool Ascenseur::run()
{
    if (recal_up)
    {
        if (is_up())
        {
            pap.setCurrentPosition(0);
            pap.moveTo(0);
            return true;
        }
        else
        {
            pap.move(-10 * signe);
            return false;
        }
    }

    bool fini;
    fini = pap.run();
    if (fini && recal_up_to_be)
    {

    recal_up_to_be = false;
        if (!is_up())
        {
            recal_up = true;
            return false;
        }
        else 
        {
            return true;
        }
    }
}

bool Ascenseur::is_near()
{
    return (abs(asc.distanceToGo()) <= NEAR);
}

void Ascenseur::go_to(float pos_cm){
    recal_up = false;
    if (pos_cm == 0.)
    {
        recal_up_to_be = true;
    }
    asc.moveTo(signe * int(RAPPORTCM * pos_cm / MICROSTEPPING));
}


void Ascenseur::write_debug()
{
    Serial.print(" bumper haut: ");
    Serial.println(bumper_asc_haut.is_on());

    Serial.println(" PAP :");
    Serial.print( "current pos : ")
    Serial.print(asc.currentPosition());

    Serial.print( " target pos : ")
    Serial.print(asc.targetPosition());
    
    Serial.print( "recal haut ? ")
    Seria.println( recal_up);
}



//BRAS
//

Bras::Bras(int cote_): period_run(50), time_out_on(false), state(RANGE_DEPART)
{

    cote = cote;_
    if (cote == GAUCHE)
    {
        servo_rot.attach(PIN_SERVO_ROT_G);
        servo_retourne.attach(PIN_SERVO_RETOURNE_G);
        asc = Ascenseur(PIN_PAP_STEP_G, PIN_PAP_DIR_G, PIN_BUMPER_ASC_H_G) ;
        color_sensor(&pulse_color_g);
        ir = SwitchAnalog(PIN_IR_G, SEUIL_IR_G)
        pin_pompe = PIN_POMPE_G;
        pinMode(pin_pompe, OUTPUT);
    }
    else
    {
        servo_rot.attach(PIN_SERVO_ROT_D);
        servo_retourne.attach(PIN_SERVO_RETOURNE_D);
        asc = Ascenseur(PIN_PAP_STEP_D, PIN_PAP_DIR_D, PIN_BUMPER_ASC_H_D) ;
        color_sensor(&pulse_color_d);
        ir = SwitchAnalog(PIN_IR_C, SEUIL_IR_D)
        pin_pompe = PIN_POMPE_D;
        pinMode(pin_pompe, OUTPUT);
    }

    scn();
    pf();
    asc.monte();
    while(!asc.run()){
        delay(1);
    }
    scr();
 }

void Bras::po()
{
    digitalWrite(pin_pompe, LOW);
}

void Bras::pf()
{
    digitalWrite(pin_pompe, LOW);
}

void Bras::a0() // asc  haut = ranger
{
    asc.go_to(ASC0);
}


void Bras::a1() // asc  haut = ranger
{
    asc.go_to(ASC1);
}


void Bras::a2() // asc  haut = ranger
{
    asc.go_to(ASC2);
}


void Bras::a3() // asc  haut = ranger
{
    asc.go_to(ASC3);
}


void Bras::a4() // asc  haut = ranger
{
    asc.go_to(ASC4);
}

void Bras::spb()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(500); 
    }
    else
    {
        servo_retourne.writeMicroseconds(2050); 
    }
}


void Bras::spv()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(1400); 
    }
    else
    {
        servo_retourne.writeMicroseconds(1300); 
    }

}


void Bras::spr()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(2200); 
    }
    else
    {
        servo_retourne.writeMicroseconds(500); 
    }
}

void Bras::scr()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(2200); 
    }
    else
    {
        servo_retourne.writeMicroseconds(600); 
    }
}


void Bras::scv()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(700); 
    }
    else
    {
        servo_retourne.writeMicroseconds(2200); 
    }
}


void Bras::scn()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(1000); 
    }
    else
    {
        servo_retourne.writeMicroseconds(1800); 
    }
}


void Bras::sce()
{
    if (cote == GAUCHE)
    {
        servo_retourne.writeMicroseconds(750); 
    }
    else
    {
        servo_retourne.writeMicroseconds(2050); 
    }
}


void Bras::sct()
{
    scn();
}



void Bras::write_debug()
 {
    asc.write_debug();

    Serial.print("IR ");
    Serial.println(ir.is_on());

    Serial.print("pression ");
    Serial.println(pression.is_on());
    
    color_sensor.write_debug();
 }

void Bras::trigger(int transition)

{
 // add things here for MAE
 //
 //
 //
     if (transition == T_RANGE|| transition == T_ACTIF_NOMINAL || transition == T_ACTIF_TORCHE || transition == POSE)
    {
         Serial.print("new trigger to be ");
         Serial.println(transition);
        trigger_to_be = transition;
    }


  int old_state;
    old_state = state;
   switch(state)
    {
        
        case RANGE_DEPART :
           if (transition == T_ACTIF_NOMINAL)
           {
            state = INT2_RANGE;
           }
           break;
        case INT_RANGE :
            if (transition == T_BUMP_HAUT)
           {
            state = INT2_RANGE;
           }
            break;
        case INT2_RANGE :
            if (transition == T_RANGE)
           {
            state = RANGE_DEPART;
           }
            else (transition == T_ACTIF_NOMINAL)
            {
                state = ATTENTE_ACTIF; 
            }
            break;
        case ATTENTE_ACTIF :
            if (transition == T_MON_IR && mon_ir_actif)
           {
            state = DESCENTE_LAT;
           }
            else if (transition == T_CALL_FOR_HELP)
            {
             state = PRISE_COPAIN;
            }
            else if (transition == T_PRISE_VERT)
            {
             state = SEND_MINE;
            }
            break;

        case DESCENTE_LAT :
            if (transition == T_ASC_PRESQUE_ARRIVE)
           {
            state = DESCENTE_POMPE_LAT;
           }
            break;


        case DESCENTE_POMPE_LAT :
            if (transition == T_ASC_ARRIVE) // ou pression ? 
           {
            state = PRISE_LAT;
           }
            break;

        case PRISE_LAT :
             if (transition == T_COUL_OK) // ou pression ? 
           {
            state = MONTE;
           }
             else if (transition == T_COUL_NOT_OK)
           {
            state = MONTE_ECH;
           }
            break;

        case MONTE :
            if (transition == T_BUMP_HAUT) // ou pression ? 
           {
            state = RANGE_PRISE;
           }
            break;
        case RANGE_PRISE :
           if (transition == TIME_OUT) // ou pression ? 
           {
            state = LACHE;
           }
           break;
        
        case LACHE :
            if (transition == TIME_OUT) // ou pression ? 
           {
            state = INT_RANGE;
           }
            break;
        case MONTE_ECH :
            if (transition == T_ASC_PRESQUE_ARRIVE) // ou pression ? 
           {
            state = RETOURNE_ECH;
           }
            break;
        case RETOURNE_ECH :
            if (transition == T_PRISE_COPAIN) // ou pression ? 
           {
            state = REPLACE_APRES_ECH;
           }
            break;
        case REPLACE_APRES_ECH :
            if (transition == TIME_OUT) // ou pression ? 
           {
            state = ATTENTE_ACTIF; //si faut rebumper metttre INT_RANGE
           }
            break;

        case SEND_MINE :
            if (transition == T_ASC_PRESQUE_ARRIVE) 
           {
            state = SENDMASTER_PRET; 
           }
            break;

        case SENDMASTER_PRET :
            if (transition == T_CAPT_PRESSION_ON) // peut etre declenchee par le master 
           {
            state = PRISE_VERT; 
           }
            break;
        
        case PRISE_VERT :
            if (transition == T_COUL_OK_MASTER) // peut etre declenchee par le master 
           {
            state = MONTE; 
           }
            else if (transition == T_COUL_NOT_OK_MASTER)
            {
            state = MONTE_ECH;
            }
            break;

        case PRISE_COPAIN :
            if (transition == T_ASC_ARRIVE)
             {
                state = MONTE; 
             }
            break;

       
    }
if (state >= 10 && (transition == RANGE || transition == TAPE))
   {
    state = INT_RANGE;
   }
   if (old_state != state)
    {
        Serial.print("new state: ");
        Serial.println(state);

        reset_time_out();
        in_state_func();
    }
}

void Bras::run(){
    col.run();
    if (asc.run())
    {
        trigger(T_ASC_ARRIVE)
    }

    
    if (period_run.is_over())
    {
        period_run.reset();
        if (state == ATTENTE_ACTIF && trigger_attente_on){
                trigger_attente_on = false;
                trigger(trigger_attente);
            }


        if (state == ATTENTE_ACTIF && coul_to_be_on){
                coul_to_be_on = false;
                next_coul = next_coul_to_be
                next_coul_on = true;
            }

        if ((state == PRISE_VERT || state == PRISE_LAT) && next_coul_on)
            {
                next_coul_on = false;
                if (next_coul)
                    {
                        trigger(T_COUL_OK_MASTER)
                    }
               else
               {
                        trigger(T_COUL_NOT_OK_MASTER)
               }
        
        }



        if (mon_ir_actif && ir.is_on())
        {
            trigger(T_MON_IR);
        }


        if (asc.is_up())
            {
                trigger(T_BUMP_HAUT);
            }

        if (is_time_out())
        {
            trigger(trigger_to_be);
        }

        if (asc.is_near())
        {
           trigger(T_ASC_PRESQUE_ARRIVE)
        }
        if (couleur == ROUGE)
        {
            if (col.is_red())
            {
                trigger(T_COUL_OK);
            }
            else if (col.is_yellow())
            {
                trigger(T_COUL_NOT_OK);
            }
        }
        else
        {
            if (col.is_yellow())
            {
                trigger(T_COUL_OK);
            }
            else if (col.is_red())
            {
                trigger(T_COUL_NOT_OK);
            }
        }


  
    }
}

void Bras::set_time_out(int dt_, int trigger)
{
    t_over = millis() + dt_;
    time_out_on = true;
    trigger_to_be = trigger;
    Serial.print("time_out for trigger ");
    Serial.println(trigger);
}

void Bras::reset_time_out()
{
    time_out_on = false;
}


bool Bras::is_time_out()
{
   if (time_out_on && t_over < millis())
   {
     time_out_on = false;
     return true;
   }
   return false;
}

void Bras::in_state_func()
{
    switch (state)
    {
        case RANGE_DEPART :
            scn(); 
            a0();
            spb();
            pf();
            break;
        case INT_RANGE :
            //va bumper
            scn(); 
            a0();
            spb();
            pf();
            break;
        case INT_RANGE2 :
            scn(); 
            a0();
            spb();
            pf();
            break;
        case ATTENTE_ACTIF :
            scn(); 
            a1();
            spb();
            pf();
            break;

        case DESCENTE_LAT :
            scn(); 
            a4();
            spb();
            pf();
            break;
        case DESCENTE_POMPE_LAT :
            scn(); 
            a4();
            spb();
            po();
            break;
        case PRISE_LAT :
            scn(); 
            a4();
            spb();
            po();
            break;
        case MONTE :
            scn(); 
            a0();
            spb();
            po();
            break;
        case RANGE_PRISE :
            scr(); 
            a0();
            spb();
            po();
            break;
        case LACHE :
            scr(); 
            a0();
            spb();
            pf();
            break;
        case MONTE_ECH :
            scn(); 
            a3();
            spb();
            po();
            break;
        case RETOURNE_ECH :
            sce(); 
            a3();
            spr();
            pf();
            break;
        case REPLACE_APRES_ECH :
            scn(); 
            a3();
            spb();
            po();
            break;

        case SEND_MINE :
            scv(); 
            a4();
            spv();
            po();
            break;
        case SENDMASTER_PRET :
            scv(); 
            a4();
            spv();
            p1();
            break;
        case PRISE_VERT :
            scv(); 
            a4();
            spv();
            p1();
            break;

        case PRISE_COPAIN :
            a2(); 
            sce();
            spb();
            p1();
            break;
    }
}

void Bras::set_autre_bras(Bras * autre_bras_)
{
  autre_bras = autre_bras_; 
}

void Bras::stop()
{
   pf();
   asc.stop();
}

void Bras::is_in_attente():
{
    return state == ATTENTE_ACTIF; 
}

void Bras::set_to_be_done(int trigger_attente_)
{
    trigger_attente = trigger_attente_;
    trigger_attente_on = true;
}

void Bras::active_ir()
{
    mon_ir_actif = true;
}

void Bras::desactive_ir()
{
    mon_ir_actif = false;
}

void Bras::call_for_help()
{
    autre_bras->set_to_be_done(T_CALL_FOR_HELP);
}
