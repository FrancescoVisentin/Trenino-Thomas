//Visentin Francesco matricola 1216419

#include "Simulation.h"

//costruttore
Simulation::Simulation(std::istream& timetables, std::istream& line)
{
    std::vector<Train*> tmp;

    parse_line(line); //inizializza "Stations"
    parse_timetable(timetables, tmp); //salva provvisoriamente tutti i treni in "tmp"

    //memorizzo i treni nei relativi contenitori
    for(int i = 0; i < tmp.size(); i++)
        if(tmp[i]->get_origin())
            backward_trains.push_back(tmp[i]);
        else
            forward_trains.push_back(tmp[i]);

    //elimino eventuali distanze relative a stazioni invalide
    for(int i = 0; i < stations_distances.size(); i++)
        if(stations_distances[i] == -1)
        {
            stations_distances.erase(stations_distances.begin() + i);
            stations_type.erase(stations_type.begin() + i);
            i--;
        }
}


//distruttore
Simulation::~Simulation()
{
    //elimino tutte le stazioni salvate
    for(int i = 0; i < stations.size(); i++)
        delete stations[i];

    //elimino eventuali treni rimasti in circolo. Non dovrebbe essere mai necessario.
    for(int i = 0; i < forward_trains.size(); i++)
        delete forward_trains[i];

    for(int i = 0; i < backward_trains.size(); i++)
        delete backward_trains[i];
}


////Effettua l'intera simulazione simulando lo scorrere del tempo. Stampa a video le varie segnalazioni richieste.
void Simulation::simulate()
{
    if(!ready) 
    {
        std::cout<<"Impossibile effettuare simulazione\nInvocazione di reset_simulation richiesta\n";
        return; //Sto tentando di effettuare più simulazioni su un oggetto che non è stato "resettato".
    }

    //Ogni iterazione del ciclo rappresenta un minuto.
    //Quando "trains" è vuoto non sono più presenti treni in circolo e la simulazione termina.
    while(forward_trains.size() > 0 || backward_trains.size() > 0)
    {
        check_box(); //prima libero eventuali treni intrappolati nel posteggio.
        check_position(); //tutti i controlli sulla posizione: se arrivato , se a 20 km, se a 5km, se a 0km dalla stazione.
        check_new_trains();//controlla nuove partenza dall'origine/capolinea.
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

        for(int i = 0; i < forward_trains.size(); i++)
            delete forward_trains[i];

        for(int i = 0; i < backward_trains.size(); i++)
            delete backward_trains[i];

        //riporto l'oggetto al suo stato iniziale.
        stations.clear();
        forward_trains.clear();
        backward_trains.clear();
        stations_distances.clear();
        stations_type.clear();
        time = 0;
        ready = true;

        std::vector<Train*> tmp;
        parse_line(line);
        parse_timetable(timetables, tmp);

        for(int i = 0; i < tmp.size(); i++)
        if(tmp[i]->get_origin())
            backward_trains.push_back(tmp[i]);
        else
            forward_trains.push_back(tmp[i]);

        for(int i = 0; i < stations_distances.size(); i++)
        if(stations_distances[i] == -1)
        {
            stations_distances.erase(stations_distances.begin() + 1);
            stations_type.erase(stations_type.begin() + i);
            i--;
        }
    }
}


/*
*   Implementazione delle funzioni private usate nel codice.
*/


//Crea le stazioni verificando che queste non siano troppo vicine tra loro
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

        if(stations_distances.back() != INVALID_STATION)
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


//Crea tutti i treni. La verifica sulla effettiva correttezza della timetable viene svolta dai costruttori dei treni.
void Simulation::parse_timetable(std::istream& timetables, std::vector<Train*>& vec) 
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
                Regional* new_train = new Regional{train_number, origin, arrival_times, stations_type, stations_distances};
                vec.push_back(new_train);
                break;
            }
            case T_VELOCE :
            {
                HighV* new_train = new HighV{train_number, origin, arrival_times,stations_type, stations_distances};
                vec.push_back(new_train);
                break;
            }
            case T_SUPER :
            {
                HighV_s* new_train = new HighV_s{train_number, origin, arrival_times,stations_type,stations_distances};
                vec.push_back(new_train);
                break;
            }
        }
    }
}


