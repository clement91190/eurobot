#ifndef PINS_H
#define PINS_H

#define ROUGE 0
#define JAUNE 1

//servo
#define PIN_SERVO_ROT_G 10
#define PIN_SERVO_ROT_D 11
#define PIN_SERVO_RETOURNE_G 12
#define PIN_SERVO_RETOURNE_D 13

//capteur couleur
#define PIN_COULEUR_D 0 // interrupt = pin2
#define PIN_COULEUR_G 1 // interrupt = pin3

//Pompe
#define PIN_POMPE_D 5
#define PIN_POMPE_G 4

// capteur pression
//
#define PIN_PRESSION_G A5
#define PIN_PRESSION_D A6

// capteur IR
#define PIN_IR_G A0
#define PIN_IR_C A1
#define PIN_IR_D A2

//bumper Asc
#define PIN_BUMPER_ASC_H_G A3
#define PIN_BUMPER_ASC_H_D A4

//PAP

#define PIN_PAP_STEP_G 7
#define PIN_PAP_DIR_G 6
#define PIN_PAP_STEP_D 9
#define PIN_PAP_DIR_D 8

#endif
