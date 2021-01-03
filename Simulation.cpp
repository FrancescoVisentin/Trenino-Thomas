#include "Simulation.h"

//costruttore
Simulation::Simulation(std::istream& timetables, std::istream& line)
{
    parse_line(line); //inizializza "Stations"
    parse_timetable(timetables); //inizializza "trains"

    for(int i = 0; i < trains.size(); i++)
        if(trains[i]->get_origin())
            backward_trains.push_back(trains[i]);
        else
            forward_trains.push_back(trains[i]);
        

}

//distruttore
Simulation::~Simulation()
{
    //elimino tutte le scansioni salvate
    for(int i = 0; i < stations.size(); i++)
        delete stations[i];

    //elimino eventuali treni rimasti in circolo. Non dovrebbe essere mai necessario.
    for(int i = 0; i < trains.size(); i++)
        delete trains[i];
}


void Simulation::simulate()
{
    if(!ready) 
    {
        std::cout<<"Impossibile effettuare simulazione.\nInvocazione di reset_simulation richiesta.\n";
        return; //Sto tentando di effettuare più simulazioni su un oggetto che non è stato "resettato".
    }

    //Ogni iterazione del ciclo rappresenta un minuto.
    //Quando "trains" è vuoto non sono più presenti treni in circolo e la simulazione termina.
    while(trains.size() > 0)
    {
        check_new_trains();
        check_trains_distance();
        check_starting_trains();
        check_incoming_trains();
        
        time++;
    }

    ready = false; //è necessario "resettare" l'oggetto.
}


void Simulation::reset_simulation(std::istream& timetables, std::istream& line)
{
    if(!ready)
    {
        for(int i = 0; i < stations.size(); i++)
            delete stations[i];

        for(int i = 0; i < trains.size(); i++)
            delete trains[i];

        //riporto l'oggetto al suo stato iniziale.
        stations.clear();
        trains.clear();
        stations_distances.clear();
        stations_type.clear();
        time = 0;
        ready = true;

        parse_line(line);
        parse_timetable(timetables);
    }
}


/*
*   Implementazione delle funzioni private usate nel codice.
*/


void Simulation::parse_line(std::istream& line)
{
    //leggo il file una riga alla volta. Se i dati sono validi creo una nuova stazione.
    while(line.peek() != EOF)
    {
        //estraggo una riga da line_description.txt
        std::string s{""};
        std::getline(line,s);

        std::string name {s};
        bool station_type {false};
        int distance {0};

        int  position = s.find_first_of("01");
        if(position != std::string::npos)
        {
            name = s.substr(0,position-1);
            station_type = stoi(s.substr(position,1));
            distance = stoi(s.substr(position+2, std::string::npos));
        }

        stations_distances.push_back(distance);
        stations_type.push_back(station_type);

        if(stations_distances.size() >= 2) //ho già almeno due stazioni create.
        {
            if(distance - stations_distances[stations_distances.size()-2] < MIN_DISTANCE) //ultime due troppo vicine.
                stations_distances.back() = INVALID_STATION;
        }

        if(distance != INVALID_STATION)
            switch (station_type)
            {
                case S_LOCALE:
                {   
                    Stazione_locale* new_station = new Stazione_locale{name, distance};
                    stations.push_back(new_station);
                    break;
                }
                case S_PRINCIPALE: 
                {   
                    Stazione_principale* new_station = new Stazione_principale{name, distance};
                    stations.push_back(new_station);
                    break;
                }
            }
    }   
}


void Simulation::parse_timetable(std::istream& timetables) 
{
    while(timetables.peek() != EOF)
    {
        //estraggo una riga da timetables.txt
        std::string s{""};
        std::getline(timetables,s);
        std::istringstream stream (s);
        
        int train_number;
        stream >> train_number;

        bool origin;
        stream >> origin;

        int train_type;
        stream >> train_type;

        std::vector<int> arrival_times;
        for(int t; stream >> t;)
            arrival_times.push_back(t);
        

        switch (train_type)
        {
            case T_REGIONALE :
            {
                Regional* new_train = new Regional{origin, train_type, arrival_times, stations_type, stations_distances};
                trains.push_back(new_train);
                break;
            }
            case T_VELOCE :
            {
                HighV* new_train = new HighV{origin, train_type, arrival_times,stations_type, stations_distances};
                trains.push_back(new_train);
                break;
            }
            case T_SUPER :
            {
                HighV_s* new_train = new HighV_s{origin, train_type, arrival_times,stations_type,stations_distances};
                trains.push_back(new_train);
                break;
            }
        }
    }
}


void Simulation::check_new_trains() //fa partire treni dalla stazione originarie
{
    std::vector<int> train_index;
    for(int i = next_train; i < trains.size(); i++)
    {
        if(trains[i]->start_time() <= time)
        {
            train_index.push_back(i);
            next_train++;
        }
    }

    //decide chi parte.
    //controlla da dove parte
    //  if(stations[?]->can_start(train[i]->orgin(), train[i]->startTime())) //quando un treno può lasciare la stazione
    //      trains[i]->start(...); //passa eventuali parametri per far capire al treno che è partito.
}

void Simulation::check_trains_distance()
{
    /*
    4 casi:
        1) treni fuori dal range 10 km
        2) treni in transito contemporaneamente
        3) treno fermo fermo
        4) treno fermo ma indeciso
    */


    for(int i = 0; i < next_forward; i++)
    {
        for(int j = i+1; j < next_forward; j++)
        {
            if(std::abs(forward_trains[i]->get_position() - forward_trains[j]->get_position()) < 10 )
            {
                if(forward_trains[i]->isNearby() || forward_trains[j]->isNearby()) //caso 2 ,3, 4
                {

                }
                else // caso 1
                {
                    forward_trains[i]->set_current_velocity(std::min(forward_trains[i]->get_current_velocity(), forward_trains[j]->get_current_velocity()));
                    forward_trains[j]->set_current_velocity(forward_trains[i]->get_current_velocity());
                }
            }
        }
    }
}
