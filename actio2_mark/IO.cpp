#include "IO.h"
// IO
//
//
//


IO::IO()
{
    servo_benne_c.attach(PIN_SERVO_BENNE_C);
    servo_benne_g.attach(PIN_SERVO_BENNE_G);
    servo_benne_d.attach(PIN_SERVO_BENNE_D);

    servo_rampe_g.attach(PIN_SERVO_RAMPE_G);
    servo_benne_d.attach(PIN_SERVO_RAMPE_D);

    servo_tacle_g.attach(PIN_SERVO_TACLE_G);
    servo_tacle_d.attach(PIN_SERVO_TACLE_D);

    servo_pile_av_g.attach(PIN_SERVO_PILE_AVG);
    servo_pile_av_d.attach(PIN_SERVO_PILE_AVD);
    servo_pile_ar_g.attach(PIN_SERVO_PILE_ARG);
    servo_pile_ar_d.attach(PIN_SERVO_PILE_ARD);
}
  

