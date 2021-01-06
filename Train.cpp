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
        : max_velocity{whatVelocity(train_type)}
{
    origin1 = origin;
    train_type1 = train_type;
    delay = 0;
    position = -1;
    //position spostata in start()
    station_index = 0;
    state = false;
    current_velocity = 0;
    //current_station = -1;
    current_rail = -1;

    if(origin == 1)
    {
        invertStations(stations_distances, stations_type);
    }

    checkTwentyKilometres(stations_distances, stations_type, arrival_times, train_type);        //gestisco le stazioni a distanza minore di 20km da quelle precedenti
    stations_distances1 = stations_distances;
    stations_type1 = stations_type;

    vector<int> distances;
    createDistances(stations_distances, stations_type, distances, train_type);      //creo il vettore Distances che contiene le distanze tra le varie stazioni
    distances1 = distances;
    
    int num_of_principal_stations = countPrincipalStations(stations_type);
    
    checkArrivalTimes(stations_distances.size(), num_of_principal_stations, stations_type, distances, arrival_times, max_velocity, train_type);     //controllo che gli orari di arrivo della timetable abbiano senso
                                                                                                                                                //e che siano in giusta quantità
    arrival_times1 = arrival_times;

    vector<int> signal_points;
    createSignalPoints(stations_distances, signal_points);        //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points1 = signal_points;
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

int countPrincipalStations(vector<bool>& stations_type)
{
    int num_of_principal_stations = 0;
    for(int i=0; i<stations_type.size(); i++)
    {
        if(stations_type.at(i) == 0)
        {
            num_of_principal_stations++;
        }
    }
    return num_of_principal_stations;
}

void invertStations(vector<int>& stations_distances, vector<bool>& stations_type)
{
    vector<int> tmp;
    int last = stations_distances.back();
    for(int i=stations_distances.size()-1; i>=0; i--)
    {
        tmp.push_back(last-stations_distances.at(i));
    }
    stations_distances = tmp;
    
    vector<bool> tmp1;
    for(int i=stations_type.size()-1; i>=0; i--)
    {
        tmp1.push_back(stations_type.at(i));
    }
    stations_type = tmp1;
}

void checkTwentyKilometres(vector<int>& stations_distances, vector<bool>& stations_type, vector<int>& arrival_times, int train_type)
{    
    if(train_type == 1)
    {
        for(int i=0; i<stations_distances.size(); i++)
        {
            if(stations_distances.at(i) == -1)          //nelle stazioni non valide elimino i corrispettivi: distanze, tipi di stazione, tempi di arrivo
            {
                stations_distances.erase(stations_distances.begin() + i);
                stations_type.erase(stations_type.begin() + i);

                if(i < arrival_times.size())            //elimino il tempo di arrivo solo se è presente inizialmente
                {
                    arrival_times.erase(arrival_times.begin() + i);
                }
            }  
        }
    }
    else
    {
        int skipped_stations {0};           //contiene le stazioni valide secondarie(saltate dai treni veloci)
        for(int i=0; i< stations_distances.size(); i++)
        {
            if(stations_type.at(i) == 1 && stations_distances.at(i) != -1)
            {
                skipped_stations++;
            }
            if(stations_distances.at(i) == -1)      //nelle stazioni non valide elimino i corrispettivi: distanze, tipi di stazione, tempi di arrivo
            {
                stations_distances.erase(stations_distances.begin() + i);
                stations_type.erase(stations_type.begin() + i);
                
                if(stations_type.at(i) == 0 && (i-skipped_stations) < arrival_times.size())     //elimino il tempo di arrivo solo se è principale(non li tolgo anche 
                                                                                                //con le secondarie perchè non ci sono inizialemente)
                {
                    arrival_times.erase(arrival_times.begin() + (i-skipped_stations));
                }
            }
        }
    }
}

void createDistances(vector<int>& stations_distances, vector<bool>& stations_type, vector<int>& distances, int train_type)
{
    if(train_type == 1)
    {
        for(int i=0; i<stations_distances.size()-1; i++)        //aggiungo tutte le distanze tra due stazioni successive
        {
            distances.push_back(stations_distances.at(i+1)-stations_distances.at(i));
        }
    }
    else
    {
        for(int i=0; i<stations_distances.size()-1; i++)        //aggiungo tutte le distanze tra due stazioni successive
        {
            if(stations_type.at(i) == 0)        //aggiungo tutte le distanze tra due stazioni principali successive
            {
                int offset = 1;
                while(((i+offset) < stations_type.size()) && (stations_type.at(i+offset) != 0))
                {
                    offset++;
                }
                distances.push_back(stations_distances.at(i+offset)-stations_distances.at(i));
            }
        }
    }
}

void checkArrivalTimes(int num_of_all_stations, int num_of_principal_stations, vector<bool>& stations_type, vector<int>& distances, vector<int>& arrival_times, int velocity, int train_type)   //controllo che gli orari di arrivo della timetable abbiano senso  
{
    int num_of_stations;
    if(train_type == 1)     //imposto il numero di stazioni utili in base al treno
    {
        num_of_stations = num_of_all_stations;
    }
    else
    {
        num_of_stations = num_of_principal_stations;
    }
    
    if(arrival_times.size() == num_of_stations)
    {
        if((arrival_times.at(1) - arrival_times.at(0)) < (((distances.at(0)-10)/velocity)*60)+8)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = ((((distances.at(0)-10)/velocity)*60)+8);     
        }
        for(int i=1; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < (((distances.at(i)-10)/velocity)*60)+13)       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = (((distances.at(i)-10)/velocity)*60)+13;     
            }
        }
    }
    else if(arrival_times.size() < num_of_stations)
    {   
        if((arrival_times.at(1) - arrival_times.at(0)) < (((distances.at(0)-10)/velocity)*60)+8)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = ((((distances.at(0)-10)/velocity)*60)+8);         //vale solo per l'orario della seconda stazione
        }
        for(int i=1; i<arrival_times.size()-1; i++)             //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < (((distances.at(i)-10)/velocity)*60)+13)         
            {
                arrival_times.at(i+1) = (((distances.at(i)-10)/velocity)*60)+13;            //vale dalla terza stazione in poi
            }
        }
        
        for(int i = arrival_times.size(); i < num_of_stations; i++)        //completa tutte gli orari delle stazioni mancanti
        {
            arrival_times.push_back((((distances.at(i-1)-10)/velocity)*60)+23);
        }
    }
    else
    {
        while(arrival_times.size() == num_of_stations)        //elimina tutte le stazioni in eccesso
        {
            arrival_times.pop_back();
        }

        if((arrival_times.at(1) - arrival_times.at(0)) < (((distances.at(0)-10)/velocity)*60)+8)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = ((((distances.at(0)-10)/velocity)*60)+8);         //vale solo per l'orario della seconda stazione
        }
        for(int i=0; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < (((distances.at(i)-10)/velocity)*60)+13)       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = (((distances.at(i)-10)/velocity)*60)+13;            //vale dalla terza stazione in poi    
            }
        }
    }
}

void createSegnalationPoints(vector<int>& stations_distances, vector<int>& signal_points)
{
    for(int i=1; i<stations_distances.size(); i++)          //aggiungo posizioni a 20 km prima di ogni stazione
    {
        signal_points.push_back(stations_distances.at(i)-20);
    }
} 