// prgramme de test  Mark zero Slave

int DEBUG = 1;
int vitesse = 0;

// bumpers
const int Pin_AN_Start = A2;
const int Pin_AN_Couleur = A3;
const int Pin_DI_Strat1 = 6;
const int Pin_DI_Strat2 = 7;
const int Pin_AN_RecalageGauche = A6;
const int Pin_AN_RecalageDroite = A7;

// sonar
const int Pin_AN_SonarG = A1;
const int Pin_AN_SonarD = A0;

// Moteur
const int PinDi_MoteurD_SENS = 8;
const int PinDi_MoteurD_PWM = 9;
const int PinDi_MoteurG_SENS = 10;
const int PinDi_MoteurG_PWM = 11;

// ODOs
const int PinDi_ODO_G_A = 2; //interruption
const int PinDi_ODO_G_B = 4;
const int PinDi_ODO_D_A = 3; //interruption
const int PinDi_ODO_D_B = 5;

int state_START = HIGH;
int state_COLOR = HIGH;
int state_STRAT1= HIGH;
int state_STRAT2 = HIGH;
const int AVANT=1; // constante sens moteur
const int ARRIERE=0; // constante sens moteur

void setup() 
{ 
  Serial.begin(9600);
  pinMode(Pin_AN_Start, INPUT);
  pinMode(Pin_AN_Couleur, INPUT);
  pinMode(Pin_DI_Strat1, INPUT);
  pinMode(Pin_DI_Strat2, INPUT);
  pinMode(PinDi_MoteurD_SENS, OUTPUT);
  pinMode(PinDi_MoteurG_SENS, OUTPUT);
  pinMode(PinDi_MoteurD_PWM, OUTPUT);
  pinMode(PinDi_MoteurG_PWM, OUTPUT);
} 
 
void loop() 
{ 
  // read mesure
  state_START = digitalRead(Pin_AN_Start);
  state_COLOR = digitalRead(Pin_AN_Couleur);
  state_STRAT1= digitalRead(Pin_DI_Strat1);
  state_STRAT2 = digitalRead(Pin_DI_Strat2);

  if(DEBUG){
    Serial.print("Bouton start : ");  
    Serial.println(state_START); 
    Serial.print("Bouton couleur : ");  
    Serial.println(state_COLOR); 
    Serial.print("Bouton strat 1 : ");  
    Serial.println(state_STRAT1); 
    Serial.print("Bouton strat 2 : ");  
    Serial.println(state_STRAT2); 
    Serial.println();
    delay(500);
  }
  

  
  if (Serial.available() > 0) {
     int inByte = Serial.read();

     switch (inByte) {
       
     // stop
     case 'a':
       vitesse = 0;
       analogWrite(PinDi_MoteurD_PWM, vitesse);
       analogWrite(PinDi_MoteurG_PWM, vitesse);
       digitalWrite(PinDi_MoteurD_SENS, 0);
       digitalWrite(PinDi_MoteurG_SENS, 1);
       Serial.println("arret moteur"); 
       break;
     
     
     // avant
     case 'b':
       vitesse = 20;
       analogWrite(PinDi_MoteurD_PWM, vitesse);
       analogWrite(PinDi_MoteurG_PWM, vitesse);
       digitalWrite(PinDi_MoteurD_SENS, 0);
       digitalWrite(PinDi_MoteurG_SENS, 1);
       Serial.println("marche avant"); 
       break;
     
     
     // arriere
     case 'c':
       vitesse = 20;
       analogWrite(PinDi_MoteurD_PWM, vitesse);
       analogWrite(PinDi_MoteurG_PWM, vitesse);
       digitalWrite(PinDi_MoteurD_SENS, 1);
       digitalWrite(PinDi_MoteurG_SENS, 0);
       Serial.println("marche arriere"); 
       break;
     
     
     // rotation horaire
     case 'd':
       vitesse = 20;
       analogWrite(PinDi_MoteurD_PWM, vitesse);
       analogWrite(PinDi_MoteurG_PWM, vitesse);
       digitalWrite(PinDi_MoteurD_SENS, 1);
       digitalWrite(PinDi_MoteurG_SENS, 1);
       Serial.println("rotation horaire"); 
       break;
     
     
      // rotation trigo
     case 'e':
       vitesse = 20;
       analogWrite(PinDi_MoteurD_PWM, vitesse);
       analogWrite(PinDi_MoteurG_PWM, vitesse);
       digitalWrite(PinDi_MoteurG_SENS, 0);
       digitalWrite(PinDi_MoteurD_SENS, 0);
       Serial.println("rotation trigo"); 
       break;
     }
     
  }
  
} 
