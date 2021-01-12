//Christian Marchiori matr. 1218317

#include "Train.h"

//functions

int Train::start_time() const
{
    return arrival_times.at(0);
}

int Train::eol() const
{
    return stations_distances.back();
}

bool Regional::isRegional() const
{
    return true;
}
bool HighV::isRegional() const
{
    return false;
}
bool HighV_s::isRegional() const
{
    return false;
}

bool Train::isRunning() const
{
    if(state == RUNNING)
    {
        return true;
    }
    return false;
}

void Train::start()
{
    set_delay(0);
    state = RUNNING;
    station_index++;
    set_current_velocity(80);
}

void Train::has_restarted()
{
    restarted = 0;
}

bool Train::has_arrived() const
{
    if(position >= stations_distances.back() && state != STOPPED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Train::past_station() const
{
    if((position >= stations_distances.at(station_index)) && (position < (stations_distances.at(station_index) + 3)))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool Train::past_five() const
{
    if((position >= stations_distances.at(station_index) - 5) && (position < (stations_distances.at(station_index))))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Train::past_twenty() const
{
    if((position >= stations_distances.at(station_index) - 20) && (position < (stations_distances.at(station_index) - 5)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Train::update()
{
    if(restarted >= 0)      //se il treno si trova entro i 5km successivi ad una stazione restarted viene incrementato
    {
        restarted++;
    }
    if(restarted > 4)       //se il treno è uscito dal limite
    {
        set_current_velocity(max_velocity);
        restarted = -1;
    }
    position = position + (current_velocity/60.);       //aggiorno la posizione in base alla velocità
}

void Regional::update_delay(int time)
{
    prec_delay = delay;         //memorizzo il ritardo alla stazione precedente in prec_delay
    delay = time - arrival_times.at(station_index);     //calcolo il nuovo delay
    
    int mod_delay = prec_delay - delay;         //calcolo la variazione del delay
    if(mod_delay == 0)      //stampe output in base alla variazione di ritardo
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' variato di 0 minuti.\n";
    }
    else if(mod_delay < 0)
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' aumentato di " << abs(mod_delay) << " minuti.\n";
    }
    else
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' diminuito di " << mod_delay << " minuti.\n";
    }
}
void HighV::update_delay(int time)
{
    int arrival_index = station_index - count_prec_secondary();
    delay = time - arrival_times.at(arrival_index);     //calcolo il nuovo delay

    int mod_delay = prec_delay - delay;         //calcolo la variazione del delay
    if(mod_delay == 0)      //stampe output in base alla variazione di ritardo
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' variato di 0 minuti.\n";
    }
    else if(mod_delay < 0)
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' aumentato di " << abs(mod_delay) << " minuti.\n";
    }
    else
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' diminuito di " << mod_delay << " minuti.\n";
    }
}
void HighV_s::update_delay(int time)
{
    int arrival_index = station_index - count_prec_secondary();
    delay = time - arrival_times.at(arrival_index);

    int mod_delay = prec_delay - delay;
    if(mod_delay == 0)      //stampe output in base alla variazione di ritardo
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' variato di 0 minuti.\n";
    }
    else if(mod_delay < 0)
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' aumentato di " << abs(mod_delay) << " minuti.\n";
    }
    else
    {
        std::cout << time << ": Il ritardo del treno " << train_number << " e' diminuito di " << mod_delay << " minuti.\n";
    }
}

int Train::count_prec_secondary() const
{
    int c = 0;
    for(int i=0; i < station_index; i++)
    {
        if(stations_type.at(i) == 1)
        {
            c++;
        }
    }
    return c;
}