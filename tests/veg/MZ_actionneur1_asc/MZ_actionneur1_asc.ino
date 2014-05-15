// prgramme de test  Mark zero Slave

#include <Servo.h>
#include <Stepper.h>
#include <AccelStepper.h>

int DEBUG = 1;



// bumpers Asc
const int Pin_AN_BumperAsc_G = A3;
const int Pin_AN_BumperAsc_D = A4;
int state_BumperAsc_G = 0;
int state_BumperAsc_D = 0;

// Relais pompe
const int Pin_DI_Pompe_G = 5;
const int Pin_DI_Pompe_D = 4;

// Capteur Pression
const int Pin_AN_Pression_G = A5;
const int Pin_AN_Pression_D = A6;
const int seuil_PRESSION = 0;
int state_Pression_G = 0;
int state_Pression_D = 0;

// a verifffffffffffff
// capteur couleur
const int Pin_Di_Couleur_G = 3;
const int Pin_Di_Couleur_D = 2;

// moteur Pas a Pas
const int Pin_Di_Papa_G_STEP = 7;
const int Pin_Di_Papa_G_DIR = 6;
const int Pin_Di_Papa_D_STEP = 9;
const int Pin_Di_Papa_D_DIR = 8;
const int nb_Step = 200; //nombre de step pour une rotation de moteur
AccelStepper  Ascenseur_G(1, Pin_Di_Papa_G_STEP, Pin_Di_Papa_G_DIR);
AccelStepper  Ascenseur_D(1, Pin_Di_Papa_D_STEP, Pin_Di_Papa_D_DIR);

// Capteur IR
const int Pin_An_IR_G = A0;
const int Pin_An_IR_C = A1;
const int Pin_An_IR_D = A2;
int state_IRG = 0;
int state_IRC = 0;
int state_IRD = 0;
const int seuilIR_lateral = 220; //inf
const int seuilIR_middle = 200;

//Servos Bras
const int Pin_Di_BRAS_G_Hz = 10;
const int Pin_Di_BRAS_G_Piv = 12;
const int Pin_Di_BRAS_D_Hz = 11;
const int Pin_Di_BRAS_D_Piv = 13;
Servo BRAS_G_Hz;
Servo BRAS_G_Piv;
Servo BRAS_D_Hz;
Servo BRAS_D_Piv;

int a = 0;     //  gen counter

/* VARIABLE POUR LES CAPTEURS COULEURS */
unsigned long pulse_cnt_G = 0;
unsigned long pulse_cnt_D = 0;
// 1000ms = 1s
#define READ_TM 1000
// two variables used to track time
unsigned long cur_tm = millis();
unsigned long pre_tm = cur_tm;
 // we'll need to access the amount
 // of time passed
unsigned int tm_diff = 0;
unsigned long frequency_G = 0;
unsigned long frequency_D = 0;


void setup() 
{ 
  Serial.begin(9600);
  
  //Ascenseur_G.setMaxSpeed(3000);
  //Ascenseur_D.setMaxSpeed(300);
  //Ascenseur_G.setSpeed(1000);
  //Ascenseur_D.setSpeed(50);
  //Ascenseur_D.moveTo(600);
  //Ascenseur_G.moveTo(600);
  
  attachInterrupt(0, add_pulse_D, RISING);
  attachInterrupt(1, add_pulse_G, RISING);
  
  pinMode(Pin_Di_Couleur_G, INPUT);
  pinMode(Pin_Di_Couleur_G, INPUT);
  pinMode(Pin_AN_Pression_G, INPUT);
  pinMode(Pin_AN_Pression_D, INPUT);
  pinMode(Pin_AN_BumperAsc_G, INPUT);
  pinMode(Pin_AN_BumperAsc_D, INPUT);
  pinMode(Pin_DI_Pompe_D, OUTPUT);
  pinMode(Pin_DI_Pompe_G, OUTPUT);
  pinMode(Pin_An_IR_G, INPUT);
  pinMode(Pin_An_IR_C, INPUT);
  pinMode(Pin_An_IR_D, INPUT);
  pinMode(Pin_DI_Pompe_D, OUTPUT);
  pinMode(Pin_DI_Pompe_G, OUTPUT);
  
  BRAS_G_Hz.attach(Pin_Di_BRAS_G_Hz);
  BRAS_G_Piv.attach(Pin_Di_BRAS_G_Piv);
  BRAS_D_Hz.attach(Pin_Di_BRAS_D_Hz);
  BRAS_D_Piv.attach(Pin_Di_BRAS_D_Piv);
  
  
  //init servos et variable
  BRAS_G_Hz.writeMicroseconds(1500); // rangement
  BRAS_G_Hz.writeMicroseconds(2000); // rangement
  BRAS_G_Piv.writeMicroseconds(500); //bas
  BRAS_D_Hz.writeMicroseconds(800);
  BRAS_D_Piv.writeMicroseconds(2050);
  
  pinMode(Pin_Di_Papa_G_STEP, OUTPUT);
  pinMode(Pin_Di_Papa_G_DIR, OUTPUT);
  pinMode(Pin_Di_Papa_D_STEP, OUTPUT);
  pinMode(Pin_Di_Papa_D_DIR, OUTPUT);
  
  digitalWrite(Pin_DI_Pompe_D, HIGH); 
  digitalWrite(Pin_DI_Pompe_G, HIGH); 
  
} 
 
