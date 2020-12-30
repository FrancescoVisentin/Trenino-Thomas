#include "Simulation.h"

//costruttore
Simulation::Simulation(std::istream& timetables, std::istream& line)
{
    parse_line(line);
    parse_timetable(timetables); //inizializza "trains"
}


void Simulation::simulate()
{
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
}


void Simulation::parse_line(std::istream& line)
{
    while(line.peek() != EOF)
    {
        //estraggo una riga da line_description.txt
        std::string s{""};
        std::getline(line,s);
        std::istringstream stream (s);

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

        switch (station_type)
        {
            case 0:
            {       
                Stazione_locale* new_station = new Stazione_locale{name, distance, station_type};
                stations.push_back(new_station);
                break;
            }
            case 1: 
            {   
                Stazione_principale* new_station = new Stazione_principale{name, distance, station_type};
                stations.push_back(new_station);
                break;
            }
        }

        stations_distances.push_back(distance);
        stations_type.push_back(station_type);
        
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
            case 1 :
            {
                Regional* new_train = new Regional{origin, train_type, arrival_times,stations_type,stations_distances};
                trains.push_back(new_train);
                break;
            }
            case 2 :
            {
                HighV* new_train = new HighV{origin, train_type, arrival_times,stations_type,stations_distances};
                trains.push_back(new_train);
                break;
            }
            case 3 :
            {
                HighV_s* new_train = new HighV_s{origin, train_type, arrival_times,stations_type,stations_distances};
                trains.push_back(new_train);
                break;
            }
        }
    }
}


