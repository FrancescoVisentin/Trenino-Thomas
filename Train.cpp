//Christian Marchiori matr. 1218317

#include "Train.h"

Regional::Regional(int t_number, bool ori, const std::vector<int>& a_times, const std::vector<bool>& s_type, const std::vector<int>& s_distances)
{
    //inizializzazione variabili membro
    origin = ori;
    train_number = t_number;
    train_type = 1;
    position = 0;
    delay = 0;
    prec_delay = 0;
    station_index = 0;
    state = STOPPED;
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

    createDistances();              //creo il vettore Distances che contiene le distanze TRA le varie stazioni
    
    checkArrivalTimes(stations_distances.size());       //controllo che gli orari di arrivo della timetable 
                                                        //abbiano senso e che siano in giusta quantità

    std::vector<int> s_points;
    createSignalPoints(stations_distances, s_points);   //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points = s_points;
}

HighV::HighV(int t_number, bool ori, const std::vector<int>& a_times, const std::vector<bool>& s_type, const std::vector<int>& s_distances)
{
    //inizializzazione variabili membro
    origin = ori;
    train_number = t_number;
    train_type = 2;
    position = 0;
    delay = 0;
    prec_delay = 0;
    station_index = 0;
    state = STOPPED;
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

    createDistances();              //creo il vettore Distances che contiene le distanze tra le varie stazioni
    
    int num_of_principal_stations = countPrincipalStations(stations_type);
    
    checkArrivalTimes(num_of_principal_stations);       //controllo che gli orari di arrivo della timetable 
                                                        //abbiano senso e che siano in giusta quantità

    std::vector<int> s_points;
    createSignalPoints(stations_distances, s_points);   //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points = s_points;
}

HighV_s::HighV_s(int t_number, bool ori, const std::vector<int>& a_times, const std::vector<bool>& s_type, const std::vector<int>& s_distances)
{
    //inizializzazione variabili membro
    origin = ori;
    train_number = t_number;
    train_type = 3;
    position = 0;
    delay = 0;
    prec_delay = 0;
    station_index = 0;
    state = STOPPED;
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

    createDistances();              //creo il vettore distances che contiene le distanze tra le varie stazioni
                                    //utile alla funzione checkArrivalTimes

    int num_of_principal_stations = countPrincipalStations(stations_type);
    
    checkArrivalTimes(num_of_principal_stations);       //controllo che gli orari di arrivo della timetable abbiano senso
                                                        //e che siano in giusta quantità

    std::vector<int> s_points;
    createSignalPoints(stations_distances, s_points);   //creo il vettore segnalation_point che contiene i punti di segnalazione
    signal_points = s_points;
}

//funzione che conta le stazioni principali
int countPrincipalStations(const std::vector<bool>& stations_type)
{
    int num_of_principal_stations = 0;      //contatore
    for(int i=0; i<stations_type.size(); i++)
    {
        if(stations_type.at(i) == 0)
        {
            num_of_principal_stations++;
        }
    }
    return num_of_principal_stations;
}

