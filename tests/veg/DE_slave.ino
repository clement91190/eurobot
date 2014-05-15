// prgramme de test  Le Debile Slave
#include <Servo.h>
#include <MPU6050.h>
#include <I2Cdev.h>
#include <Wire.h>

int DEBUG = 1;
int vitesse = 0;

//IMU
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// bumpers
const int Pin_AN_Start = A2;
const int Pin_AN_Couleur = A3;
const int Pin_AN_Strat1 = A7;
const int Pin_AN_Strat2 = A6;

// sonar
const int Pin_AN_Sonar = A1;

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
unsigned long pulse_cnt_G = 0;
unsigned long pulse_cnt_D = 0;

//servos CAM
const int PinDi_CAM = 6;
Servo CAM;

int state_START = HIGH;
int state_COLOR = HIGH;
int state_STRAT1= HIGH;
int state_STRAT2 = HIGH;
int state_SONAR = 0;

const int AVANT = 1; // constante sens moteur
const int ARRIERE = 0; // constante sens moteur

void setup() 
{ 
  
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    //Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  
  
  attachInterrupt(1, add_pulse_D, RISING);
  attachInterrupt(0, add_pulse_G, RISING);
  Serial.begin(9600);
  pinMode(PinDi_ODO_G_B, INPUT);
  pinMode(PinDi_ODO_D_B, INPUT);
  pinMode(Pin_AN_Start, INPUT);
  pinMode(Pin_AN_Couleur, INPUT);
  pinMode(Pin_AN_Strat1, INPUT);
  pinMode(Pin_AN_Strat2, INPUT);
  pinMode(PinDi_MoteurD_SENS, OUTPUT);
  pinMode(PinDi_MoteurG_SENS, OUTPUT);
  pinMode(PinDi_MoteurD_PWM, OUTPUT);
  pinMode(PinDi_MoteurG_PWM, OUTPUT);
  pinMode(Pin_AN_Sonar, INPUT);
  CAM.attach(PinDi_CAM);
  CAM.writeMicroseconds(1000);
} 
 
void loop() 
{ 
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //#ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    //#endif
  CAM.writeMicroseconds(1200); //POSITION REGARDE PAR TERRE
  //delay(1000);
  //CAM.writeMicroseconds(1950); // POSITION FACIALE
  //delay(1000);  
  // read mesure
  state_START = analogRead(Pin_AN_Start);
  state_COLOR = analogRead(Pin_AN_Couleur);
  state_STRAT1= analogRead(Pin_AN_Strat1);
  state_STRAT2 = analogRead(Pin_AN_Strat2);
  state_SONAR = analogRead(Pin_AN_Sonar);

  
  if(DEBUG){
    Serial.print("SONAR : ");  
    Serial.println(state_SONAR);
    Serial.print("Bouton start : ");  
    Serial.println(state_START); 
    Serial.print("Bouton couleur : ");  
    Serial.println(state_COLOR); 
    Serial.print("Bouton strat 1 : ");  
    Serial.println(state_STRAT1); 
    Serial.print("Bouton strat 2 : ");  
    Serial.println(state_STRAT2); 
    //Serial.println();
    Serial.println("tic G : ");
    Serial.println(pulse_cnt_G);
    Serial.println("tic D : ");
    Serial.println(pulse_cnt_D);
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


void add_pulse_D() {
 if(digitalRead(PinDi_ODO_D_B)==HIGH)
 {
   pulse_cnt_D++;
 }
 else
 {
   pulse_cnt_D--;
 }
  // increase pulse count
 
 return;
 } 
 
 void add_pulse_G() {
 if(digitalRead(PinDi_ODO_G_B)==LOW)
 {
   pulse_cnt_G++;
 }
 else
 {
   pulse_cnt_G--;
 }
  // increase pulse count

 return;
 } 
