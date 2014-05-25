#include "OrdersRaspberry.h"

//Fichier de définition des fonction pour les ordre reçu depuis la Gumstix
//


OrdersRaspberry::OrdersRaspberry(Autom* slave_) : period(100), slave(slave_), serial_count(0)
{
    treated=true;
    //s.resize(28);
}

void OrdersRaspberry::run()
{
    if(period.is_over())
    {
        period.reset();
        //Serial.println(memFree);
        treatSerial();
        //Serial.println("coucou");

        if ( !treated)
        {
            Serial.println("coucou");
            executeinstr();
            treated = true;
            stream.str("");
            serial_count = 0;
            s="";
            //s.resize(28);
        }
    }

}

void OrdersRaspberry::treatSerial()
{
    //int serial_count(0);
    char serial_char;
    //serial_count = 0;
    //Serial.println("coucou");

    // Ici on traite les différents messages reçus.
    while(Serial.available()>0 && serial_count < 28)
    {
        //delay(8);
        serial_char= Serial.read();
        //Serial.println(serial_char);
        if (serial_char =='\n')
        {
            Serial.println("Fin de ligne");
            //on ne traite que les instructions de minimum 2 char
            if (serial_count<1)
            {
                treated = true;
            }
            else
            {
                treated = false;
            }
            Serial.print("Enregistre : ");
            Serial.print(s.c_str());
            Serial.print("  serial_count  ");
            Serial.println(serial_count);
            return;
        }
        //Serial.println(serial_count);
        s = s + serial_char;
        serial_count = serial_count + 1;
    }
}

