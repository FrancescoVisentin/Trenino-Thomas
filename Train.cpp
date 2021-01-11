#include "Train.h"

#include <cmath>
#include <iostream>

Regional::Regional(int t_number, bool ori, vector<int> a_times, vector<bool> s_type, vector<int> s_distances)
//: Train(origin, 1, arrival_times, stations_type, stations_distances)
{
    origin = ori;
    train_number = t_number;
    train_type = 1;
    delay = 0;
    prec_delay = 0;
    position = 0;
    //position spostata in start()
    station_index = 0;
    state = false;
    current_velocity = 0;
    current_rail = -1;
    max_velocity = 160;
    restarted = -1;

    arrival_times = a_times;
    stations_distances = s_distances;
    stations_type = s_type;

    if(origin == 1)
    {
        invertStations(stations_distances, stations_type);
    }

    checkTwentyKilometres();        //gestisco le stazioni a distanza minore di 20km da quelle precedenti

    vector<int> dist;
    createDistances(dist);      //creo il vettore Distances che contiene le distanze tra le varie stazioni
    distances = dist;
    
    int num_of_principal_stations = countPrincipalStations(stations_type);
    
    checkArrivalTimes();    //controllo che gli orari di arrivo della timetable abbiano senso
                            //e che siano in giusta quantità

    vector<int> s_points;
    createSignalPoints(stations_distances, s_points);        //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points = s_points;
}

HighV::HighV(int t_number, bool ori, vector<int> a_times, vector<bool> s_type, vector<int> s_distances)
//: Train(origin, 2, arrival_times, stations_type, stations_distances)
{
    origin = ori;
    train_number = t_number;
    train_type = 2;
    delay = 0;
    prec_delay = 0;
    position = 0;
    //position spostata in start()
    station_index = 0;
    state = false;
    current_velocity = 0;
    current_rail = -1;
    max_velocity = 240;
    restarted = -1;

    arrival_times = a_times;
    stations_distances = s_distances;
    stations_type = s_type;;

    if(origin == 1)
    {
        invertStations(stations_distances, stations_type);
    }

    checkTwentyKilometres();        //gestisco le stazioni a distanza minore di 20km da quelle precedenti

    vector<int> dist;
    createDistances(dist);      //creo il vettore Distances che contiene le distanze tra le varie stazioni
    distances = dist;
    
    int num_of_principal_stations = countPrincipalStations(stations_type);
    
    checkArrivalTimes();    //controllo che gli orari di arrivo della timetable abbiano senso
                            //e che siano in giusta quantità

    vector<int> s_points;
    createSignalPoints(stations_distances, s_points);        //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points = s_points;
}

HighV_s::HighV_s(int t_number, bool ori, vector<int> a_times, vector<bool> s_type, vector<int> s_distances)
//: Train(origin, 3, arrival_times, stations_type, stations_distances)
{
    origin = ori;
    train_number = t_number;
    train_type = 3;
    delay = 0;
    prec_delay = 0;
    position = 0;
    //position spostata in start()
    station_index = 0;
    state = false;
    current_velocity = 0;
    current_rail = -1;
    max_velocity = 300;
    restarted = -1;

    arrival_times = a_times;
    stations_distances = s_distances;
    stations_type = s_type;

    if(origin == 1)
    {
        invertStations(stations_distances, stations_type);
    }

    checkTwentyKilometres();        //gestisco le stazioni a distanza minore di 20km da quelle precedenti

    vector<int> dist;
    createDistances(dist);      //creo il vettore Distances che contiene le distanze tra le varie stazioni
    distances = dist;
    
    int num_of_principal_stations = countPrincipalStations(stations_type);
    
    checkArrivalTimes();    //controllo che gli orari di arrivo della timetable abbiano senso
                            //e che siano in giusta quantità

    vector<int> s_points;
    createSignalPoints(stations_distances, s_points);        //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points = s_points;
}

int Train::countPrincipalStations(vector<bool>& stations_type)
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