//Controlla se i treni che sono stati inviati ai box possono uscire e raggiungere la stazione
void Simulation::check_box()
{
    for(int i = 0; i < boxed_trains.size(); i++)
    {
        int station_index = get_index(boxed_trains[i]);
        bool origin = boxed_trains[i]->get_origin();

        if(stations[station_index]->can_arrive_from_box(origin, boxed_trains[i]))//true se i binari non sono già prenotati
        {
            boxed_trains[i]->set_state(Train::STOPPED); //un treno che era ai box sicuramente VOLEVA fermarsi ma ha trovato tutto pieno
                                            //allora il suo stato, qunado riparte, sicuramente sarà messo a -1.
            boxed_trains[i]->set_current_velocity(Stazione::T_VELOCITY);
            boxed_trains.erase(boxed_trains.begin() + i);
            i--;
        }
        
    }
}


//svolge tutti i controlli relativi alla posizione di un treno sulla tratta.
void Simulation::check_position()
{
    for(int i = 0; i < next_forward; i++)
    {
        int train_state = forward_trains[i]->get_state();

        //le seguenti funzioni si basano sulla posizione del treno all'interno della linea.
        //Se una funzione si attiva allora le altre sicuramente non lo faranno quindi, qualora
        //una si attivasse, passo direttamente alla prossima iterazione del ciclo (al prossimo treno).
        if(at_destination(forward_trains, i, next_forward)) continue;
        if(at_station(forward_trains[i], train_state)) continue;
        if(at_five(forward_trains[i],train_state)) continue;
        if(at_twenty(forward_trains[i])) continue;
    }

    for(int i = 0; i < next_backward; i++)
    {
        int train_state = backward_trains[i]->get_state();

        if(at_destination(backward_trains, i, next_backward)) continue;
        if(at_station(backward_trains[i], train_state)) continue;
        if(at_five(backward_trains[i],train_state)) continue;
        if(at_twenty(backward_trains[i])) continue;
    }
}


//Controlli relativi ad un treno fermo in stazione.
//Verifica:
//  -se è appena arrivato ed in tal caso lo ferma.
//  -se può ripartire.
//
bool Simulation::at_station(Train* train, int train_state)
{   
    if(train_state == Train::STOPPED && train->past_station()) // true se deve fermasi e la posizione del treno è tra: [pos_stazione, pos_stazione +5)
    {
        int station_index = get_index(train);
        bool origin = train->get_origin();

        if(train->get_current_velocity() != 0)//non è già stato fermato
        {
            train->set_current_velocity(0); //lo fermo
            train->set_position(get_distance(station_index, origin)); //correzzione sulla posizione per tempo discreto.
            train->update_delay(time);
            stations[station_index]->new_stopped_train(train, time); //salva orario d'arrivo per quel treno

            if(train->get_position() >= train->eol()) //true se è al capolinea.
            {
                if(train->get_delay() >= 0)
                {
                    std::cout<<"\n"<< time << ": Il treno "<< train->get_train_number()
                        <<" e' ARRIVATO A DESTINAZIONE con " << train->get_delay() << " minuti di ritardo\n\n";
                }
                else
                {
                    std::cout<<"\n"<< time << ": Il treno "<< train->get_train_number()
                        <<" e' ARRIVATO A DESTINAZIONE con " << std::abs(train->get_delay()) << " minuti di anticipo\n\n";
                }
                
            }
        }
        else if(stations[station_index]->can_restart(train, time)) //controlla che siano passati minimo 5 minuti e che il treno possa ripartire
        {
            stations[station_index]->new_departure(origin, time);
            train->set_current_velocity(Stazione::T_VELOCITY);
            train->has_restarted();
            train->set_state(Train::RUNNING);
            train->update_index();

            if(train->get_position() < train->eol())
            {
                std::cout<< time <<": Il treno " << train->get_train_number()
                    <<" e' ripartito dalla stazione "<< stations[station_index]->get_name() << "\n"; 
            }
        }
        
        return true;
    }

    return false;
}