//funzione che inverte le distanze del vettore stations_distances solamente se il treno parte dal capolinea
void invertStations(std::vector<int>& stations_distances, std::vector<bool>& stations_type)
{
    //inverto stations_distances
    std::vector<int> tmp;
    int last = stations_distances.back();       //distanza del capolinea dall'origine
    for(int i=stations_distances.size()-1; i>=0; i--)
    {
        if(stations_distances.at(i) == -1)      //se la stazione non è valida non calcolo la distanza
        {                                       //dal capolinea ma inserisco -1
            tmp.push_back(-1);
        }
        else
        {
            tmp.push_back(last-stations_distances.at(i));
        }
    }
    stations_distances = tmp;
    
    //inverto stations_type (non serve gestire quelle non valide perchè saranno gestite successivamente)
    std::vector<bool> tmp1;
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
            {                                       //la timetables potrebbe contenere meno tempi di arrivo del dovuto
                arrival_times.erase(arrival_times.begin() + i);
                //il seguente ciclo for è neccessario per aggiustare i tempi di arrivo successivi ad una stazione non valida
                //il treno, non passando per la stazione non valida, impiega meno tempo del dovuto ad arrivare alle successive
                //il tempo totale per fermarsi è 13 minuti (4+5+4), il tempo per transitare è in questo caso (10/160)*60 = 4 min
                //quindi il treno recupera 13-4 = 9 minuti 
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
    int skipped_stations {0};           //contiene le stazioni valide secondarie (saltate dai treni veloci)
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
                //il seguente ciclo for è neccessario per aggiustare i tempi di arrivo successivi ad una stazione non valida
                //Il treno, non passando per la stazione non valida, impiega meno tempo del dovuto ad arrivare alle successive
                //il tempo totale per fermarsi è 13 minuti (4+5+4), il tempo per transitare è in questo caso (10/240)*60 = 3 min
                //quindi il treno recupera 13-3 = 10 minuti 
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
                //il seguente ciclo for è neccessario per aggiustare i tempi di arrivo successivi ad una stazione non valida
                //il treno, non passando per la stazione non valida, impiega meno tempo del dovuto ad arrivare alle successive
                //il tempo totale per fermarsi è 13 minuti (4+5+4), il tempo per transitare è in questo caso (10/300)*60 = 2 min
                //quindi il treno recupera 13-2 = 11 minuti 
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

//createDistances inizializza il vettore distances (utile alla funzione checkArrivalTimes)
void Regional::createDistances()
{
    for(int i=0; i<stations_distances.size()-1; i++)        //aggiungo tutte le distanze tra due stazioni successive
    {
        distances.push_back(stations_distances.at(i+1)-stations_distances.at(i));
    }
}

void HighV::createDistances()
{    
    for(int i=0; i<stations_distances.size()-1; i++)
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

void HighV_s::createDistances()
{    
    for(int i=0; i<stations_distances.size()-1; i++)
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

//controllo che gli orari di arrivo della timetable abbiano senso e siano in giunta quantità
//in queste situazioni viene impostato un tempo minimo di percorrenza tra una stazione e l'altra
//i tempi minimi vengono calcolati in questo modo:
//- primo tratto --> 4 min per uscire dal limite della stazione + x min per arrivare al limite della seconda stazione
//                  + 4 min per arrivare al centro della stazione = x + 8 min
//- altri tratti --> 5 min di fermata + 4 min per uscire dal limite della stazione + x min per arrivare 
//                  al limite della seconda stazione + 4 min per arrivare al centro della stazione = x + 13 min
void Train::checkArrivalTimes(int num_of_stations)     
{
    int velocity = max_velocity;

    int min_time = ceil(((((distances.at(0)-10)*1.)/velocity)*60)+8);   //tempo minimo per arrivare alla seconda stazione
    
    if(arrival_times.size() == num_of_stations)     //se il numero di tempi di arrivo nella timetable è corretto
    {
        int c = 0;
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)      //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time; 
            c++;
        }
        for(int i=1; i<arrival_times.size()-1; i++)                     //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/velocity)*60)+13))
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/velocity)*60)+13);
                c++;   
            }
        }
        if(c != 0)
        {
            std::cout << "Nella timetable del treno " << train_number << " sono stati modificati "<< c <<" tempi di arrivo.\n"; 
        }
    }
    else if(arrival_times.size() < num_of_stations)         //se il numero di tempi di arrivo nella timetable è minore del dovuto
    {   
        int c = 0;
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)      //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;
            c++;  
        }
        for(int i=1; i<arrival_times.size()-1; i++)                     //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/velocity)*60)+13))         
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/velocity)*60)+13);
                c++;
            }
        }
        if(c != 0)
        {
            std::cout << "Nella timetable del treno " << train_number << " sono stati modificati "<< c <<" tempi di arrivo.\n"; 
        }
        int c2 = 0;
        for(int i = arrival_times.size(); i < num_of_stations; i++)        //completa tutte gli orari delle stazioni mancanti
        {
            arrival_times.push_back(arrival_times.at(i-1) + ceil(((((distances.at(i-1)-10)*1.)/velocity)*60)+23));
            c2++;
        }
        std::cout << "Nella timetable del treno " << train_number << " sono state aggiunti "<< c << " orari di arrivo\n";
    }
    else        //se il numero di tempi di arrivo nella timetable è maggiore del dovuto
    {
        int c2 = 0;
        while(arrival_times.size() == num_of_stations)        //elimina tutte le stazioni in eccesso
        {
            arrival_times.pop_back();
            
        }
        std::cout << "Nella timetable del treno " << train_number << " sono state eliminati "<< c2 <<" orari di arrivo\n";
        
        int c = 0;
        if((arrival_times.at(1) - arrival_times.at(0)) < min_time)       //se l'orario è scorretto lo imposto al minimo possibile
        {
            arrival_times.at(1) = arrival_times.at(0) + min_time;
            c++;
        }
        for(int i=0; i<arrival_times.size()-1; i++)                     //se l'orario è scorretto lo imposto al minimo possibile
        {
            if((arrival_times.at(i+1) - arrival_times.at(i)) < ceil(((((distances.at(i)-10)*1.)/velocity)*60)+13))      
            {
                arrival_times.at(i+1) = arrival_times.at(i) + ceil(((((distances.at(i)-10)*1.)/velocity)*60)+13);
                c++;
            }
        }
        if(c != 0)
        {
            std::cout << "Nella timetable del treno " << train_number << " sono stati modificati "<< c <<" tempi di arrivo.\n"; 
        }
    }
}

//funzione che inserisce in un vettore tutte le posizioni in cui il treno invierà un segnale alla prossima stazione
void createSignalPoints(const std::vector<int>& stations_distances, std::vector<int>& signal_points)
{
    for(int i=1; i<stations_distances.size(); i++)          //aggiungo posizioni a 20 km prima di ogni stazione
    {
        signal_points.push_back(stations_distances.at(i)-20);
    }
} 