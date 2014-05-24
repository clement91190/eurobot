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

void IO::bras_attente()
{
    
}
void IO::range_bras()
{

}
void IO::prise_centre(bool ma_coul)
{
    
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
        
