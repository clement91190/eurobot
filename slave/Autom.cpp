#include "Autom.h"
/* Autom implementation*/

#define GAIN_ODO_G_PMI 0.3390043
#define GAIN_ODO_D_PMI 0.3381305
#define GAIN_ODO_inter_PMI 0.0131697

#define GAIN_ODO_G_MARK 0.357
#define GAIN_ODO_D_MARK 0.357
#define GAIN_ODO_inter_MARK 0.004299


Autom::Autom():
    real_coord(),
    period_update_coords(10),
    control(),
    period_pid_loop(40),
#ifdef PMI
    gain_inter_odos(GAIN_ODO_inter_PMI), //0.011971135478867 //0.01309
    gain_odo_g(GAIN_ODO_G_PMI),	//0.357
    gain_odo_d(GAIN_ODO_D_PMI), //0.351618 //0.357
    camera(),
#else
    gain_inter_odos(GAIN_ODO_inter_MARK), //0.01309
    gain_odo_g(GAIN_ODO_G_MARK),	
    gain_odo_d(GAIN_ODO_D_MARK),
#endif
    last_ticG(0),
    last_ticD(0),
    distance_g(0),
    distance_d(0)
   {
    send_cmd();
   }
   
   
   Camera::Camera()
   {
	   #ifdef PMI
			camera.attach(PIN_SERVO_CAM);
			inclinaison_par_terre();
	   #else
			//placer la camera du gros si ca marche
			//camera.attach(PIN_SERVO_CAM);
	   #endif
   }
   
   void Camera::inclinaison_frontale()
   {
	   #ifdef PMI
			camera.writeMicroseconds(1950);
	   #else
	   
	   #endif
   }
   void Camera::inclinaison_mediane()
   {
	   #ifdef PMI
			camera.writeMicroseconds(1575);
	   #else
	   
	   #endif
   }
    void Camera::inclinaison_par_terre()
   {
	   #ifdef PMI
			camera.writeMicroseconds(1200);
	   #else
	   
	   #endif
   }
   

void Autom::update_cap(){
    /* attention ici, faudra tester la precision*/
    float cap = (ticD * gain_odo_d - ticG * gain_odo_g) * gain_inter_odos;       
    real_coord.set_cap(real_coord.get_cap() + cap);
    //Serial.println(ticG);
}

void Autom::update_coords(){
    /* peut etre prendre la moyenne des caps ou autre technique d'integration? */ 
    update_cap();
    int delta_ticG = ticG; //- last_ticG;
    int delta_ticD = ticD; //  - last_ticD;
    reset_tics_odos();
    float d = (delta_ticG * gain_odo_g + delta_ticD * gain_odo_d) * 0.5;
    
    //pour test et debug de gain 
    distance_d += delta_ticD * gain_odo_d;
    distance_g += delta_ticG * gain_odo_g;
    
        
    real_coord.forward_translation(d);
    /* maybe add a delta cond on distance to avoid noise ? */
    last_ticD = ticD;
    last_ticG = ticG;
};

void Autom::reset_tics_odos(){
    ticG = 0;
    ticD = 0;
}

void Autom::send_cmd(){

 int cmd_g = control.get_cmd_g();
 int cmd_d = control.get_cmd_d();
 //cmd_g = 20;
 //cmd_d = 20; // for testing motor signs

 bool fw_g = control.get_fw_g();
 bool fw_d = control.get_fw_d();

 //fw_g = true;
 //fw_d = true;

 //dir logic
#ifdef PROUT
 if (!fw_g) digitalWrite(PIN_MOT_DIRG, HIGH);
 else digitalWrite(PIN_MOT_DIRG, LOW);
 if (!fw_d) digitalWrite(PIN_MOT_DIRD, LOW);
 else digitalWrite(PIN_MOT_DIRD, HIGH);


#else
 if (fw_g) digitalWrite(PIN_MOT_DIRG, HIGH);
 else digitalWrite(PIN_MOT_DIRG, LOW);
 if (fw_d) digitalWrite(PIN_MOT_DIRD, LOW);
 else digitalWrite(PIN_MOT_DIRD, HIGH);
#endif
  // cmd
 analogWrite(PIN_MOT_CMDG, cmd_g);
 analogWrite(PIN_MOT_CMDD, cmd_d);
 
 //write_cmd(cmd_g, cmd_d, fw_g, fw_d);
  //write_cmd(cmd_g, cmd_d, fw_g, fw_d);
}
void Autom::stop()
{
    analogWrite(PIN_MOT_CMDG, 0);
    analogWrite(PIN_MOT_CMDD, 0);
}

void Autom::write_cmd(int cmd_g, int cmd_d, bool fw_g, bool fw_d){
    Serial.print(" cmdG  ");
    Serial.print(cmd_g);
    Serial.print("  G  ");
    Serial.print(fw_g);
    Serial.print("  cmdD  ");
    Serial.print(cmd_d);
    Serial.print("  D  ");
    Serial.print(fw_d);
}

void Autom::run(){
    /* this is where all the magic happends */
    
    if (period_update_coords.is_over())
    {
        period_update_coords.reset();
        update_coords();
        //real_coord.write_serial();
    }
    if (period_pid_loop.is_over())
    {
        period_pid_loop.reset();
        control.run(real_coord);
        send_cmd();
    }
}

ControlLoop* Autom::get_control(){
    return &control;
}


void write_serial_strat()
{
   Serial.print("* COUL ");
   Serial.println(digitalRead(PIN_AN_COULEUR));
   Serial.print("* STRAT ");
   Serial.print(digitalRead(PIN_DI_STRAT1));
   Serial.print(" ");
   Serial.println(digitalRead(PIN_DI_STRAT2));
    
    }


void Autom::setxycap(Coord new_coord)
{
    real_coord = new_coord;
    get_control()->setxycap(new_coord);
}

void Autom::debuggDistanceInit()
{
	distance_g = 0;
	distance_d = 0;
}


float Autom::debuggDistance_g()
{
	return distance_g;
}

float Autom::debuggDistance_d()
{
	return distance_d;
}

