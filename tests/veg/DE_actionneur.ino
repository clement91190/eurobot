// prgramme de test  Mark zero Slave

#include <Servo.h>


int DEBUG = 1;



// bumpers Asc
const int Pin_AN_BumperAsc_BAS = A2;
const int Pin_AN_BumperAsc_HAUT = A3;
int state_BumperAsc_BAS = 0;
int state_BumperAsc_HAUT = 0;

// Moteur
const int PinDi_Moteur_SENS = 4;
const int PinDi_Moteur_PWM = 5;
int vitesse = 0;

// ODOs
const int PinDi_ODO_A = 2; //interruption
const int PinDi_ODO_B = 13;
unsigned long pulse_cnt_ODO = 0;


// capteur couleur
const int Pin_Di_Couleur = 3;
unsigned long pulse_cnt_COLOR = 0;

// Capteur IR
const int Pin_An_IR = A0;
int state_IR = 0;
const int seuilIR = 0; //inf


//Servos Fresques
const int Pin_Di_FRESK_G = 11;
const int Pin_Di_FRESK_D = 12;
Servo FRESK_G;
Servo FRESK_D;

//Servos Arbalette
const int Pin_Di_ARBAL_G = 9;
const int Pin_Di_ARBAL_D = 10;
Servo ARBAL_G;
Servo ARBAL_D;

//Servos PINCE
const int Pin_Di_PINCE_G = 7;
const int Pin_Di_PINCE_D = 8;
const int Pin_Di_PINCE_C = 6;
Servo PINCE_G;
Servo PINCE_D;
Servo PINCE_C;



/* VARIABLE POUR LES CAPTEURS COULEURS */
// 1000ms = 1s
#define READ_TM 1000
// two variables used to track time
unsigned long cur_tm = millis();
unsigned long pre_tm = cur_tm;
 // we'll need to access the amount
 // of time passed
unsigned int tm_diff = 0;
unsigned long frequency = 0;



