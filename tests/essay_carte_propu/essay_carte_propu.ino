
#include <Servo.h> 
 
const int sensMoteur1=4; // Constante pour la broche 2
const int vitesseMoteur1=2; // Constante pour la broche 3
const int sensMoteur2=8; // Constante pour la broche 2
const int vitesseMoteur2=6; // Constante pour la broche 3


const int AVANT=1; // constante sens moteur
const int ARRIERE=0; // constante sens moteur

// --- Déclaration des constantes des broches analogiques ---


// --- Déclaration des variables globales ---

int vitesse=105; // variable de vitesse
 
 
void setup() 
{ 
  //MD.attach(2); //PWM
  pinMode(sensMoteur1, OUTPUT); 
  pinMode(vitesseMoteur1, OUTPUT); 
  pinMode(sensMoteur2, OUTPUT); 
  pinMode(vitesseMoteur2, OUTPUT); 

  //MG.attach(4); //DIR
} 
 
void loop() 
{ 
  vitesse = 0;
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,AVANT); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,AVANT); // Marche avant 
  delay(3000); 

  
  vitesse = 150;
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,AVANT); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,AVANT); // Marche avant 
  delay(3000); 
  
  vitesse = 150;
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,ARRIERE); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,ARRIERE); // Marche avant 
  delay(3000); 
  
  
  vitesse = 255;
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,AVANT); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,AVANT); // Marche avant 
  delay(3000); 
  
  vitesse = 255;
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,ARRIERE); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,ARRIERE); // Marche avant 
  delay(3000); 
  /*
  vitesse = vitesse + 50; // incrémente la vitesse
  if (vitesse>255) vitesse=105; //RAZ vitesse
  
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,AVANT); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,AVANT); // Marche avant 
  
  delay(3000); 
  
  analogWrite(vitesseMoteur1, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur1,ARRIERE); // Marche avant 
  analogWrite(vitesseMoteur2, vitesse); // génère une impulsion PWM sur la broche de vitesse du moteur
  digitalWrite(sensMoteur2,ARRIERE); // Marche avant 
  
  delay(3000); 
  
  analogWrite(vitesseMoteur1,0); // génère une impulsion PWM sur la broche de vitesse du moteur
   analogWrite(vitesseMoteur2,0); // génère une impulsion PWM sur la broche de vitesse du moteur
  delay(3000);
  */
 
  
} 
