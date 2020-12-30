#include "Train.h"


Regional::Regional(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances)
        : Train(origin, train_type, arrival_times, stations_type, stations_distances)
{}

HighV::HighV(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances)
        : Train(origin, train_type, arrival_times, stations_type, stations_distances)
{}

HighV_s::HighV_s(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances)
        : Train(origin, train_type, arrival_times, stations_type, stations_distances)
{}

Train::Train(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances)
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
    
    vector<int> distances;          //questo vettore conterrà tutte le distanze tra le varie stazioni
    for(int i=0; i<stations_distances.size()-1; i++)
    {
        distances.push_back(stations_distances.at(i+1)-stations_distances.at(i));
    }
    
    checkArrivalTimes(arrival_times, distances, velocity);      //controllo che gli orari di arrivo della timetable abbiano senso
    
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

void checkArrivalTimes(vector<int>& arrival_times, vector<int> distances, int velocity)   //controllo che gli orari di arrivo della timetable abbiano senso  
{
    for(int i=0; i<arrival_times.size()-1; i++)
    {
        if((arrival_times.at(i+1) - arrival_times.at(i)) < (distances.at(i)/velocity)*60)
        {
            arrival_times.at(i+1) = ((distances.at(i)/velocity)*60);     //se l'orario è scorretto lo imposto al minimo possibile
        }
    }
}