#ifndef PINS_H
#define PINS_H

/*
//pin alloc
#define PIN_ODO_INCD  0 // == pin 2
#define PIN_ODO_INCG  1 // == pin 3
#define PIN_ODO_DIRD  12
#define PIN_ODO_DIRG  11

#define PIN_MOT_CMDD  9
#define PIN_MOT_CMDG  10
#define PIN_MOT_DIRD  5
#define PIN_MOT_DIRG  6
*/

// slave Gros
// prgramme de test  Mark zero Slave

//int DEBUG = 1;
//int vitesse = 0;

// bumpers
#define PIN_AN_START  A2
#define PIN_AN_COULEUR  A3
#define PIN_DI_STRAT1  6
#define PIN_DI_STRAT2  7
#define PIN_AN_BUMPG  A6
#define PIN_AN_BUMPD  A7

// sonar
#define PIN_AN_SONARG  A1
#define PIN_AN_SONARD  A0

// Moteur
//#define PinDi_MoteurD_SENS = 8;
//#define PinDi_MoteurD_PWM = 9;
//#define PinDi_MoteurG_SENS = 10;
//#define PinDi_MoteurG_PWM = 11;

// ODOs
//#define PinDi_ODO_G_A = 2; //interruption
//#define PinDi_ODO_G_B = 4;
//#define PinDi_ODO_D_A = 3; //interruption
//#define PinDi_ODO_D_B = 5;

#define PIN_ODO_INCG  0 // == pin 2
#define PIN_ODO_INCD  1 // == pin 3
#define PIN_ODO_DIRG  4
#define PIN_ODO_DIRD  5

#define PIN_MOT_CMDD  9
#define PIN_MOT_CMDG  11
#define PIN_MOT_DIRD  8
#define PIN_MOT_DIRG  10




#endif