void Train::invertStations(vector<int>& stations_distances, vector<bool>& stations_type)
{
    vector<int> tmp;
    int last = stations_distances.back();
    for(int i=stations_distances.size()-1; i>=0; i--)
    {
        if(stations_distances.at(i) == -1)
        {
            tmp.push_back(-1);
        }
        else
        {
            tmp.push_back(last-stations_distances.at(i));
        }
    }
    stations_distances = tmp;
    
    vector<bool> tmp1;
    for(int i=stations_type.size()-1; i>=0; i--)
    {
        tmp1.push_back(stations_type.at(i));
    }
    stations_type = tmp1;
}

void Regional::checkTwentyKilometres()
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
                for(int j=i; j < arrival_times.size(); j++)
                {
                    arrival_times.at(j) = arrival_times.at(j) - 9;
                } 
            }
            i--;
        }  
    } 
}

void HighV::checkTwentyKilometres()
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
            if(stations_type.at(i) == 0 && (i-skipped_stations) < arrival_times.size())     //elimino il tempo di arrivo solo se è principale(non li tolgo anche 
                                                                                            //con le secondarie perchè non ci sono inizialemente)
            {
                arrival_times.erase(arrival_times.begin() + (i-skipped_stations));
                for(int j=i-skipped_stations; j < arrival_times.size(); j++)
                {
                    arrival_times.at(j) = arrival_times.at(j) - 10;
                }
            }
            stations_distances.erase(stations_distances.begin() + i);
            stations_type.erase(stations_type.begin() + i);
            i--;
        }
    }
}

void HighV_s::checkTwentyKilometres()
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
            if(stations_type.at(i) == 0 && (i-skipped_stations) < arrival_times.size())     //elimino il tempo di arrivo solo se è principale(non li tolgo anche 
                                                                                            //con le secondarie perchè non ci sono inizialemente)
            {
                arrival_times.erase(arrival_times.begin() + (i-skipped_stations));
                for(int j=i-skipped_stations; j < arrival_times.size(); j++)
                {
                    arrival_times.at(j) = arrival_times.at(j) - 11;
                }
            }
            stations_distances.erase(stations_distances.begin() + i);
            stations_type.erase(stations_type.begin() + i);
            i--;
        }
    }
}

void Regional::createDistances(vector<int>& distances)
{
    for(int i=0; i<stations_distances.size()-1; i++)        //aggiungo tutte le distanze tra due stazioni successive
    {
        distances.push_back(stations_distances.at(i+1)-stations_distances.at(i));
    }
}
void HighV::createDistances(vector<int>& distances)
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

void HighV_s::createDistances(vector<int>& distances)
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

void Regional::checkArrivalTimes()   //controllo che gli orari di arrivo della timetable abbiano senso  
{
    int velocity = max_velocity;
    int num_of_stations = stations_distances.size();

    int min_time = ceil(((((distances.at(0)-10)*1.)/160)*60)+8);
    //ceil(((((distances.at(i)-10)*1.)/160)*60)+13);
    
    if(arrival_times.size() == num_of_stations)
    {
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";  
        }
        for(int i=1; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/160)*60)+13))       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/160)*60)+13);
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";      
            }
        }
    }
    else if(arrival_times.size() < num_of_stations)
    {   
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;         //vale solo per l'orario della seconda stazione
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";  
        }
        for(int i=1; i<arrival_times.size()-1; i++)             //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/160)*60)+13))         
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/160)*60)+13);            //vale dalla terza stazione in poi
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";
            }
        }
        
        for(int i = arrival_times.size(); i < num_of_stations; i++)        //completa tutte gli orari delle stazioni mancanti
        {
            arrival_times.push_back(arrival_times.at(i) + ceil(((((distances.at(i-1)-10)*1.)/160)*60)+23));
            cout << "Nella timetable del treno " << train_number << " sono state aggiunti alcuni orari di arrivo\n";
        }
    }
    else
    {
        while(arrival_times.size() == num_of_stations)        //elimina tutte le stazioni in eccesso
        {
            arrival_times.pop_back();
            cout << "Nella timetable del treno " << train_number << " sono state eliminati alcuni orari di arrivo\n";
        }

        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;         //vale solo per l'orario della seconda stazione
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";
        }
        for(int i=0; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/160)*60)+13))      //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/160)*60)+13);            //vale dalla terza stazione in poi    
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";
            }
        }
    }
}

