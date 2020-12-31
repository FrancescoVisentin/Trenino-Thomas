#include "Train.h"


Regional::Regional(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances)
        : Train(origin, train_type, arrival_times, stations_type, stations_distances)
{}

HighV::HighV(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances)
        : Train(origin, train_type, arrival_times, stations_type, stations_distances)
{}

HighV_s::HighV_s(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances)
        : Train(origin, train_type, arrival_times, stations_type, stations_distances)
{}

Train::Train(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances)
        : velocity{whatVelocity(train_type)}
{
    if(origin == 0)         //se l'origine è 0 la posizione è al km 0
    {
        position = 0;
    }
    else                    //se l'origine è 1 la posizione è al km massimo dalla stazione di origine
    {
        position = stations_distances.back();
    }
    delay = 0;              //ritardo iniziale nullo
    

    /*
    l'idea è la seguente:
    voglio che il vettore distances contenga le distanze tra stazioni che sono effettivamente corrette.
    Se una stazione è sbagliata la rimuovo quindi la rimuovo da tutti e tre i vettori. VIENE QUINDI GIA' RIMOSSO ANCHE 
    IL CORRISPETTIVO TEMPO DI ARRIVO DA ARRIVAL_TIMES.

    infatti:
    -nei treni regionali che fermano in tutte le stazioni gli indici dei corrispettivi dati nei tre vettori concidono ed,
     al massimo, se arrival_times è più corto degli altri, non serve nemmeno rimuovere la stazione rotta da li.

     -nei treni veloci può essere presente un offset. questo offeset è dato dal numero di stazioni locali che si trovano all'interno
      (rispettivamente) di stations_distances ed stations_type. io conto quindi quante stazioni secondarie (non nulle) trovo e quello sarà
      l'offset che mi permette di lavorare su arrival_times correttamente

    Ora dovresti avere:
     -stations_distances ed stations_type corretti e tutti con stazioni validi.
     -arrival_times con tempi relativi a stazioni valide MA NON CORRETTI.

    Dovrebbe bastar aggiustare check_arrival_times compito che le affido volentieri.
    BUON ANNO, spero che il tutto abbia senso, dimmi se ho scritto stronzate <3

    */
    vector<int> distances;     //SPOSTARE MEMBRO     //questo vettore conterrà tutte le distanze tra le varie stazioni
    if(train_type == 1)
    {
        //rimuovo eventuali stazioni smarze.
        for(int i = 0; i < stations_distances.size(); i++) // lo passo tutto perchè c++ non implementa nemmeno una cazzo di
        {                                                  // ricerca binaria che restetuisce l'indirizzo e non ho palle di scriverla io
            if(stations_distances[i] == -1)
            {
                stations_distances.erase(stations_distances.begin() + i);
                stations_type.erase(stations_type.begin() + i);

                if(i < arrival_times.size())
                    arrival_times.erase(arrival_times.begin() + i);
            }
        }
    
        //distances tra tutte le stazioni.
        for(int i=0; i<stations_distances.size()-1; i++)
            distances.push_back(stations_distances.at(i+1)-stations_distances.at(i));
    }
    else
    {
        int skipped_stations {0}; //è il numero di stazioni secondarie che rappresenta l'offset tra il vettore arrival_times ed i vettori stations_distances/times
        for(int i = 0; i < stations_distances.size(); i++)
        {
            if(stations_type[i] == 1 && stations_distances[i] != -1) skipped_stations++; //rappresenta l'offset tra il vet

            if(stations_distances[i] == -1)
            {
                if(stations_type[i] == 0 && (i-skipped_stations) < arrival_times.size())
                    arrival_times.erase(arrival_times.begin()+ (i-skipped_stations));
                
                
                stations_distances.erase(stations_distances.begin() + i);
                stations_type.erase(stations_type.begin() + i);
            }
        }

        //QUI NON HO PRATICAMENTE TOCCATO NIENTE, ho solo rimosso condizione ridondante.
        for(int i=0; i<stations_distances.size()-1; i++)
        {
            if(stations_type.at(i) == 0)
            {
                int offset2 = 1;
                while(((i+offset2) < stations_type.size()) && (stations_type.at(i+offset2) != 0))
                {
                    offset2++;
                }
                distances.push_back(stations_distances.at(i+offset2)-stations_distances.at(i));
            }
        }
    }
    
    checkArrivalTimes(arrival_times, distances, velocity, stations_distances.size());      //controllo che gli orari di arrivo della timetable abbiano senso
    
    vector<int> segnalation_points;     //questo vettore conterrà tutti i punti in cui il treno dovrà inviare un segnale alla prossima stazione
    for(int i=0; i<stations_distances.size(); i++)
    {
        segnalation_points.push_back(stations_distances.at(i)-20);
        if(segnalation_points.back()<0)     //se ci sono stazioni a meno di 20km dall'origine allora inivieranno il segnale al km 0
        {
            segnalation_points.at(i) = 0; 
        }
    }
}

int whatVelocity(int train_type)        //fornisce la velocità massima in base al tipo di treno
{
    if(train_type == 1)
        {return 160;}
    else if(train_type == 2)
        {return 240;}
    else
        {return 300;}
}

void checkArrivalTimes(vector<int>& arrival_times, vector<int> distances, int velocity, int num_of_stations, vector<bool> stations_type)   //controllo che gli orari di arrivo della timetable abbiano senso  
{
    if(arrival_times.size() == num_of_stations)
    {
        if((arrival_times.at(1) - arrival_times.at(0)) < (((distances.at(0)-10)/velocity)*60)+8)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = ((((distances.at(0)-10)/velocity)*60)+8);     
        }
        for(int i=1; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < (distances.at(i)/velocity)*60)       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = ((distances.at(i)/velocity)*60);     
            }
        }
    }
    else if(arrival_times.size() < num_of_stations)
    {   
        for(int i=0; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < (((distances.at(i)-10)/velocity)*60)+8)       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = ((distances.at(i)/velocity)*60);
            }
        }
        
        int first_exces_station = arrival_times.size();       //memoriazzia la posizione nel vector della prima stazione mancante (SISTEMARE CONTATORE i)
        
        for(int i = first_exces_station; i < num_of_stations; i++)        //completa tutte gli orari delle stazioni mancanti
        {
            arrival_times.push_back(((distances.at(i-1)/velocity)*60)+10);
        }
    }
    else
    {
        int last_exces_station = arrival_times.size()-1;        //memorizza la posizione nel vector dell'ultima stazione in eccesso

        while(last_exces_station == num_of_stations)        //elimina tutte le stazioni in eccesso
        {
            arrival_times.pop_back();
            last_exces_station --;
        }

        for(int i=0; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < (distances.at(i)/velocity)*60)       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = ((distances.at(i)/velocity)*60);     
            }
        }
    }
}