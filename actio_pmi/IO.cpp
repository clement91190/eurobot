#include "IO.h"

Ascenseur::Ascenseur():
    pin_cmd_mot(PIN_MOT_CMD_ASC), pin_dir_mot(PIN_MOT_DIR_ASC),
    bumper_asc_haut(PIN_BUMPER_ASC_HAUT), bumper_asc_bas(PIN_BUMPER_ASC_BAS)
{
    bumper_asc_bas.reverse();
    bumper_asc_haut.reverse();
}

bool Ascenseur::is_up()
{
    return bumper_asc_haut.is_on();    
}

bool Ascenseur::is_low()
{
    return bumper_asc_bas.is_on();
}

void Ascenseur::monte()
{
    start_asserv(HAUT);
}

void Ascenseur::descend(){
    start_asserv(BAS);
}
void Ascenseur::start_asserv(int target_)
{
    target = target_;
    in_asserv = true;
}

void Ascenseur::write_debug()
{
    Serial.print(" bumper haut: ");
    Serial.println(bumper_asc_haut.is_on());

     Serial.print(" bumper  bas: ");
    Serial.println(bumper_asc_bas.is_on());

    Serial.print(" tic odo: ");
    Serial.println(tic_odo);


}

void Ascenseur::run()
{
    if (in_asserv)
    {
        if (target == HAUT)
        {
            if (bumper_asc_haut.is_on())
            {
                in_asserv = false;
                send_zeros();
            }
            //else if (tic_odo < TIC_HAUT)
            //{
           //     send_monte();
            //}
           else
            {
                send_monte();
            }
                
        }
        else if (target == BAS)
        {
          /*if (tic_odo > TIC_BAS)
            {
                send_desc();
            }*/
          if (bumper_asc_bas.is_on())
            {
                Serial.println("ASS FINI");
                send_zeros();
                in_asserv = false;
            }
          else
            {
                send_desc();
            }
        }
        else 
        {
            if (tic_odo > target + 10)
            {
                send_monte(); 
            }
            else if (tic_odo < target - 10 )
            {
                send_desc();
            }
            else if (tic_odo < target)
            {
                send_maintien_p();
            }
            else if (tic_odo >= target)
            {
                send_zeros();
            }
        }
    }
}

void Ascenseur::send_monte()
{
    digitalWrite(PIN_MOT_DIR_ASC, 0);
    analogWrite(PIN_MOT_CMD_ASC, 250);
}

void Ascenseur::send_maintien_p()
{
    digitalWrite(PIN_MOT_DIR_ASC, 0);
    analogWrite(PIN_MOT_CMD_ASC, 1);
}

void Ascenseur::send_desc()
{
    digitalWrite(PIN_MOT_DIR_ASC, 1);
    analogWrite(PIN_MOT_CMD_ASC, 100);
}

void Ascenseur::send_zeros()
{
    digitalWrite(PIN_MOT_DIR_ASC, 0);
    analogWrite(PIN_MOT_CMD_ASC, 0);
}

//PINCE
//

Pince::Pince():
    ir_feu(PIN_IR), time_out_on(false), state(INT_RANGE), period_run(50)
{
    asc.monte();
    asc.run();
    delay(2000);
    servo_pince_g.attach(PIN_SERVO_PINCE_G);
    servo_pince_d.attach(PIN_SERVO_PINCE_D);
    servo_pince_rot.attach(PIN_SERVO_PINCE_ROT);
    ir_feu.reverse();
    in_state_func();
    trigger(RANGE);
 }

void Pince::write_debug()
 {
    asc.write_debug();
    Serial.print("IR ");
    Serial.println(ir_feu.is_on());
    col.write_debug();
 }
