



int dir1 = 2;
int pwm1 = 5;
int dir2 = 13;
int pwm2 = 6;
int signe = 1;
int dir = 1;
int vitesse = 100;

void setup(){
 pinMode(dir1, OUTPUT);
 pinMode(dir2, OUTPUT);
 pinMode(pwm1, OUTPUT);
 pinMode(pwm2, OUTPUT);
 int prescalerVal = 0x07;
 //TCCR0B = xxxxx010, frequency is 8 kHz
 TCCR0B &= ~prescalerVal;
 //prescalerVal = 1;
 TCCR0B |= 1;
 
 digitalWrite(dir1, HIGH);
 digitalWrite(dir2, HIGH);
 
}

void loop(){
  if (vitesse >= 230)
  {
     signe = -1; 
  }
  if (vitesse <= 0)
  {
    signe = -1 * signe;
    dir = -1 * dir;
    if (dir == 1)
    {
       digitalWrite(dir1, HIGH);
       digitalWrite(dir2, HIGH);
   
    }
    else
    {
     digitalWrite(dir1, LOW);
     digitalWrite(dir2, LOW);
    
    }
  }
  
 vitesse = vitesse + 1 * signe;
  
 analogWrite(pwm1, vitesse);
 analogWrite(pwm2, vitesse);
  
    
   
  delay(800);
  
   
}