void loop() 
{ 
  
  // truc de merde pour la periode de la mesure des capteurs couleurs
  pre_tm   = cur_tm;
  cur_tm   = millis();
  if( cur_tm > pre_tm ) {
    tm_diff += cur_tm - pre_tm;
 }
 else if( cur_tm < pre_tm ) {
           // handle overflow and rollover (Arduino 011)
    tm_diff += ( cur_tm + ( 34359737 - pre_tm ));
 }
 
   // if enough time has passed to do a new reading...
 
 if( tm_diff >= READ_TM ) {
 
       // re-set the ms counter
   tm_diff = 0;
 
      // get our current frequency reading
   frequency_G = get_tsl_freq_G();
   frequency_D = get_tsl_freq_D();
 
 }

  //lecture des autres capteurs IR et Bumpers
  state_Pression_G = analogRead(Pin_AN_Pression_G);
  state_Pression_D = analogRead(Pin_AN_Pression_D);
  state_IRG = analogRead(Pin_An_IR_G);
  state_IRC = analogRead(Pin_An_IR_C);
  state_IRD = analogRead(Pin_An_IR_D);
  state_BumperAsc_G = digitalRead(Pin_AN_BumperAsc_G);  
  state_BumperAsc_D = digitalRead(Pin_AN_BumperAsc_D);  
  delay(100);
  
  
  //Ascenseur_D.run();
  //Ascenseur_G.runSpeed();
  /*
  int del = 600;
  digitalWrite(Pin_Di_Papa_D_DIR, HIGH);
  for(int x = 0; x < 2000; x++) {
    digitalWrite(Pin_Di_Papa_D_STEP, HIGH);   
    delayMicroseconds(del);               
    digitalWrite(Pin_Di_Papa_D_STEP, LOW);  
    delayMicroseconds(del);              
   }
    delay(1000);   
  
  digitalWrite(Pin_Di_Papa_D_DIR, LOW);
  for(int x = 0; x < 2000; x++) {
    digitalWrite(Pin_Di_Papa_D_STEP, HIGH);   
    delayMicroseconds(del);               
    digitalWrite(Pin_Di_Papa_D_STEP, LOW);  
    delayMicroseconds(del);              
   }
   delay(1000);
   
  
  */
  
  if(DEBUG)
  {
    Serial.print("Pression Gauche : ");  
    Serial.println(state_Pression_G); 
    Serial.print("Pression Droite : "); 
    Serial.println(state_Pression_D); 
    Serial.print("IR gauche : ");  
    Serial.println(state_IRG); 
    Serial.print("IR central : ");  
    Serial.println(state_IRC); 
    Serial.print("IR droite : ");  
    Serial.println(state_IRD); 
    Serial.print("Bumper Asc gauche : ");  
    Serial.println(state_BumperAsc_G); 
    Serial.print("Bumper Asc droite : ");  
    Serial.println(state_BumperAsc_D); 
    Serial.print("frequence capteur couleur Gauche : ");  
    Serial.println(frequency_G); 
    Serial.print("couleur capteur couleur Gauche : ");  
    if(frequency_G > 3000000 & frequency_G < 8000000){
      Serial.println("rouge");
    }
    else{
     if(frequency_G > 8000000){
      Serial.println("jaune");
      } 
      else{
        Serial.println("je sais pas");
      }
    }
    
    Serial.print("frequence capteur couleur Droit : ");  
    Serial.println(frequency_D); 
    Serial.print("couleur capteur couleur Droit : ");  
    if(frequency_D > 3000000 & frequency_D < 8000000){
      Serial.println("rouge");
    }
    else{
     if(frequency_D > 8000000){
      Serial.println("jaune");
      } 
      else{
        Serial.println("je sais pas");
      }
    }

    Serial.println();
    
    delay(500);
    
  }
  
  if (Serial.available() > 0) {
     int inByte = Serial.read();

     switch (inByte) {
       
     // active pompe gauche
     case 'a':
       digitalWrite(Pin_DI_Pompe_G, LOW); 
       Serial.println("activation pompe gauche"); 
       break;
       
       // active pompe gauche
     case 'b':
       digitalWrite(Pin_DI_Pompe_G, HIGH); 
       Serial.println("desactivation pompe gauche");        
       break;
     
     // active pompe droite
     case 'c':
       digitalWrite(Pin_DI_Pompe_D, LOW); 
       Serial.println("activation pompe droite"); 
       break;
       
       // desactive pompe droite
     case 'd':
       digitalWrite(Pin_DI_Pompe_D, HIGH); 
       Serial.println("desactivation pompe droite");        
       break;
      
       // BRAS GAUCHE PIVOT bas
     case 'e':
       BRAS_G_Piv.writeMicroseconds(500); 
       Serial.println("BRAS GAUCHE PIVOT bas");        
       break; 
       
       // BRAS GAUCHE PIVOT milieu
     case 'f':
       BRAS_G_Piv.writeMicroseconds(1400); 
       Serial.println("BRAS GAUCHE PIVOT milieu");        
       break; 
       
       // BRAS GAUCHE PIVOT haut
     case 'g':
       BRAS_G_Piv.writeMicroseconds(2200); 
       Serial.println("BRAS GAUCHE PIVOT haut");        
       break; 
       
        // BRAS GAUCHE horizontal chope avant vertical
     case 'k':
       BRAS_G_Hz.writeMicroseconds(700); // rangement
       Serial.println("BRAS GAUCHE horizontal chope avant vertical");        
       break; 
       
      // BRAS GAUCHE horizontal chope echange vertical
     case 'l':
       BRAS_G_Hz.writeMicroseconds(750); // rangement
       Serial.println("BRAS GAUCHE horizontal echange vertical");        
       break; 
       
       // BRAS GAUCHE horizontal chope lateral
     case 'm':
       BRAS_G_Hz.writeMicroseconds(1100); // rangement
       Serial.println("BRAS GAUCHE horizontal chope chope lateral");        
       break; 
       
       // BRAS GAUCHE horizontal chope rangement
     case 'n':
       BRAS_G_Hz.writeMicroseconds(2000); // rangement
       Serial.println("BRAS GAUCHE horizontal rangement");        
       break; 
   
       // BRAS DROIT PIVOT bas
     case 'h':
       BRAS_D_Piv.writeMicroseconds(2050); 
       Serial.println("BRAS DROIT PIVOT bas");        
       break; 
       
       // BRAS DROIT PIVOT milieu
     case 'i':
       BRAS_D_Piv.writeMicroseconds(1300); 
       Serial.println("BRAS DROIT PIVOT milieu");        
       break; 
       
       // BRAS DROIT PIVOT haut
     case 'j':
       BRAS_D_Piv.writeMicroseconds(500); 
       Serial.println("BRAS DROIT PIVOT haut");        
       break; 
       
        // BRAS DROITE horizontal chope avant vertical
     case 'p':
       BRAS_D_Hz.writeMicroseconds(2100); // rangement
       Serial.println("BRAS DROITE horizontal chope avant vertical");        
       break; 
       
      // BRAS DROITE horizontal chope echange vertical
     case 'q':
       BRAS_D_Hz.writeMicroseconds(2000); // rangement
       Serial.println("BRAS DROITE horizontal echange vertical");        
       break; 
       
       // BRAS DROITE horizontal chope lateral
     case 'r':
       BRAS_D_Hz.writeMicroseconds(1700); // rangement
       Serial.println("BRAS DROITE horizontal chope chope lateral");        
       break; 
       
       // BRAS DROITE horizontal chope rangement
     case 's':
       BRAS_D_Hz.writeMicroseconds(800); // rangement
       Serial.println("BRAS DROITE horizontal rangement");        
       break;  
   
       
     }
     
  }
  
}
  

void add_pulse_D() {
 
  // increase pulse count
 pulse_cnt_D++;
 return;
 } 
 
 void add_pulse_G() {
 
  // increase pulse count
 pulse_cnt_G++;
 return;
 } 
 
 unsigned long get_tsl_freq_D() {
    // copy pulse counter and multiply.
    // the multiplication is necessary for the current
    // frequency scaling level.  Please see the
    // OUTPUT SCALING section below for more info
 
  unsigned long freq = pulse_cnt_D * 100;
 
   // re-set pulse counter
  pulse_cnt_D = 0;
 
  return(freq);
}

unsigned long get_tsl_freq_G() {
    // copy pulse counter and multiply.
    // the multiplication is necessary for the current
    // frequency scaling level.  Please see the
    // OUTPUT SCALING section below for more info
 
  unsigned long freq = pulse_cnt_G * 100;
 
   // re-set pulse counter
  pulse_cnt_G = 0;
 
  return(freq);
}
 