void Pince::trigger(int transition)
{
 // add things here for MAE
 //
 //
 //
     if (transition == T_BAS|| transition == RANGE || transition == TAPE || transition == T_ACTIF_FEU || transition == T_ACTIF_TORCHE || transition == POSE)
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
           if (transition == TAPE)
           {
            state = TAPE_VERTICAL;
           }
           else if (transition == T_BAS)
           {
            state = INTERMEDIAIRE_SORTIE;
           }
           break;
        case TAPE_VERTICAL :
            if (transition == RANGE)
            {
                state = RANGE_DEPART;
            }
            else if (transition == T_BAS)
            {
                state = INTERMEDIAIRE_SORTIE;
            }
            break;
        case INTERMEDIAIRE_SORTIE :
            if (transition == TAPE)
                {
                    state = TAPE_VERTICAL;
                }
            else if (transition == T_BAS)
                {
                    state = HORIZ_VERT;
                }
            else if (transition == RANGE)
                {
                    state = RANGE_DEPART;
                }
            break;
        case HORIZ_VERT :
            if (transition == T_ACTIF_FEU || transition == T_ACTIF_TORCHE)
                {
                    state = NOMINAL_VIDE;
                }
            break;
        case NOMINAL_VIDE : 
            if (transition == T_ACTIF_FEU)
            {
                state = ACTIF_FEU;
            }
            else if (transition == T_ACTIF_TORCHE)
            {
                state = ACTIF_TORCHE;
            
            }
            break;
            
        case ACTIF_TORCHE :
            if (transition == T_IR)
            {
                state = PRISE_TORCHE;
            }
            break;

        case PRISE_TORCHE : 
            if (transition == POSE)
            {
                state = NOMINAL_VIDE;
            }
            break;
        case ACTIF_FEU :
            if (transition == T_IR)
            {
                state = PRISE_FEU;
            }
            break;

        case PRISE_FEU:
            if (transition == TIME_OUT)
            {
                state = MONTE_COOL;
            }
            break;
        case MONTE_COOL:
            if (transition == TIME_OUT)
            {
                state = MONTE_COOL2;
            }
            break;
        case MONTE_COOL2 :
            if (transition == COOL_OK)
            {
                state = WAIT_COOL_OK;
            }
            else if (transition == COOL_NOT_OK)
            {
                state = RETOURNE_COOL_N;
            }
            break;
        case WAIT_COOL_OK :
            if (transition == POSE)
                {
                   state = HORIZ_VERT;
                }
            break;
        case RETOURNE_COOL_N :
            if (transition == TIME_OUT)
            {
                state = DESCEND_COOL_N;
            }
            break;
        case DESCEND_COOL_N :
            if (transition == POSE)
                {
                    state = POSE_COOL_N;
                }
            break;
        case POSE_COOL_N :
            if (transition == TIME_OUT)
            {
                state = INT1_COOL_N;
            }
            break;
        case INT1_COOL_N :
            if (transition == TIME_OUT)
            {
                state = INT2_COOL_N;
            }
            break;
        case INT2_COOL_N :
            if (transition == TIME_OUT)
            {
                state = INT3_COOL_N;
            }
            break;
        case INT3_COOL_N :
            if (transition == TIME_OUT)
            {
                state = HORIZ_VERT;
            }
            break;
        case INT_RANGE :
            if (transition == T_BUMP_HAUT)
            {
                state = INT2_RANGE; 
            }
            break;
        case INT2_RANGE :
            if (transition == RANGE || transition == TAPE)
            {
                state = INTERMEDIAIRE_SORTIE; 
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

void Pince::run(){
    col.run();
    
    if (period_run.is_over())
    {
        period_run.reset();
        asc.run();
        if (asc.is_up())
            {
                trigger(T_BUMP_HAUT);
            }
        if (is_time_out())
        {
            trigger(trigger_to_be);
        }
        if (ir_feu.is_off())
        {
            trigger(T_IR);
        }
        if (couleur == ROUGE)
        {
            if (col.is_red())
            {
                trigger(COOL_OK);
            }
            else if (col.is_yellow())
            {
                trigger(COOL_NOT_OK);
            }
        }
        else
        {
            if (col.is_yellow())
            {
                trigger(COOL_OK);
            }
            else if (col.is_red())
            {
                trigger(COOL_NOT_OK);
            }
        }
    }
}

void Pince::set_time_out(int dt_, int trigger)
{
    t_over = millis() + dt_;
    time_out_on = true;
    trigger_to_be = trigger;
    Serial.print("time_out for trigger ");
    Serial.println(trigger);
}

void Pince::reset_time_out()
{
    time_out_on = false;
}


bool Pince::is_time_out()
{
   if (time_out_on && t_over < millis())
   {
     time_out_on = false;
     return true;
   }
   return false;
}

void Pince::slr()
{
    servo_pince_g.writeMicroseconds(1420); //rangement
    servo_pince_d.writeMicroseconds(1580); //rangement
}

void Pince::slt()
{
    servo_pince_g.writeMicroseconds(1900); //rangement
    servo_pince_d.writeMicroseconds(1580); //rangement
}
void Pince::slo()
{
    servo_pince_g.writeMicroseconds(1800); //saisie
    servo_pince_d.writeMicroseconds(1200); //saisie
}

void Pince::slf()
{
    servo_pince_g.writeMicroseconds(2200); //saisie
    servo_pince_d.writeMicroseconds(800); //saisie
}

void Pince::scv() //position de depart
{
    servo_pince_rot.writeMicroseconds(1340); //vertical
}
void Pince::scn()
{
    servo_pince_rot.writeMicroseconds(2140); //vertical
}

void Pince::scr()
{
    servo_pince_rot.writeMicroseconds(560); //vertical
}

void Pince::a0()
{
    asc.descend();
}

void Pince::a1()
{
    asc.monte();
}

void Pince::in_state_func()
{
    switch (state)
    {
        case RANGE_DEPART :
            a1();
            scv();
            slr();
            break;
        case TAPE_VERTICAL :
            a1();
            scv();
            slt();
            break;
        case INTERMEDIAIRE_SORTIE :
            set_time_out(500, trigger_to_be);
            a1();
            slo();
            scv();
            break;
        case HORIZ_VERT :
            a1();
            slo();
            scn();
            break;
        case NOMINAL_VIDE : 
            set_time_out(500, trigger_to_be);
            a0();
            slo();
            scn();
            break;
        case ACTIF_TORCHE :
            a0();
            slo();
            scn();
            break;
        case PRISE_TORCHE : 
            a0();
            slf();
            scn();
            break;
        case ACTIF_FEU :
            a0();
            slo();
            scn();
            break;
        case PRISE_FEU :
            set_time_out(500, TIME_OUT);
            a0();
            slf();
            scn();
            break;
        case WAIT_COOL_OK :
            break;
        case MONTE_COOL :
            set_time_out(500, TIME_OUT);
            a1();
            slf();
            scn();
            break;
        case MONTE_COOL2 :
            a1();
            slf();
            scn();
            break;
        case RETOURNE_COOL_N :
            set_time_out(500, TIME_OUT);
            a1();
            slf();
            scr();
            break;
        case DESCEND_COOL_N :
            a0();
            slf();
            scr();
            break;
        case POSE_COOL_N :
            set_time_out(500, TIME_OUT);
            a0();
            slo();
            scr();
            break;
        case INT1_COOL_N : 
            set_time_out(500, TIME_OUT);
            a1();
            slo();
            scr();
            break;
        case INT2_COOL_N : 
            set_time_out(500, TIME_OUT);
            a1();
            slf();
            scr();
            break;
        case INT3_COOL_N :
            set_time_out(500, TIME_OUT);
            a1();
            slf();
            scn();
            break;
        case INT_RANGE :
            a1();
            slo();
            break;
        case INT2_RANGE :
            set_time_out(10, trigger_to_be);
            a1();
            slo();
            break;
    }
}

// IO
//
//
//
IO::IO():pince()
{
    servo_fresque_g.attach(PIN_SERVO_FRESQUE_G);
    servo_fresque_d.attach(PIN_SERVO_FRESQUE_D);
    //ranger_servo_fresque();
    poser_fresque();

    servo_filet_g.attach(PIN_SERVO_FILET_G);
    servo_filet_d.attach(PIN_SERVO_FILET_D);
    ranger_servo_filet();
}

void IO::poser_fresque()
{
    servo_fresque_g.writeMicroseconds(1500);
    servo_fresque_d.writeMicroseconds(1500);
}

void IO::ranger_servo_fresque()
{
    servo_fresque_g.writeMicroseconds(1900);
    servo_fresque_d.writeMicroseconds(1100);
}


void IO::envoi_filet()
{
       servo_filet_g.writeMicroseconds(1600); //ouverture
       servo_filet_d.writeMicroseconds(1400); //ouverture
}

void IO::ranger_servo_filet()
{
       servo_filet_g.writeMicroseconds(1100); //ouverture
       servo_filet_d.writeMicroseconds(1900); //ouverture
}
   
void IO::run()
{
    pince.run();
}

void IO::trigger(int transition)
{
    pince.trigger(transition);
}

void IO::write_debug()
{
    pince.write_debug();
}
