// programme de test Mark Zero Actionneur secondaire

#include <Servo.h> 
 
// IR
const int PinAN_IRG = A0;
const int PinAN_IRD = A1;
int state_IRG = 0;
int state_IRD = 0;
const int seuil = 350;
 
// Pile
const int PinDi_PIlE_AVG = 6;
const int PinDi_PIlE_ARG = 8;
const int PinDi_PIlE_AVD = 7;
const int PinDi_PIlE_ARD = 9;
Servo PIlE_AVG;  
Servo PIlE_ARG;  
Servo PIlE_AVD;  
Servo PIlE_ARD;  

// TACLE
const int PinDi_TACLE_G = 4;
const int PinDi_TACLE_D = 5;
Servo TACLE_G;  
Servo TACLE_D;  

// BENNE
const int PinDi_BENNE_D = 11;
const int PinDi_BENNE_C = 10;
const int PinDi_BENNE_G = 12;
Servo BENNE_D;  
Servo BENNE_C; 
Servo BENNE_G; 

// RAMPE
const int PinDi_RAMPE_D = 3;
const int PinDi_RAMPE_G = 2;
Servo RAMPE_D;  
Servo RAMPE_G; 



void setup() 
{ 
  Serial.begin(9600);
  pinMode(PinAN_IRG, INPUT);
  pinMode(PinAN_IRD, INPUT);
  
  PIlE_AVG.attach(PinDi_PIlE_AVG);
  PIlE_ARG.attach(PinDi_PIlE_ARG);
  PIlE_AVD.attach(PinDi_PIlE_AVD);
  PIlE_ARD.attach(PinDi_PIlE_ARD);
  
  TACLE_G.attach(PinDi_TACLE_G);  
  TACLE_D.attach(PinDi_TACLE_D);  
  
  RAMPE_D.attach(PinDi_RAMPE_D);
  RAMPE_G.attach(PinDi_RAMPE_G);
  
  BENNE_D.attach(PinDi_BENNE_D);  
  BENNE_C.attach(PinDi_BENNE_C);
  BENNE_G.attach(PinDi_BENNE_G);
  
  //init
  PIlE_AVG.writeMicroseconds(580);
  PIlE_ARG.writeMicroseconds(1460);
  PIlE_AVD.writeMicroseconds(1540);
  PIlE_ARD.writeMicroseconds(530);
  
  TACLE_G.writeMicroseconds(600);
  TACLE_D.writeMicroseconds(1700);
  
  RAMPE_D.writeMicroseconds(1840);
  RAMPE_G.writeMicroseconds(1700);
  
  BENNE_C.writeMicroseconds(1300); //position fermee
  BENNE_G.writeMicroseconds(1930); //fermee
  BENNE_D.writeMicroseconds(930); //ferme
 
 } 
 
void loop() 
{ 
  if (Serial.available() > 0) {
     int inByte = Serial.read();

     switch (inByte) {
       
     // ouverture tacle gauche
     case 'a':
       TACLE_G.writeMicroseconds(1500);
       Serial.println("ouverture tacle gauche"); 
       break;
       
     // fermeture tacle gauche
     case 'b':    
      TACLE_G.writeMicroseconds(600); 
      Serial.println("fermeture tacle gauche");   
       break;
     
     
     // ouverture tacle droite
     case 'c':
       TACLE_D.writeMicroseconds(1020);
       Serial.println("ouverture tacle droite"); 
       break;
       
     // fermeture tacle droite
     case 'd':    
      TACLE_D.writeMicroseconds(1700); 
      Serial.println("fermeture tacle droite");   
       break;
     
     
     // expulsion rampe gauche
     case 'g':   
      Serial.println("expulsion rampe gauche"); 
      RAMPE_G.writeMicroseconds(1500);
      delay(1000);
      RAMPE_G.writeMicroseconds(1600);
       break;
       
      // expulsion rampe droite
     case 'h':   
      Serial.println("expulsion rampe droite"); 
      RAMPE_D.writeMicroseconds(1600);
      delay(1000);
      RAMPE_D.writeMicroseconds(1750);
       break;
      
     // vidange gauche
     case 'e':   
      Serial.println("vidange gauche"); 
      state_IRG = analogRead(PinAN_IRG);
      //Serial.print("IR Gauche : ");  
      //Serial.println(state_IRG); 
      while (state_IRG > seuil)
      {
        PIlE_AVG.writeMicroseconds(580);
        PIlE_ARG.writeMicroseconds(1460);
        delay(100);
        PIlE_AVG.writeMicroseconds(1000);
        PIlE_ARG.writeMicroseconds(1200);
        delay(500);
        PIlE_AVG.writeMicroseconds(580);
        PIlE_ARG.writeMicroseconds(1460);
        delay(300);
        state_IRG = analogRead(PinAN_IRG);
        //Serial.print("IR Gauche : ");  
        //Serial.println(state_IRG); 
      }
       break;
       
       
     // vidange droite
     case 'f':   
      Serial.println("vidange droite"); 
      state_IRD = analogRead(PinAN_IRD);
      //Serial.print("IR Droit : ");  
      //Serial.println(state_IRD); 
      while (state_IRD > seuil)
      {
        PIlE_AVD.writeMicroseconds(1540);
        PIlE_ARD.writeMicroseconds(530);
        delay(100);
        PIlE_AVD.writeMicroseconds(1080);
        PIlE_ARD.writeMicroseconds(760);
        delay(500);
        PIlE_AVD.writeMicroseconds(1540);
        PIlE_ARD.writeMicroseconds(530);
        delay(300);
        state_IRD = analogRead(PinAN_IRD);
        //Serial.print("IR Droit : ");  
        //Serial.println(state_IRD);
      }
       break;
       
       // benne central avale
     case 'i':   
      Serial.println("benne centrale avale"); 
      BENNE_C.writeMicroseconds(1300); //position fermee
      //BENNE_C.writeMicroseconds(1900); //position ouverte
       break;
       
      // benne central degueulle
     case 'j':   
      Serial.println("benne centrale degueulle"); 
      //BENNE_C.writeMicroseconds(1300); //position fermee
      BENNE_C.writeMicroseconds(1900); //position ouverte
       break;
       
       // benne bras ceuillette ferme
     case 'k':   
      Serial.println("benne bras ceuillette ferme"); 
      BENNE_D.writeMicroseconds(930); //ferme
      BENNE_G.writeMicroseconds(1930); //fermee
       break;
       
      // benne bras ceuillette mi-ouvert
     case 'l':   
      Serial.println("benne bras ceuillette mi-ouvert"); 
      BENNE_D.writeMicroseconds(1100); //mi-ouvert
      BENNE_G.writeMicroseconds(1750); //mi-fermee
       break;
       
      // benne bras ceuillette ouvert
     case 'm':   
      Serial.println("benne bras ceuillette ouvert"); 
      BENNE_D.writeMicroseconds(1400); //ouvert
      BENNE_G.writeMicroseconds(1400); //ouvert
       break;
       
     }
     
     
  }

  
/*

  myservoAV.writeMicroseconds(580);
  myservoAR.writeMicroseconds(1460);
  myservo_RT_G.writeMicroseconds(1000);
  //myservo_RT_D.writeMicroseconds(950);
  delay(2000);
  myservoAV.writeMicroseconds(1000);
  myservoAR.writeMicroseconds(1200);    
  myservo_RT_G.writeMicroseconds(1500);
  myservo_RT_D.writeMicroseconds(1200);
  delay(2000);
  myservo_RT_D.writeMicroseconds(1370);
  delay(2000);
  myservo_RT_D.writeMicroseconds(1100);
  delay(2000);
  */
} 
