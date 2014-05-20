#ifndef PINS_H
#define PINS_H

// bumpers
#define PIN_AN_START  A2
#define PIN_AN_COULEUR  A3

#define PIN_AN_BUMPG  A6
#define PIN_AN_BUMPD  A7

// sonar
#define PIN_AN_SONARG  A1



#ifdef PMI
//pmi
#define PIN_DI_STRAT1  A7
#define PIN_DI_STRAT2  A6
#define PIN_SERVO_CAM  6

#define PIN_ODO_INCD  1 // == pin 2
#define PIN_ODO_INCG  0 // == pin 3
#define PIN_ODO_DIRD  5
#define PIN_ODO_DIRG  4

#define PIN_MOT_CMDG  11
#define PIN_MOT_CMDD  9
#define PIN_MOT_DIRG  10
#define PIN_MOT_DIRD  8



#else
// mark
#define PIN_DI_STRAT1  6
#define PIN_DI_STRAT2  7
#define PIN_AN_SONARD  A0

#define PIN_ODO_INCG  0 // == pin 2
#define PIN_ODO_INCD  1 // == pin 3
#define PIN_ODO_DIRG  4
#define PIN_ODO_DIRD  5

#define PIN_MOT_CMDD  9
#define PIN_MOT_CMDG  11
#define PIN_MOT_DIRD  8
#define PIN_MOT_DIRG  10



#endif

#endif
