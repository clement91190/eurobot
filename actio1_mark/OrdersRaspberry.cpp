#include "OrdersRaspberry.h"

//Fichier de définition des fonction pour les ordre reçu depuis la Gumstix
//


OrdersRaspberry::OrdersRaspberry(IO* io_) : period(100), io(io_), serial_count(0)
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
    int y = 0;
    int r = 0;
    int v = 0;
    int s=0;
    bool precis;
    Coord target;
    switch (ordre)
    {
     case 'S':
        Serial.println("STOP tout ");
        io->stop();

    case 'D':
        Serial.println(" debug");
        io->write_debug();

    case 'C':
        switch (ind)
        {
            case 0:
                Serial.println ("couleur rouge");
                io->set_couleur(0);
            break;
        
            case 1:
                Serial.println("couleur jaune");
                io->set_couleur(1);
            break;
        
        }
        break;

    case 'B' :
        //Vitesse vit;
        // ordre de type Slave
        switch (ind)
        {
         case 0: 
            Serial.println("Aller a etat attente ");
            io->bras_actif();
            break;

        case 1: 
            Serial.println("Ranger");
            io->range_bras();
            break;


        case 2: 
            Serial.println(" activation automatique des irs");
            io->active_irs();
            break;


        case 3: 
            Serial.println("desactive irs");
            io->desactive_irs();
            break;


        case 4: 
            Serial.println("Prise centre");
            stream >> x;
            Serial.println("de ma couleur ? ");
            Serial.println(atoi(x.c_str()));
            io->prise_centre(atoi(x.c_str()));
            break;


        case 5: 
            Serial.println("confirme prise");
            io->conf_prise_centre();
            break;
       }
        return;
    }


}

