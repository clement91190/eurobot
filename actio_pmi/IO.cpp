#include "IO.h"

Switch::Switch(int pin_):pin(pin_), reversed(false)
{
}
bool Switch::is_on()
{
    if (reversed)
    {
        return (digitalRead(pin) == LOW);
    }
    else
    {
        return (digitalRead(pin) == HIGH);
    }

}

bool Switch::is_off()
{
    if (reversed)
    {
        return (digitalRead(pin) == HIGH);
    }
    else
    {
        return (digitalRead(pin) == LOW);
    }

}

void Switch::reverse()
{
    reversed = !reversed;
}

Ascenseur::Ascenseur():
    pin_cmd_mot(PIN_MOT_CMD_ASC), pin_dir_mot(PIN_MOT_DIR_ASC),
    bumper_asc_haut(PIN_BUMPER_ASC_HAUT), bumper_asc_bas(PIN_BUMPER_ASC_BAS)
{}

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
    target = target;
    in_asserv = true;
}

void Ascenseur::run()
{
    if (in_asserv)
    {
        if (target == HAUT)
        {
            if (tic_odo < TIC_HAUT)
            {
                send_monte();
            }
            else if (bumper_asc_haut.is_on())
            {
                send_zeros();
            }
            else
            {
                send_maintien_p();
            }
                
        }
        else if (target == BAS)
        {
          if (tic_odo > TIC_BAS)
            {
                send_desc();
            }
          if (bumper_asc_bas.is_on())
            {
                send_zeros();
                in_asserv = false;
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
    analogWrite(PIN_MOT_CMD_ASC, 10);
}

void Ascenseur::send_desc()
{
    digitalWrite(PIN_MOT_DIR_ASC, 1);
    analogWrite(PIN_MOT_CMD_ASC, 100);
}

void Ascenseur::send_zeros()
{
    digitalWrite(PIN_MOT_DIR_ASC, 1);
    analogWrite(PIN_MOT_CMD_ASC, 0);
}

ColorSensor::ColorSensor():
    period_reset(80)
{
   // periode d'echantillonage de remise a 0 pour la lecture de la couleur
}

bool ColorSensor::is_red()
{
   //TODO write constants for red and yellowlast_count
   return true;
}
bool ColorSensor::is_yellow()
{
    return false;
}

void ColorSensor::run()
{
    if (period_reset.is_over())
    {
        period_reset.reset();
        last_count = pulse_color;
        pulse_color = 0;
    }
}
 
//PINCE
//

Pince::Pince():
    ir_feu(PIN_IR)
{
    servo_pince_g.attach(PIN_SERVO_PINCE_G);
    servo_pince_d.attach(PIN_SERVO_PINCE_D);
    servo_pince_rot.attach(PIN_SERVO_PINCE_ROT);
    ir_feu.reverse();
    ranger_lateral();
    rotation_pince_milieu();
 }

void Pince::trigger(int transition)
{
 // add things here for MAE
}

void Pince::run(){
    asc.run();
}

void Pince::ranger_lateral()
{
    servo_pince_g.writeMicroseconds(1420); //rangement
    servo_pince_d.writeMicroseconds(1580); //rangement
}
void Pince::ouvrir_pince()
{
    servo_pince_g.writeMicroseconds(1800); //saisie
    servo_pince_d.writeMicroseconds(1200); //saisie
}

void Pince::serrer_feu_pince()
{
    servo_pince_g.writeMicroseconds(2200); //saisie
    servo_pince_d.writeMicroseconds(800); //saisie
}

void Pince::rotation_pince_milieu() //position de depart
{
    servo_pince_rot.writeMicroseconds(1340); //vertical
}
void Pince::rotation_pince_normal()
{
    servo_pince_rot.writeMicroseconds(2140); //vertical
}

void Pince::rotation_pince_retourne()
{
    servo_pince_rot.writeMicroseconds(560); //vertical
}


// IO
//
//
//
IO::IO():pince()
{
    servo_fresque_g.attach(PIN_SERVO_FRESQUE_G);
    servo_fresque_d.attach(PIN_SERVO_FRESQUE_D);
    ranger_servo_fresque();

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
