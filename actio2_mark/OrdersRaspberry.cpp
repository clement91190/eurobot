#include "OrdersRaspberry.h"

//Fichier de définition des fonction pour les ordre reçu depuis la Gumstix
//Order for the big robot


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
    switch (ordre)
    {
    case 'T' :
        // ordre de type Tacle Laterale
        switch (ind)
        {
			
			
        case 0: 
			Serial.println("fermeture tacle droite");
            io->tacle_droite_fermeture();
            // tacle droite fermee
            break;
            
        case 1: 
			Serial.println("mi ouveture tacle droite");
            io->tacle_droite_miOuverture();
            // tacle droite mi ouverte pour rush de depart
            break;
            
        case 2: 
			Serial.println("grande ouverture tacle droite");
            io->tacle_droite_ouverture();
            // tacle droite grande ouverte pour racler les feux en hauteur
            break;
            
        case 3: 
			Serial.println("fermeture tacle gauche");
            io->tacle_gauche_fermeture();
            // tacle gauche fermee
            break;
            
        case 4: 
			Serial.println("mi ouveture tacle gauche");
            io->tacle_gauche_miOuverture();
            // tacle gauche mi ouverte pour rush de depart
            break;
            
        case 5: 
			Serial.println("grande ouverture tacle gauche");
            io->tacle_gauche_ouverture();
            // tacle gauche grande ouverte pour racler les feux en hauteur
            break;
       }
       return;
       
       
       
       case 'C' :
        // ordre de type Canon a Mammouth
        switch (ind)
        {
        case 0: 
			Serial.println("chargement du canon droit");
            io->canon_droite_enclenche();
            // armement du canon droit
            break;
            
        case 1: 
			Serial.println("mise à feux du canon droit, extermination de mamouths!!!");
            io->canon_droite_tir();
            // tir de la rampe droite
            break;
            
        case 2: 
			Serial.println("repos du canon droit, pour refroidir un peu...");
            io->canon_droite_repos();
            // repos de la rampe droite
            break;
            
        case 3: 
			Serial.println("chargement du canon gauche");
            io->canon_gauche_enclenche();
            // armement du canon gauche
            break;
            
        case 4: 
			Serial.println("mise à feux du canon gauche, extermination de mamouths!!!");
            io->canon_gauche_tir();
            // tir de la rampe gauche
            break;
            
        case 5: 
			Serial.println("repos du canon gauche, pour refroidir un peu...");
            io->canon_gauche_repos();
            // repos de la rampe gauche
            break;
       }
       return;
       
       case 'R' :
        // ordre de type Rateau a fruit moisis
        switch (ind)
        {
        case 0: 
			Serial.println("rateau bas, pas chapeau bas monsieur");
            io->benne_rateau_bas();
            // rateau en position de rangement initial
            break;
            
        case 1: 
			Serial.println("rateau mi haut");
            io->benne_rateau_mileu();	
            // rateau en position a mi hauteur pour passer au dessus des rembardes
            break;
            
        case 2: 
			Serial.println("rateau haut");
            io->benne_rateau_haut();
            // rateau en position haute pour la cueillette de fruitmouth
            break;
            
        case 3: 
			Serial.println("benne fermée");
            io->benne_centrale_close();
            // benne fermée pour recevoir les fruits
            break;
            
        case 4: 
			Serial.println("benne qui gerbe, :p");
            io->benne_centrale_gerbe();
            // benne ouverte pour deverser les fruits dans les bacs
            break;
       }
       return;
       
       case 'P' :
        // ordre de type Pile de stockage laterale
        switch (ind)
        {
        case 0: 
			Serial.println("Ejection d'un feux sur la pile droite");
            io->pile_droite_trigger(EJECTION);
            // ejection d'un feux de la pile droite, en mode aveugle
            break;
            
        case 1: 
			Serial.println("Vidange automatique de la pile droite");
            io->pile_droite_trigger(VIDANGE);
            // ejection automatique de la pile droite complete
            break;
            
        case 2: 
			Serial.println("consultation de la pile droite");
            io->pile_droite_etatMaster();
            // renvoi "* PILED 0", ou "* PILED 1" si un feux est dispo
            break;
            
        case 3: 
			Serial.println("Ejection d'un feux sur la pile gauche");
            io->pile_gauche_trigger(EJECTION);
            // ejection d'un feux de la pile droite, en mode aveugle
            break;
            
        case 4: 
			Serial.println("Vidange automatique de la pile gauche");
            io->pile_gauche_trigger(VIDANGE);
            // ejection automatique de la pile droite complete
            break;
            
        case 5: 
			Serial.println("consultation de la pile gauche");
            io->pile_gauche_etatMaster();
            // renvoi "* PILEG 0", ou "* PILEG 1" si un feux est dispo
            break;
            
    case 8:
            Serial.println("DEBUG des IRs");
            io->pile_debugIr();
            //affiche les valeurs des IR pour Debugage des seuils
            break;
	case 9: 
		Serial.println("STOP les piles");
		io->pile_gauche_trigger(STOP);
		io->pile_droite_trigger(STOP);
		//arrete le processus de vidange pour  la fin de match
		break;
       }
        return;
    }


}

