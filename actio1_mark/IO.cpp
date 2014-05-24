#include "IO.h"

IO::IO():brasg(GAUCHE), brasd(DROITE)
{
    brasg.set_autre_bras(&brasd);
    brasd.set_autre_bras(&brasg);
    ir_central = SwitchAnalog(PIN_IR_C, SEUIL_IR_C);
    
}


void IO::run()
{
    brasg.run();
    brasd.run();
}


void IO::write_debug()
{
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
    brasg.next_coul(ma_coul);
  }
  else if (brasd.is_in_attente())
  {
    brasd.trigger(T_PRISE_VERT);
    brasg.next_coul(ma_coul);
  }
  else
  {
    brasg.set_to_be_done(T_PRISE_VERT);
    brasg.set_to_be_next_coul(ma_coul);
  }
}
void IO::set_couleur(int coul)
{
    couleur = coul;
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
        
