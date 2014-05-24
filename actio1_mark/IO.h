#ifndef IO_H
#define IO_H

#include <Period.h>
#include "Arduino.h"
#include <Servo.h>
#include "interrupts.h"
#include "pins.h"
#include "Bras.h"
#include <SwitchAnalog.h>

class IO
/** (x,y,cap) objects **/
{
    private:
        Bras brasg;
        Bras brasd;
        int couleur;
        SwitchAnalog ir_central;

    public:
        IO();
        void write_debug();
        void run();

        void bras_actif();
        void range_bras();
        void active_irs();
        void desactive_irs();
        void prise_centre(bool ma_coul);
        void set_couleur(int coul);
        void routine_torches();
        void stop();
        void pose();

};

#endif