//Controlli relativi ad un treno che sta entrando in stazione.
//Verifica:
//  -se può fermarsi
//  -se deve essere inserito ai box.
//
bool Simulation::at_five(Train* train, int train_state)
{
    if(train->get_state() != Train::RUNNING && train->past_five()) //true
    {
        int station_index = get_index(train);
        bool origin = train->get_origin();

        switch (train_state)
        {
            case Train::STOPPED:
            {
                if(train->get_current_velocity() != Stazione::T_VELOCITY) //il treno è appena arrivato a -5km dalla stazione
                {
                    train->set_current_velocity(Stazione::T_VELOCITY); //velocità massima nei pressi della stazione
                    train->set_position(get_distance(station_index, origin) - 5); //correzzione sulla posizione
                }
                break;
            }
            case Train::BOXED:
            {
                if(train->get_current_velocity() != 0) // il treno non è già stato messo ai box.
                {
                    boxed_trains.push_back(train);
            
                    train->set_position(get_distance(station_index, origin) - 5); //correzzione sulla posizione del treno.
                    train->set_current_velocity(0); //essendo la velocità a zero l'update sulla sua posizione non crea problemi.

                    std::cout<<time<<": Il treno "<< train->get_train_number() <<" e' ENTRATO AI BOX\n";

                }
                break;
            }
        }

        return true;
    }

    return false;
}


//Controlli effettuati superati i 20km dalla stazione:
//Verifica che:
//  -Se il treno dovrà transitare, fermarsi od andare ai box
//  -Medesimo controllo verrà effettuato più volte. Il risulato restituito inizialmente non cambia.
//
bool Simulation::at_twenty(Train* train)
{
    if(train->past_twenty())
    {
        int station_index = get_index(train);
        bool origin = train->get_origin();

        int train_state = stations[station_index]->handle_new_train(origin, train, time); //comunica al treno cosa dovrà fare alla prossima stazione.
        
        if(train_state == 0 && train->get_state() != 0) //prima volta che viene comunicato che deve fermarsi ai box
        {
            std::cout << time << ": Il treno numero " << train->get_train_number() << " e' a 20 km dalla prossima stazione "
				<< stations[station_index]->get_name() << ". Dovra' sostare ai box.\n";
        }
        
        train->set_state(train_state); //-1 se dovrà fermarsi, 0 se andrà nei box, 1 se transiterà

        //se un treno deve transitare "punta" già alla stazione successiva
        if(train_state == Train::RUNNING)
            train->update_index();

        return true;
    }

    return false;
}


//Verifica se un treno è arrivato a fine corsa.
bool Simulation::at_destination(std::vector<Train*>& vec, int index, int& counter)
{
    if(vec[index]->has_arrived()) //true se pos >= pos_ultima_stazione && state != -1;
    {
        //elimino quel treno dalla lista
        delete vec[index];
        vec.erase(vec.begin() + index);
        counter--;

        return true;
    }

    return false;
}


//Restituisce l'indice della prossima stazione a cui il treno sta arrivando.
//Nel caso un trenno fosse partito dal capolinea è necessario invertire l'indice
int Simulation::get_index(Train* train)
{
    if(train->get_origin())
    {
        return stations.size() - train->get_station_index() - 1;
    }
    else
    {
        return train->get_station_index();
    }
}


//Restituisce la distanza della stazione a "index" rispetto all'origine del treno.
int Simulation::get_distance(int index, bool origin)
{
    if(origin)
        return stations_distances.back() - stations_distances[index];

    return stations_distances[index];
}