void OrdersRaspberry::executeinstr()
{
    //char ordre = cmdBuffer[bufindr][strchr_pointer];
    stream.str(s);
    std::string temp;
    stream>>temp;
    char ordre = temp[0];
    Serial.print("ordre ind = ");
    int ind = ((int)temp[1]) - 48;
    Serial.print(ordre);
    Serial.print("@");
    Serial.println(ind);
    
    std::string temp2;


    std::string cap;
    std::string x;
    std::string y;
    std::string z;
    int r = 0;
    int v = 0;
    int s=0;
    bool precis;
    Coord target;
    switch (ordre)
    {
  /*     
    case 'C' :
    //ordre de type camera control
		switch (ind)
        {
			case 0:
				Serial.print("inclinaison camera par terre");
				slave->camera_control()->inclinaison_par_terre();
				break;
				
			case 1:
				Serial.print("inclinaison camera 45deg");
				slave->camera_control()->inclinaison_mediane();
			break;
			
			case 2:
				Serial.print("inclinaison camera frontale");
				slave->camera_control()->inclinaison_frontale();
			break;
		}
        break;*/

    case 'D':
            Serial.println("debug");
            slave->get_control()->write_debug();
         
        switch (ind)
        {
           case 0:
                Serial.println("Reinit");
                slave->debuggDistanceInit();
                slave->debuggTicInit();
            break;

            case 1:
                Serial.print("distance gain gauche : ");
                Serial.println(slave->debuggDistance_g());
                Serial.print("distance gain droit : ");
                Serial.println(slave->debuggDistance_d());
                Serial.print("Tic total droit : ");
                Serial.println(slave->debuggTic_d());
                Serial.print("Tic total gauche : ");
                Serial.println(slave->debuggTic_g());
            break;
        }
        break;
    case 'S' :
        //Vitesse vit;
        // ordre de type Slave
        switch (ind)
        {
        case 0: // set x, y, cap
            Serial.print("SET X Y CAP :");
            stream >> x >> y >> cap;
          
            Serial.println(atoi(x.c_str()));
            if (atoi(x.c_str())== -1)
            {
                Serial.println("recal y -> x unknown");
                slave->setxycap_no_x(atoi(y.c_str()), 3.14 * atoi(cap.c_str())/ 180.0);
            }
            else
            {
                if (atoi(x.c_str())== -1)
                {
                    Serial.println("recal x -> y unknown");
                    slave->setxycap_no_y(atoi(x.c_str()), 3.14 * atoi(cap.c_str())/ 180.0);
                }
                else
                {
                    slave->setxycap(Coord(atoi(x.c_str()), atoi(y.c_str()), 3.14 * atoi(cap.c_str())/ 180.0));
                }

            }
            Serial.print(atoi(x.c_str()));
            Serial.print(" ");
            Serial.print(atoi(y.c_str()));
            Serial.print(" ");
            Serial.print(atoi(cap.c_str()));
            Serial.println(" ");
            break;

        case 1: //Recal
            Serial.println("GET X Y CAP: ");
            slave->get_control()->write_real_coords();
            
            break;

        case 2: //Recal
            Serial.println("Recalage");
            slave->get_control()->recaler();
        //    slave->set<Recaler>(Marche::AVANT);
            break;
        case 3: //BFCap
            if(!(stream>>cap>>precis))
            {
                Serial.print ("err ");
            }
            target = Coord(0, 0, 3.14 * atoi(cap.c_str()) / 180.0);
            Serial.print ("BFCap ");
            Serial.println(atoi(cap.c_str()));
            slave->get_control()->set_BF(BFCAP, target); 
         //   slave->set<BFCap>(cap,precis);
            break;
        case 4: //BFAvance

            stream >> x >> v;
            Serial.println("BFAvance");
            Serial.print(" ");
            Serial.print(atoi(x.c_str()));
            Serial.print(" ");
            Serial.println(v);
            target = Coord(atoi(x.c_str()), 0, 0);
            slave->get_control()->set_BF(BFFW, target); 

            //vit=determinerVitesse(v);
      //      slave->set<BFAvance>(x , vit );
            break;
        case 5: //BFDroite
            
            stream>>x >>y >> cap>> v;

            Serial.print ("BFDroite ");
            Serial.print(atoi(x.c_str()));
            Serial.print(" ");
            Serial.print(atoi(y.c_str()));
            Serial.print(" ");
            Serial.println(atoi(cap.c_str()));
            //Serial.print(" ");
            //Serial.println(v);

            target = Coord(atoi(x.c_str()), atoi(y.c_str()), 3.14 * atoi(cap.c_str())/ 180.0);
            slave->get_control()->set_BF(BFXYCAP, target); 
            //vit=determinerVitesse(v);
        //    slave->set<BFDroite>(x,y,cap ,vit,precis);
            break;
        case 6: //BFCercle
            stream>> x >> y >> r >> cap>> s >> v;
            /*
            Serial.println("BFCercle");
            Serial.print(x);
            Serial.print(" ");
            Serial.print(y);
            Serial.print(" ");
            Serial.print(r);
            Serial.print(" ");
            //Serial.print(cap);
            Serial.print(" ");
            Serial.print(s);
            Serial.print(" ");
            Serial.println(v);
            //vit=determinerVitesse(v);
         //   slave->set<BFCercle>(x,y,r, cap ,s,  vit);
            break;*/
        case 7: //set speed
            Serial.println("Set speed");
            stream >> x;
            Serial.print("speed ");
            Serial.println(atoi(x.c_str()));

            slave->get_control()->set_speed(atoi(x.c_str()));

         //   slave->set<ARRET_ROBOT>();
            break;
        case 8: //Reprendre
            Serial.println(" SET GAINS (DEBUG) ");

            stream>> x >> y >> z >> cap;
           
            if (atoi(x.c_str()) == 1 )
            {
                Serial.println("setting gains for dep");
                Serial.println(" Kp KI KV");
                
                slave->get_control()-> setTuningDep(atof(y.c_str()), atof(z.c_str()), atof(cap.c_str()));
                slave->get_control()->write_debug();

            }
            else
            {
                Serial.println(" setting gains for cap");
                Serial.println(" Kp KI KV");
                
                slave->get_control()-> setTuningCap(atof(y.c_str()), atof(z.c_str()), atof(cap.c_str()));
                slave->get_control()->write_debug();


            }
            break;

        case 9:
		
			//slave->get_control()->debuggDistance_d();
            /*
            stream>> x >> y >> cap;
            Serial.println("setXYCap");
            Serial.print(x);
            Serial.print(" ");
            Serial.print(y);
            Serial.print(" ");
            //Serial.print(cap);
            Serial.println(" ");
          //  slave->setXYCap(x, y ,cap);*/
            break;
        }
        return;
    }


}

