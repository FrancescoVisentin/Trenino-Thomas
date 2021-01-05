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
Simulation::~Simulation() //(MODIFICA)
{
    //elimino tutte le stazioni salvate
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
        std::cout<<"Impossibile effettuare simulazione\nInvocazione di reset_simulation richiesta\n";
        return; //Sto tentando di effettuare più simulazioni su un oggetto che non è stato "resettato".
    }

    //Ogni iterazione del ciclo rappresenta un minuto.
    //Quando "trains" è vuoto non sono più presenti treni in circolo e la simulazione termina.
    while(trains.size() > 0)
    {
        check_box(); //prima libero eventuali treni intrappolati.
        check_position(); //tutti i controlli sulla posizione: se arrivato , se a 20 km, se a 5km, se a 0km dalla stazione.
        check_new_trains();//controlla nuove partenza.
        check_trains_distance();//controlla distanza tra treni
        update_position();

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
        forward_trains.clear();
        backward_trains.clear();
        stations_distances.clear();
        stations_type.clear();
        time = 0;
        ready = true;

        parse_line(line);
        parse_timetable(timetables);

        for(int i = 0; i < trains.size(); i++)
        if(trains[i]->get_origin())
            backward_trains.push_back(trains[i]);
        else
            forward_trains.push_back(trains[i]);
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


void Simulation::check_box()
{
    for(int i = 0; i < boxed_trains.size(); i++)
    {
        int station_index = boxed_trains[i]->get_station_index();
        bool origin = boxed_trains[i]->get_origin();
        
        //in base al verso del treno devo adattare l'indice restituito
        if(origin)
        {
            station_index = stations.size() - station_index -1; //conto dalla fine
        }
        else
        {
            station_index = station_index + 1; //conto dall'inizio
        }

        if(stations[station_index]->can_leave(origin,time)) //true se i binari non sono già prenotati
        {
            boxed_trains[i]->set_state(-1); //un treno che era ai box sicuramente VOLEVA fermarsi ma ha trovato tutto pieno
                                            //allora il suo stato, qunado riparte, sicuramente sarà messo a -1. 
            boxed_trains[i]->set_current_velocity(80);
        }
        
    }
}


void Simulation::update_position()
{
    for(int i = 0; i < next_forward; i++)
        forward_trains[i]->update(); //dovrebbe bastar: position = position + cur_vel/60

    for(int i = 0; i < next_backward; i++)
        backward_trains[i]->update(); //dovrebbe bastar: position = position + cur_vel/60
}


void Simulation::check_position()
{
    for(int i = 0; i < next_forward; i++)
    {
        int train_state = forward_trains[i]->get_state();

        //le seguenti funzioni si basano sulla posizione del treno all'interno della linea.
        //Se una funzione si attiva allora le altre sicuramente non lo faranno quindi, qualora
        //una si attivasse passo direttamente alla prossima iterazione del ciclo.
        if(at_destination(forward_trains, i, next_forward)) continue;
        if(at_station(forward_trains[i], train_state)) continue;
        if(at_five(forward_trains[i],train_state)) continue;
        if(at_twenty(forward_trains[i], false)) continue; //(CONTROLLA HANDLE_NEW_TRAIN E STESSO TRENO RIPETUTO)
    }

    for(int i = 0; i < next_backward; i++)
    {
        int train_state = backward_trains[i]->get_state();

        if(at_destination(backward_trains, i, next_backward)) continue;
        if(at_station(backward_trains[i], train_state)) continue;
        if(at_five(backward_trains[i],train_state)) continue;
        if(at_twenty(backward_trains[i], true)) continue; //sono già sui backward.   
    }

}


bool Simulation::at_station(Train* train, int train_state)
{
    bool origin = train->get_origin();
    
    if(origin) //treni dal capolinea
    {
        if(train->past_station()) // true se la posizione del treno è tra: [pos_stazione, pos_stazine +5)
        {
            int station_index = stations.size() - train->get_station_index(); // sta già puntando alla stazione successiva

            if(train_state = -1)// il treno deve fermarsi / è già fermo
            {
                if(train->get_current_velocity() != 0)//non è già stato fermato
                {
                    train->set_current_velocity(0); //lo fermo
                    stations[station_index]->new_stopped_train(train, time);//salva orario d'arrivo per quel treno
                }
                else if(stations[station_index]->can_restart(train, time))//controlla che siano passati minimo 5 minuti e che il treno possa ripartire (algoritmo vecchio?)
                {
                    train->set_current_velocity(80);
                    train->set_state(1);
                }
            }
            return true;
        }
    }
    else //treni dall'origine
    {
        if(train->past_station()) // true se la posizione del treno è tra: [pos_stazione, pos_stazine +5)
        {
            int station_index = train->get_station_index(); // sta già puntando alla stazione successiva

            if(train_state = -1)// il treno deve fermarsi / è già fermo
            {
                if(train->get_current_velocity() != 0)//non è già stato fermato
                {
                    train->set_current_velocity(0); //lo fermo
                    stations[station_index]->new_stopped_train(train, time);//salva orario d'arrivo per quel treno
                }
                else if(stations[station_index]->can_restart(train, time))//controlla che siano passati minimo 5 minuti e che il treno possa ripartire (algoritmo vecchio?)
                {
                    train->set_current_velocity(80);
                    train->set_state(1);
                }
            }
            return true;
        }
    }

    return false;
}


bool Simulation::at_five(Train* train, int train_state)
{
    if(train->past_five())
    {
        switch (train_state)
        {
            case -1:
            {
                if(train->get_current_velocity() != 80)//il treno non è proprio a 5km ma, ad esempio, a 3.5km, 2km...
                    train->set_current_velocity(80); //velocità massima nei pressi della stazione
                break;
            }
            case 0:
            {   
                /*NOTA:
                un treno ai box:
                    -ha velocità = 0 -> update_positions non crea problemi;
                    -è dentro i 5 km -> check_trains_distance lo ignora.
                    -non è al copolinea -> check_arrivals non crea problemi.
                    -check_incoming_trains  e funzioni successive non si attivano per "continue"
                    -non viene mai salvato due volte su boxed_trains
                    -NON è quindi necessario rimuovere il suo puntatore dal vettore in cui è salvato. (forward_trains o backward_trains)
                */
                if(train->get_current_velocity() != 0) // il treno non è già  stato messo ai box.
                {
                    
                    boxed_trains.push_back(train);
                    train->set_current_velocity(0);//treno è fermo. rende
                }
                break;
            }
        }

        return true;
    }

    return false;
}


bool Simulation::at_twenty(Train* train , bool type) //funziona a 20 km
{
    if(type) //treni partiti dal capolinea
    {
        if(train->past_twenty())
        {
            int station_index = stations.size() - train->get_station_index() - 1; //devo contare alla rovescia
            int train_state = stations[station_index]->handle_new_train(true, train); //DEVE FUNZIONARE BENE PASSANDO DUE VOLTE LO STESSO TRENO
            train->set_state(train_state);

            return true;
        }

        return false;
    }
    else // treni partiti dall'origine
    {
        if(train->past_twenty())
        {
            int station_index = train->get_station_index() + 1; //devo contare alla rovescia
            int train_state = stations[station_index]->handle_new_train(false, train);
            train->set_state(train_state); //-1 se dovrà fermarsi, 0 se andrà nei box, 1 se transiterà

            return true;
        }

        return false;
    }
}


bool Simulation::at_destination(std::vector<Train*>& vec, int index, int& counter)
{
    if(vec[index]->has_arrived())
    {
        delete vec[index];
        vec.erase(vec.begin() + index);
        counter--;

        return true;
    }

    return false;
}


void Simulation::check_new_trains() //fa partire treni dalla stazione originarie
{
    //Verifica eventuali treni in partenza dall'origine e gestisce priorità nella partenza dei treni
    int index {next_forward};
    if(stations[0]->can_leave(false, time))
    {
        for(int i = next_forward; i <= forward_trains.size(); i++)
            if(forward_trains[i]->start_time() <= time)
                index++;

        if(index != next_forward) //ci sono treni che devono partire
        {
            check_priority(next_forward, index, forward_trains);
            forward_trains[next_forward]->start();
            stations[0]->new_departure(false, time);
            next_forward++;
        }
    }

    //Verifica eventuali treni in partenza dal capolinea e gestisce priorità nella partenza dei treni
    index = next_backward;
    if(stations.back()->can_leave(true, time))
    {
        for(int i = next_backward; i <= backward_trains.size(); i++)
            if(backward_trains[i]->start_time() <= time)
                index++;

        if(index != next_backward) //ci sono treni che devono partire
        {
            check_priority(next_backward, index, backward_trains);
            backward_trains[next_backward]->start();
            stations.back()->new_departure(true, time);
            next_backward++;
        }    
    }
}


//ricerca il treno avente priorità massima tra gli indici [a, b) del vettore
//questo treno viene posizionato in cima alla lista dei treni in partenza.
void Simulation::check_priority(int a, int b, std::vector<Train*>& v)
{
    int max_index {a};
    for(int i = a+1; i < b; i++)
        if(v[max_index]->get_train_type() < v[i]->get_train_type())
            max_index = i;

    if(a != max_index) //c'è un massimo diverso
    {
        Train* temp = v[a];
        v[a] = v[max_index];
        v[max_index] = temp;
    }
}


void Simulation::check_trains_distance()
{
    std::vector<Train*> moving_trains;
    for(int i = 0; i < next_forward; i++)
    {
        if(!forward_trains[i]->isNearby())
            moving_trains.push_back(forward_trains[i]);
        else
        {
            if(forward_trains[i]->isRunning()) //true se è in transito/ripartito, false se si fermerà
                moving_trains.push_back(forward_trains[i]);
        }
    }

    for(int i = 0; i < moving_trains.size()-1; i++)
    {
        for(int j = i+1; j < moving_trains.size(); j++)
        {
            int position1 = moving_trains[i]->get_position();
            int position2 = moving_trains[j]->get_position();
            
            if(std::abs(position1-position2) < 10) //da verificare il 10
            {
                int min_velocity = std::min(moving_trains[i]->get_current_velocity(), moving_trains[j]->get_current_velocity());
                moving_trains[i]->set_current_velocity(min_velocity);
                moving_trains[j]->set_current_velocity(min_velocity);
            }
        }
    }

    moving_trains.clear();
    for(int i = 0; i < next_backward; i++)
    {
        if(!backward_trains[i]->isNearby())
            moving_trains.push_back(backward_trains[i]);
        else
        {
            if(backward_trains[i]->isRunning()) //true se è in transito/ripartito, false se si fermerà
                moving_trains.push_back(backward_trains[i]);
        }
    }

    for(int i = 0; i < moving_trains.size()-1; i++)
    {
        for(int j = i+1; j < moving_trains.size(); j++)
        {
            int position1 = moving_trains[i]->get_position();
            int position2 = moving_trains[j]->get_position();
            
            if(std::abs(position1-position2) < 10) //da verificare il 10
            {
                int min_velocity = std::min(moving_trains[i]->get_current_velocity(), moving_trains[j]->get_current_velocity());
                moving_trains[i]->set_current_velocity(min_velocity);
                moving_trains[j]->set_current_velocity(min_velocity);
            }
        }
    }
}
