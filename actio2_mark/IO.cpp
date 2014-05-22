#include "IO.h"
// IO
// IO Mark Zero
//
//
Pile::Pile(int cote)
{
	
	
}



IO::IO(): pile_gauche(GAUCHE), pile_droite(DROITE)
{

    servo_benne_c.attach(PIN_SERVO_BENNE_C);
    servo_benne_g.attach(PIN_SERVO_BENNE_G);
    servo_benne_d.attach(PIN_SERVO_BENNE_D);

    servo_rampe_g.attach(PIN_SERVO_RAMPE_G);
    servo_benne_d.attach(PIN_SERVO_RAMPE_D);

    servo_tacle_g.attach(PIN_SERVO_TACLE_G);
    servo_tacle_d.attach(PIN_SERVO_TACLE_D);

}
  

