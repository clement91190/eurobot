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
    case 'P' :
        //Vitesse vit;
        // ordre de type Slave
        switch (ind)
        {
        case 0: 
            Serial.println("init_pince_mae");
            // pince en position de base en bas, ouverte, prete a retourner tout ce qu'elle trouve
            break;

        case 1: 
            Serial.println("stop pince mae");
            //met la pince en position basse en attente
            break;

        case 2: 
            Serial.println("range pince");
            //range la pince comme au depart.
            break;

        case 3: //recal
            Serial.println("prise du totem");
            // pince prete a chopper le totem 
            // des que l'ir le voit
            break;

        case 4: //recal
            Serial.println("prise du totem");
            // pince prete a chopper le totem 
            // des que l'ir le voit
            break;

        case 5: //BFCap
            Serial.println("pince pour pousser les feux");
            // faire tomber les feux de la bonne couleur
            break;
        }
        return;
    case 'F' :
        //Vitesse vit;
        // ordre de type Slave
        switch (ind)
        {
        case 0: 
            Serial.println("pose les fresques");
            io->poser_fresque();
            break;
        case 1: 
            Serial.println("range servos fresques");
            io->ranger_servo_fresque();
            break;
        case 2: 
            Serial.println("pose les fresques");
            io->envoi_filet();
            break;
        case 3: 
            Serial.println("shoot le filet");
            io->ranger_servo_filet();
            break;
        }
        return;

    }


}

