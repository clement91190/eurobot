#include "IO.h"

IO::IO():
    brasg(GAUCHE , PIN_PAP_STEP_G, PIN_PAP_DIR_G, PIN_BUMPER_ASC_H_G, PIN_IR_G, SEUIL_IR_G, &pulse_color_g),
    brasd(DROITE , PIN_PAP_STEP_D, PIN_PAP_DIR_D, PIN_BUMPER_ASC_H_D, PIN_IR_D, SEUIL_IR_D, &pulse_color_d),
    ir_central(PIN_IR_C, SEUIL_IR_C), couleur(0)
{
    //brasg.set_autre_bras(&brasd);
    //brasd.set_autre_bras(&brasg);
    
}


void IO::run()
{
    brasg.run();
    brasd.run();
    if (brasd.is_trigger_autre_on())
    {
        int transition = brasd.get_trigger_autre();
        if (transition == T_PRISE_COPAIN)
        {
            brasg.set_time_out(600, transition) ;
        }
        else
        {
            brasg.trigger(transition);
        }

    }

    if (brasg.is_trigger_autre_on())
    {
        int transition = brasg.get_trigger_autre();
        if (transition == T_PRISE_COPAIN)
        {
            brasd.set_time_out(600, transition) ;
        }
        else
        {
            brasd.trigger(transition);
        }
    }
}


void IO::write_debug()
{
    Serial.println("Ma couleur rouge(=0) ? ");
    Serial.println(couleur);

    Serial.println("BRAS G");
    brasg.write_debug();

    Serial.println("BRAS D");
    brasd.write_debug();
}

void IO::bras_actif()
{
    brasg.trigger(T_ACTIF_NOMINAL);
    brasd.trigger(T_ACTIF_NOMINAL);
}
void IO::range_bras()
{
    brasg.trigger(T_RANGE);
    brasd.trigger(T_RANGE);
}
void IO::prise_centre(bool ma_coul)
{
  if (brasg.is_in_attente())
  {
    brasg.trigger(T_PRISE_VERT);
    brasg.set_next_coul(ma_coul);
    bras_prise = GAUCHE;
  }
  else if (brasd.is_in_attente())
  {
    brasd.trigger(T_PRISE_VERT);
    brasd.set_next_coul(ma_coul);
    bras_prise = DROITE;
  }
  else
  {
    brasg.set_to_be_done(T_PRISE_VERT);
    brasg.set_to_be_next_coul(ma_coul);
    bras_prise = GAUCHE;
  }
}
void IO::set_couleur(int coul)
{
    couleur = coul;
    brasg.set_couleur(coul);
    brasd.set_couleur(coul);
}
void IO::routine_torches()
{
    //TODO
}
void IO::stop()
{
    //fonction bloquante -> fin du match.
    brasg.stop();
    brasd.stop();
    Serial.println("ACTIO 2 SHUTDOWN");
    while (1){delay(1);}
}
void IO::pose()
{

}

void IO::active_irs()
{
    brasg.active_ir();
    brasd.active_ir();
}

void IO::desactive_irs()
{
    brasg.desactive_ir();
    brasd.desactive_ir();

}
 
void IO:: conf_prise_centre()
{
    if (bras_prise == GAUCHE)
     {
        Serial.println("confirme au bras g");
        brasg.trigger(T_CAPT_PRESSION_ON);
     }
    else
    {
        Serial.println("confirme au bras d");
        brasd.trigger(T_CAPT_PRESSION_ON);
    }

}
        
