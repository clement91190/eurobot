#include "OrdersRaspberry.h"

//Fichier de définition des fonction pour les ordre reçu depuis la Gumstix
//


OrdersRaspberry::OrdersRaspberry(Autom* slave_) : period(100), slave(slave_)
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

        if ( !treated)
        {
            executeinstr();
            treated = true;
            stream.str("");
            s="";
            //s.resize(28);
        }
    }

}

void OrdersRaspberry::treatSerial()
{
    int serial_count = 0;
    char serial_char;

    // Ici on traite les différents messages reçus.
    while(Serial.available()>0 && serial_count < 28)
    {
        delay(1);
        serial_char= Serial.read();
        if (serial_char =='\n')
        {
            Serial.println("Fin de ligne");
            //on ne traite que les instructions de minimum 2 char
            if (serial_count<2)
            {
                treated = true;
            }
            else
            {
                treated = false;
            }
            Serial.print("Enregistre : ");
            Serial.println(s.c_str());
            return;
        }
        s+=serial_char;
        serial_count++;
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

    int cap = 0;
    int x = 0;
    int y = 0;
    int r = 0;
    int v = 0;
    int s=0;
    bool precis;
    switch (ordre)
    {
    case 'S' :
        //Vitesse vit;
        // ordre de type Slave
        switch (ind)
        {
        case 0: //Calcul Gain
            Serial.println("Reglage Odo");
    //        slave->reglageOdo();
            break;

        case 1: //Recal
            Serial.println("Recal arriere");
      //      slave->set<Recaler>();
            break;

        case 2: //Recal
            Serial.println("Recal avant");
        //    slave->set<Recaler>(Marche::AVANT);
            break;
        case 3: //BFCap
            if(!(stream>>cap>>precis))
            {
                Serial.print ("err ");
            }
            Serial.print ("BFCap ");
            Serial.println(cap);
         //   slave->set<BFCap>(cap,precis);
            break;
        case 4: //BFAvance

            stream >> x >>v;
            Serial.println("BFAvance");
            Serial.print(" ");
            Serial.print(x);
            Serial.print(" ");
            Serial.println(v);

            //vit=determinerVitesse(v);
      //      slave->set<BFAvance>(x , vit );
            break;
        case 5: //BFDroite

            stream>>x >>y >> cap>> v>>precis;

            Serial.println("BFDroite");
            Serial.print(x);
            Serial.print(" ");
            Serial.print(y);
            Serial.print(" ");
            Serial.print(cap);
            Serial.print(" ");
            Serial.println(v);

            //vit=determinerVitesse(v);
        //    slave->set<BFDroite>(x,y,cap ,vit,precis);
            break;
        case 6: //BFCercle
            stream>> x >> y >> r >> cap>> s >> v;

            Serial.println("BFCercle");
            Serial.print(x);
            Serial.print(" ");
            Serial.print(y);
            Serial.print(" ");
            Serial.print(r);
            Serial.print(" ");
            Serial.print(cap);
            Serial.print(" ");
            Serial.print(s);
            Serial.print(" ");
            Serial.println(v);
            //vit=determinerVitesse(v);
         //   slave->set<BFCercle>(x,y,r, cap ,s,  vit);
            break;
        case 7: //Recal
            Serial.println("Arret Robot");
         //   slave->set<ARRET_ROBOT>();
            break;
        case 8: //Reprendre
            Serial.println("Reprendre");
          //  slave->set<REPRENDRE>();
            break;

        case 9:
            stream>> x >> y >> cap;
            Serial.println("setXYCap");
            Serial.print(x);
            Serial.print(" ");
            Serial.print(y);
            Serial.print(" ");
            Serial.print(cap);
            Serial.println(" ");
          //  slave->setXYCap(x, y ,cap);
            break;
        }
        return;
/*
#ifdef PRINCIPAL
    case 'B' :

        //ordre pour les MAE BRAS
      	switch (ind)
	{
	case 1: //StartGato
    }		
	return;*/
    }


}
