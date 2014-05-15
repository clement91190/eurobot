Switch::Switch(int pin_):pin(pin_), reversed(false)
{
}
bool Switch::is_on()
{
    if (reverse)
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
    if (reverse)
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
    reversed = !reversed
}

Ascenseur::Ascenseur(int pin_cmd_mot_, int pin_dir_mot_, int pin_bumper_high, int pin_bumper_low__):
    pin_cmd_mot(pin_cmd_mot_), pin_dir_mot(pin_dir_mot_), bumper_asc_haut(pin_bumper_high_), bumper_asc_bas(pin_bumper_low_)
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
                send_descend();
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
                send_descend();
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

void Ascenseur::send_monte();
{
    digitalWrite(PIN_MOT_ASC_ASC, 0);
    analogWrite(PIN_MOT_CMD_ASC, 250);
}

void Ascenseur::send_maintien_p();
{
    digitalWrite(PIN_MOT_ASC_ASC, 0);
    analogWrite(PIN_MOT_CMD_ASC, 10);
}

void Ascenseur::send_desc();
{
    digitalWrite(PIN_MOT_ASC_ASC, 1);
    analogWrite(PIN_MOT_CMD_ASC, 100);
}

void Ascenseur::send_zeros();
{
    digitalWrite(PIN_MOT_ASC_ASC, 1);
    analogWrite(PIN_MOT_CMD_ASC, 0);
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
