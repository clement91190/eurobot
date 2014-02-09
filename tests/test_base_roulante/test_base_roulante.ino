//pin alloc
int pin_odo_incD = 0; // == pin 2
int pin_odo_incG = 1; // == pin 3
int pin_odo_dirD = 8;
int pin_odo_dirG = 9;

int pin_mot_cmdD = 5;
int pin_mot_cmdG = 6;
int pin_mot_dirD = 10;
int pin_mot_dirG = 11;

//variables globales

long ticG;
long ticD;
int vitesse = 100;
int dir = 1;
int signe = 1;

void inc_ticG(){
  int chb = digitalRead(pin_odo_dirG) ;
 if (chb == 1)
{
  ticG = ticG + 1;
}
  else
  {
   ticG = ticG - 1; 
  }
}

void inc_ticD(){
  int chb = digitalRead(pin_odo_dirD) ;
 if (chb == 1)
{
  ticD = ticD + 1;
}
  else
  {
   ticD = ticD - 1; 
  }
}


void print_tic_odo()
{
    Serial.print(" odoG :");
    Serial.print(ticG);
    Serial.print(" odoD :");
    Serial.println(ticD);
}

void setup()
{
  attachInterrupt(pin_odo_incG, inc_ticG, RISING);
  attachInterrupt(pin_odo_incD, inc_ticD, RISING);
  Serial.begin(9600) ; 
  pinMode(pin_mot_dirG, OUTPUT);
  pinMode(pin_mot_dirD, OUTPUT);
  pinMode(pin_mot_cmdG, OUTPUT);
  pinMode(pin_mot_cmdD, OUTPUT);
  int prescalerVal = 0x07;
  //TCCR0B = xxxxx010, frequency is 8 kHz
  TCCR0B &= ~prescalerVal;
  //prescalerVal = 1;
  TCCR0B |= 1;
 
}

void write_cmd(){
 if (dir == 1)
    {
       digitalWrite(pin_mot_dirG, LOW);
       digitalWrite(pin_mot_dirD, HIGH);
   
    }
    else
    {
     digitalWrite(pin_mot_dirG, HIGH);
     digitalWrite(pin_mot_dirD, LOW);
    
    } 
 analogWrite(pin_mot_cmdG, vitesse);
 analogWrite(pin_mot_cmdD, vitesse);
  
}

void loop(){
  
  if (ticD <= -30){
    dir = 1; 
  }
  if (ticD >=0){
    dir = -1;
  }
  print_tic_odo();
  
  
 write_cmd();   
 Serial.println(vitesse);
  Serial.print(dir); 
  delay(800);
  
}

