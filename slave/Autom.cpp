#include "Autom.h"
/* Autom implementation*/

Autom::Autom():
    real_coord(),
    period_update_coords(10),
    period_pid_loop(30),
    gain_odo_g(0.357),
    gain_odo_d(0.357),
    gain_inter_odos(0.01309),
    last_ticG(0),
    last_ticD(0)
   {}

void Autom::update_cap(){
    /* attention ici, faudra tester la precision*/
    float cap = (ticD * gain_odo_d - ticG * gain_odo_g) * gain_inter_odos;       
    real_coord.set_cap(cap);
}

void Autom::update_coords(){
    /* peut etre prendre la moyenne des caps ou autre technique d'integration? */ 
    update_cap();
    int delta_ticG = ticG - last_ticG;
    int delta_ticD = ticD - last_ticD;
    float d = (delta_ticG * gain_odo_g + delta_ticD * gain_odo_d) * 0.5;   
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
// cmd_g = 120;
//    cmd_d = 120;

 bool fw_g = control.get_fw_g();
 bool fw_d = control.get_fw_d();

 //dir logic
 if (fw_g) digitalWrite(PIN_MOT_DIRG, HIGH);
 else digitalWrite(PIN_MOT_DIRG, LOW);
 if (fw_d) digitalWrite(PIN_MOT_DIRD, LOW);
 else digitalWrite(PIN_MOT_DIRD, HIGH);

 // cmd
 analogWrite(PIN_MOT_CMDG, cmd_g);
 analogWrite(PIN_MOT_CMDD, cmd_d);
 
 //write_cmd(cmd_g, cmd_d, fw_g, fw_d);
  write_cmd(cmd_g, cmd_d, fw_g, fw_d);
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
        /* add the pid here */
    }
}

ControlLoop* Autom::get_control(){
    return &control;
}
