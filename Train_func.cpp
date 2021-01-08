#include "Train.h"
#include <iostream>

#include <cmath>

//functions

int Train::start_time()
{
    return arrival_times.at(0);
}

int Train::eol()
{
    return stations_distances.back();
}

bool Regional::isRegional()
{
    return true;
}
bool HighV::isRegional()
{
    return false;
}
bool HighV_s::isRegional()
{
    return false;
}

bool Train::isRunning()
{
    if(state == 1)
    {
        return true;
    }
    return false;
}

void Train::start()
{
    //il trenino thomas è pronto a schiantarsi
    int pos = 0;
    if(origin == 0)         //se l'origine è 0 la posizione è al km 0
    {
        pos = 0;
    }
    else                    //se l'origine è 1 la posizione è al km massimo dalla stazione di origine
    {
        pos = stations_distances.back();
    }
    set_position(pos);
    set_delay(0);
    state = 1;
    station_index++;
    set_current_velocity(80);   
    //set_current_station(0);     //devo farmela passare
    set_current_rail(1);      //devo farmelo passare
}

void Train::has_restarted()
{
    restarted = 0;
}

bool Train::has_arrived(int time)
{
    if(position >= stations_distances.back() && state != -1)
    {
        cout << time << ": Il treno [num treno] è arrivato a destinazione con " << delay << " minuti di ritardo"; 
        return true;
    }
    else
    {
        return false;
    }
}

bool Train::past_station(int time)
{
    if((position >= stations_distances.at(station_index)) && (position < (stations_distances.at(station_index) + 3)))
    {
        int mod_delay = prec_delay - delay;
        if(mod_delay < 0)
        {
            cout << time << " : Il ritardo è aumentato di " << abs(mod_delay) << " minuti.\n";
        }
        else
        {
            cout << time << " : Il ritardo è diminuito di " << mod_delay << " minuti.\n";
        }
        return true;
    }
    else
    {
        return false;
    }
    
}

bool Train::past_five()
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

bool Train::past_twenty(int time)
{
    if((position >= stations_distances.at(station_index) - 20) && (position < (stations_distances.at(station_index) - 5)))
    {
        cout << time << " : Il treno numero [num treno] è a 20 km dalla prossima stazione"; 
        return true;
    }
    else
    {
        return false;
    }
}

void Train::update()
{
    if(restarted >= 0)
    {
        restarted++;
    }
    if(restarted > 4)
    {
        set_current_velocity(max_velocity);
        restarted = -1;
    }
    position = position + (current_velocity/60);
}

void Regional::update_delay(int time)
{
    prec_delay = delay;
    delay = time - arrival_times.at(station_index-1);
}
void HighV::update_delay(int time)
{
    int arrival_index = (station_index-1) - count_prec_secondary();
    delay = time - arrival_times.at(arrival_index);
}
void HighV_s::update_delay(int time)
{
    int arrival_index = (station_index-1) - count_prec_secondary();
    delay = time - arrival_times.at(arrival_index);
}

int Train::get_station_index()
{
    while(station_index < stations_distances.size() && position > stations_distances.at(station_index))
    {
        station_index++;
    }
    return station_index;
}

int Train::count_prec_secondary()
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