//controlla nuove partenza dall'origine/capolinea.
void Simulation::check_new_trains()
{
    if(time == 1440) //dopo mezzannotte non può partire nessun treno.
    {
        //elimino eventuali treni rimasti e non ancora partiti.
        for(int i = next_forward; i < forward_trains.size(); i++)
            delete forward_trains[i];

        forward_trains.erase(forward_trains.begin() + next_forward, forward_trains.end());

        for(int i = next_backward; i < backward_trains.size(); i++)
            delete backward_trains[i];
            
        backward_trains.erase(backward_trains.begin() + next_backward, backward_trains.end());

        return;
    }

    if(time > 1440) return;


    //Verifica eventuali treni in partenza dall'origine e gestisce priorità nella partenza dei treni
    int index {next_forward};
    if(stations[0]->can_leave(false, time))
    {
        for(int i = next_forward; i < forward_trains.size(); i++)
            if(forward_trains[i]->start_time() <= time)
                index++;

        if(index != next_forward) //ci sono treni che devono partire
        {
            check_priority(next_forward, index, forward_trains); //controlla a che treno dare la precedenza
            forward_trains[next_forward]->start();
            forward_trains[next_forward]->has_restarted();
            stations[0]->new_departure(false, time);

            std::cout<< time << ": Il treno " << forward_trains[next_forward]->get_train_number()
                << " e' PARTITO DA " << stations[0]->get_name() << "\n";
            next_forward++;
        }
    }

    //Verifica eventuali treni in partenza dal capolinea e gestisce priorità nella partenza dei treni
    index = next_backward;
    if(stations.back()->can_leave(true, time))
    {
        for(int i = next_backward; i < backward_trains.size(); i++)
            if(backward_trains[i]->start_time() <= time)
                index++;

        if(index != next_backward) //ci sono treni che devono partire
        {
            check_priority(next_backward, index, backward_trains); //controlla a che treno dare la precedenza
            backward_trains[next_backward]->start();
            backward_trains[next_backward]->has_restarted();
            stations.back()->new_departure(true, time);
            
            std::cout<< time << ": Il treno " << backward_trains[next_backward]->get_train_number()
                << " e' PARTITO DA " << stations.back()->get_name() << "\n";
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

    if(a != max_index) //c'è un treno più veloce che vuole partire
    {
        //gli scambio di posto
        Train* temp = v[a];
        v[a] = v[max_index];
        v[max_index] = temp;
    }
}


//Controlla che i treni mantengano la distanza minima richiesta.
//I treni aventi velocità pari a 0 od 80 km/h vengono ignorati perchè si trovano sicuramente nei pressi di una stazione
void Simulation::check_trains_distance()
{
    std::vector<Train*> moving_trains(0);
    for(int i = 0; i < next_forward; i++)
    {
        if(forward_trains[i]->get_current_velocity() != Stazione::T_VELOCITY && forward_trains[i]->get_current_velocity() != 0) //true se è in stazione
            moving_trains.push_back(forward_trains[i]);
    }   

    int size = moving_trains.size();
    for(int i = 0; i < size-1; i++)
    {
        for(int j = i+1; j < size; j++)
        {
            double position1 = moving_trains[i]->get_position();
            double position2 = moving_trains[j]->get_position();
            
            if(std::abs(position1-position2) < MIN_TRAIN_DIST)
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
        if(backward_trains[i]->get_current_velocity() != Stazione::T_VELOCITY && backward_trains[i]->get_current_velocity() != 0) //true se è in stazione
            moving_trains.push_back(backward_trains[i]);
    }

    size = moving_trains.size();
    for(int i = 0; i < size-1; i++)
    {
        for(int j = i+1; j < size; j++)
        {
            double position1 = moving_trains[i]->get_position();
            double position2 = moving_trains[j]->get_position();
            
            if(std::abs(position1-position2) < MIN_TRAIN_DIST)
            {
                int min_velocity = std::min(moving_trains[i]->get_current_velocity(), moving_trains[j]->get_current_velocity());
                moving_trains[i]->set_current_velocity(min_velocity);
                moving_trains[j]->set_current_velocity(min_velocity);
            }
        }
    }
}


void Simulation::update_position()
{
    //aggiorna la posizione dei treni
    for(int i = 0; i < next_forward; i++)
        forward_trains[i]->update();

    for(int i = 0; i < next_backward; i++)
        backward_trains[i]->update();

    //per ogni stazione aggiorna il suo elenco di treni in transito
    for(int i = 0; i < stations.size(); i++)
        stations[i]->update_transit();
}