void HighV::checkArrivalTimes()   //controllo che gli orari di arrivo della timetable abbiano senso  
{
    int velocity = max_velocity;
    int num_of_stations = countPrincipalStations(stations_type);
    
    int min_time = ceil(((((distances.at(0)-10)*1.)/240)*60)+8);
    //ceil(((((distances.at(i)-10)*1.)/240)*60)+13);
    
    if(arrival_times.size() == num_of_stations)
    {
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";  
        }
        for(int i=1; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/240)*60)+13))       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/240)*60)+13);
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";      
            }
        }
    }
    else if(arrival_times.size() < num_of_stations)
    {   
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;         //vale solo per l'orario della seconda stazione
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";  
        }
        for(int i=1; i<arrival_times.size()-1; i++)             //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/240)*60)+13))         
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/240)*60)+13);            //vale dalla terza stazione in poi
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";
            }
        }
        
        for(int i = arrival_times.size(); i < num_of_stations; i++)        //completa tutte gli orari delle stazioni mancanti
        {
            arrival_times.push_back(arrival_times.at(i) + ceil(((((distances.at(i-1)-10)*1.)/240)*60)+23));
            cout << "Nella timetable del treno " << train_number << " sono state aggiunti alcuni orari di arrivo\n";
        }
    }
    else
    {
        while(arrival_times.size() == num_of_stations)        //elimina tutte le stazioni in eccesso
        {
            arrival_times.pop_back();
            cout << "Nella timetable del treno " << train_number << " sono state eliminati alcuni orari di arrivo\n";
        }

        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;         //vale solo per l'orario della seconda stazione
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";
        }
        for(int i=0; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/240)*60)+13))      //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/240)*60)+13);            //vale dalla terza stazione in poi    
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";
            }
        }
    }
}

void HighV_s::checkArrivalTimes()   //controllo che gli orari di arrivo della timetable abbiano senso  
{
    int velocity = max_velocity;
    int num_of_stations = countPrincipalStations(stations_type);
    
    int min_time = ceil(((((distances.at(0)-10)*1.)/300)*60)+8);
    //ceil(((((distances.at(i)-10)*1.)/300)*60)+13);
    
    if(arrival_times.size() == num_of_stations)
    {
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";  
        }
        for(int i=1; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/300)*60)+13))       //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/300)*60)+13);
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";      
            }
        }
    }
    else if(arrival_times.size() < num_of_stations)
    {   
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;         //vale solo per l'orario della seconda stazione
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";  
        }
        for(int i=1; i<arrival_times.size()-1; i++)             //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/300)*60)+13))         
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/300)*60)+13);            //vale dalla terza stazione in poi
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";
            }
        }
        
        for(int i = arrival_times.size(); i < num_of_stations; i++)        //completa tutte gli orari delle stazioni mancanti
        {
            arrival_times.push_back(arrival_times.at(i) + ceil(((((distances.at(i-1)-10)*1.)/300)*60)+23));
            cout << "Nella timetable del treno " << train_number << " sono state aggiunti alcuni orari di arrivo\n";
        }
    }
    else
    {
        while(arrival_times.size() == num_of_stations)        //elimina tutte le stazioni in eccesso
        {
            arrival_times.pop_back();
            cout << "Nella timetable del treno " << train_number << " sono state eliminati alcuni orari di arrivo\n";
        }

        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;         //vale solo per l'orario della seconda stazione
            cout << "Nella timetable del treno " << train_number << " è stato modificato il tempo di arrivo alla seconda stazione.\n";
        }
        for(int i=0; i<arrival_times.size()-1; i++)
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/300)*60)+13))      //se l'orario è scorretto lo imposto al minimo possibile
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/300)*60)+13);            //vale dalla terza stazione in poi    
                cout << "Nella timetable del treno " << train_number << " sono stati modificati alcuni tempi di arrivo.\n";
            }
        }
    }
}

void Train::createSignalPoints(vector<int>& stations_distances, vector<int>& signal_points)
{
    for(int i=1; i<stations_distances.size(); i++)          //aggiungo posizioni a 20 km prima di ogni stazione
    {
        signal_points.push_back(stations_distances.at(i)-20);
    }
} 