void setup() 
{ 
  Serial.begin(9600);

  attachInterrupt(0, add_pulse_ODO, RISING);
  attachInterrupt(1, add_pulse_COLOR, RISING);
  
  pinMode(PinDi_ODO_B, INPUT);
  pinMode(PinDi_ODO_A, INPUT);
  
  pinMode(Pin_Di_Couleur, INPUT);
  pinMode(Pin_AN_BumperAsc_HAUT, INPUT);
  pinMode(Pin_AN_BumperAsc_BAS, INPUT);
  pinMode(Pin_An_IR, INPUT);
  pinMode(PinDi_Moteur_PWM, OUTPUT);
  pinMode(PinDi_Moteur_SENS, OUTPUT);
  
 
  FRESK_G.attach(Pin_Di_FRESK_G);
  FRESK_G.writeMicroseconds(1900); //ferme
  //FRESK_G.writeMicroseconds(1500); //ouvert
  FRESK_D.attach(Pin_Di_FRESK_D);
  FRESK_D.writeMicroseconds(1100);
  //FRESK_D.writeMicroseconds(1500); //ouvert
  
  ARBAL_G.attach(Pin_Di_ARBAL_G);
  ARBAL_D.attach(Pin_Di_ARBAL_D);
  ARBAL_G.writeMicroseconds(1100); //fermeture
  //ARBAL_G.writeMicroseconds(1600); //ouverture
  ///ARBAL_G.writeMicroseconds(1500); //middle
  
  ARBAL_D.writeMicroseconds(1900); //fermeture
  //ARBAL_D.writeMicroseconds(1400); //ouverture
  
  
  
  
  
  PINCE_G.attach(Pin_Di_PINCE_G);
  PINCE_D.attach(Pin_Di_PINCE_D);
  PINCE_G.writeMicroseconds(1500); //vertical
  PINCE_D.writeMicroseconds(1500); //vertical
  PINCE_G.writeMicroseconds(2100); //faire tomber le feux
  PINCE_G.writeMicroseconds(2200); //saisie
  PINCE_D.writeMicroseconds(800); //saisie
  
  
  
  PINCE_G.writeMicroseconds(1420); //rangement
  PINCE_D.writeMicroseconds(1580); //rangement
  //PINCE_G.writeMicroseconds(1800); //moiss bat menad'
  //PINCE_D.writeMicroseconds(1200); //moiss bat menad'
  
  
  
  PINCE_C.attach(Pin_Di_PINCE_C);
  PINCE_C.writeMicroseconds(1340); //vertical
  //PINCE_C.writeMicroseconds(560); //retourne le truc
  //PINCE_C.writeMicroseconds(2140); //pince par terre
  
  
  

  //init servos et variable
  //BRAS_G_Hz.writeMicroseconds(1500); // rangement

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
   frequency = get_tsl_freq();
 
 }



  //lecture des autres capteurs IR et Bumpers
  state_IR = analogRead(Pin_An_IR);
  state_BumperAsc_BAS = analogRead(Pin_AN_BumperAsc_BAS);  
  state_BumperAsc_HAUT = analogRead(Pin_AN_BumperAsc_HAUT);  
  delay(100);

  
  
  if(DEBUG)
  {
    Serial.print("IR  : ");  
    Serial.println(state_IR); 
    Serial.print("Bumper Asc HAUT : ");  
    Serial.println(state_BumperAsc_HAUT); 
    Serial.print("Bumper Asc BAS : ");  
    Serial.println(state_BumperAsc_BAS); 
    Serial.print("TIC ODO : ");  
    Serial.println(pulse_cnt_ODO); 
    Serial.print("frequence capteur couleur : ");  
    Serial.println(frequency); 
    Serial.print("couleur capteur couleur Gauche : ");  
    if(frequency > 3000000 & frequency < 8000000){
      Serial.println("rouge");
    }
    else{
     if(frequency > 8000000){
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
       
     // moteur ascenseur stop
     case 'a':
       vitesse = 0;
       analogWrite(PinDi_Moteur_PWM, vitesse);
       digitalWrite(PinDi_Moteur_SENS, 0);
       Serial.println("arret moteur ascenseur"); 
       break;
     
     
     // moteur monte
     case 'b':
       vitesse = 250;
       Serial.println("ascenseur monte"); 
       while(state_BumperAsc_HAUT > 1000)
       {
         analogWrite(PinDi_Moteur_PWM, vitesse);
         digitalWrite(PinDi_Moteur_SENS, 0);
         delay(50);
         state_BumperAsc_HAUT = analogRead(Pin_AN_BumperAsc_HAUT);  
       }
       vitesse = 0;
       analogWrite(PinDi_Moteur_PWM, vitesse);
       digitalWrite(PinDi_Moteur_SENS, 0);
       
       break;
     
     
     //moteur  descends
     case 'c':
       vitesse = 100;
       Serial.println("ascenseur descend"); 
       while(state_BumperAsc_BAS > 1000)
       {
         analogWrite(PinDi_Moteur_PWM, vitesse);
         digitalWrite(PinDi_Moteur_SENS, 1);
         delay(50);
         state_BumperAsc_BAS = analogRead(Pin_AN_BumperAsc_BAS);  
       }
       vitesse = 0;
       analogWrite(PinDi_Moteur_PWM, 0);
       digitalWrite(PinDi_Moteur_SENS, 0);
       break;
       
       
     //Fresque rangement
     case 'd':
       Serial.println("Fresque rangement"); 
       FRESK_G.writeMicroseconds(1900); //fermee
       FRESK_D.writeMicroseconds(1100); //fermee
       break;
     
     //Fresque largage
     case 'e':
       Serial.println("Fresque largage"); 
       FRESK_G.writeMicroseconds(1500); //ouverture
       FRESK_D.writeMicroseconds(1500); //ouverture
       break;  
     
     
     //filet replis
     case 'f':
       Serial.println("filet replis"); 
       ARBAL_G.writeMicroseconds(1100); //fermeture
       //ARBAL_G.writeMicroseconds(1600); //ouverture
       ARBAL_D.writeMicroseconds(1900); //fermeture
       //ARBAL_D.writeMicroseconds(1400); //ouverture
       break;    
       
     //filet replis
     case 'g':
       Serial.println("filet detente de Joule Gay Lussac :p"); 
       //ARBAL_G.writeMicroseconds(1100); //fermeture
       ARBAL_G.writeMicroseconds(1600); //ouverture
       //ARBAL_D.writeMicroseconds(1900); //fermeture
       ARBAL_D.writeMicroseconds(1400); //ouverture
       break;      
       
       //pince laterale rangement
     case 'h':
       Serial.println("pince laterale rangement"); 
        PINCE_G.writeMicroseconds(1420); //rangement
        PINCE_D.writeMicroseconds(1580); //rangement
        break; 
        
         //pince laterale moiss bat'
     case 'i':
       Serial.println("pince moiss bat'"); 
        PINCE_G.writeMicroseconds(1800); //moiss bat menad'
        PINCE_D.writeMicroseconds(1200); //moiss bat menad'
        break; 
        
           //pince laterale pince!!!
     case 'j':
       Serial.println("pince laterale pince!!!"); 
        PINCE_G.writeMicroseconds(2200); //saisie
        PINCE_D.writeMicroseconds(800); //saisie
        break; 
        
             //pince central debout/ranger!!!
     case 'k':
       Serial.println("pince central range toi sale bat'!!!"); 
        PINCE_C.writeMicroseconds(1340); //vertical
        break; 
        
        
             //pince centrale endroit!!!
     case 'l':
       Serial.println("pince central moiss bat'!!!"); 
        PINCE_C.writeMicroseconds(2140); //pince par terre
        break; 
        
                    //pince centrale envers!!!
     case 'm':
       Serial.println("pince central retourne toi!!!"); 
        PINCE_C.writeMicroseconds(560); //retourne le truc
        break; 
        
        
  //
  //
        
        
        
        
        

       
       
       case 'z':
         Serial.println("test MAE bras"); 
         int tourne = 0;
         int etat = 0;
         
         while(tourne){
           
           if (Serial.available() > 0) {
            int inByte = Serial.read();
            switch (inByte) {
              
              case 'p':
              Serial.println("patte");
              if(etat == 0){
               Serial.println("pousse!");
               etat = 1; 
              }
              if(etat == 1){
               Serial.println("rangement");
               etat = 0; 
              }
              break;
              
              case 'r':
              Serial.println("recolte");
              if(etat == 0){
               Serial.println("pousse!");
               etat = 2; 
              }
              if(etat == 1){
               Serial.println("rangement");
               etat = 0; 
              }
              break;
              
              case 'z':
              Serial.println("exit");
              tourne = 0;
              break;
              
            }
           
           switch (etat) {
             case '0':
             Serial.println("Initialisation"); 
             vitesse = 250;
             Serial.println("ascenseur monte"); 
             while(state_BumperAsc_HAUT > 1000)
             {
               analogWrite(PinDi_Moteur_PWM, vitesse);
               digitalWrite(PinDi_Moteur_SENS, 0);
               delay(50);
               state_BumperAsc_HAUT = analogRead(Pin_AN_BumperAsc_HAUT);  
             }
             vitesse = 0;
             analogWrite(PinDi_Moteur_PWM, vitesse);
             digitalWrite(PinDi_Moteur_SENS, 0);
              PINCE_G.writeMicroseconds(1420); //rangement
              PINCE_D.writeMicroseconds(1580); //rangement
              PINCE_C.writeMicroseconds(1340); //vertical
             break;
             
             
             case '1':
             Serial.println("coup de pate"); 
              PINCE_D.writeMicroseconds(1580); //rangement
             break;
             
           }
           
           
           
           
           
           
         }
           
           
           
         }
         
         
         
       
       
       
       
     }
     
  }
  
}
  
  
  
  
 /*
 interruption ODO
 */
void add_pulse_ODO() {
 if(digitalRead(PinDi_ODO_B)==LOW)
 {
   pulse_cnt_ODO++;
 }
 else
 {
   pulse_cnt_ODO--;
 }
  // increase pulse count
 
 return;
 } 
 
 
 /*
 interruption capteur couleur
 */
void add_pulse_COLOR() {
 
  // increase pulse count
 pulse_cnt_COLOR++;
 return;
 } 
 
 
 
 /*
 renvoie la frequence calculer par le capteur couleur
 */
 unsigned long get_tsl_freq() {
    // copy pulse counter and multiply.
    // the multiplication is necessary for the current
    // frequency scaling level.  Please see the
    // OUTPUT SCALING section below for more info
 
  unsigned long freq = pulse_cnt_COLOR * 100;
 
   // re-set pulse counter
  pulse_cnt_COLOR = 0;
 
  return(freq);
}

