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
    set_delay(0);
    state = 1;
    station_index++;
    set_current_velocity(80);   
    //set_current_rail(1);      //il set penso sia fatto da Station
}

void Train::has_restarted()
{
    restarted = 0;
}

bool Train::has_arrived(int time)
{
    if(position >= stations_distances.back() && state != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Train::past_station()
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

bool Train::past_twenty()
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
    if(restarted >= 0)
    {
        restarted++;
    }
    if(restarted > 4)
    {
        set_current_velocity(max_velocity);
        restarted = -1;
    }
    position = position + (current_velocity/60.);
}

void Regional::update_delay(int time)
{
    prec_delay = delay;
    delay = time - arrival_times.at(station_index);

    int mod_delay = prec_delay - delay;
    if(mod_delay < 0)
    {
        cout << time << ": Il ritardo del treno " << train_number << " è aumentato di " << abs(mod_delay) << " minuti.\n";
    }
    else
    {
        cout << time << ": Il ritardo del treno " << train_number << " è diminuito di " << mod_delay << " minuti.\n";
    }
}
void HighV::update_delay(int time)
{
    int arrival_index = (station_index) - count_prec_secondary();
    delay = time - arrival_times.at(arrival_index);

    int mod_delay = prec_delay - delay;
    if(mod_delay < 0)
    {
        cout << time << ": Il ritardo del treno " << train_number << " è aumentato di " << abs(mod_delay) << " minuti.\n";
    }
    else
    {
        cout << time << ": Il ritardo del treno " << train_number << " è diminuito di " << mod_delay << " minuti.\n";
    }
}
void HighV_s::update_delay(int time)
{
    int arrival_index = (station_index) - count_prec_secondary();
    delay = time - arrival_times.at(arrival_index);

    int mod_delay = prec_delay - delay;
    if(mod_delay < 0)
    {
        cout << time << ": Il ritardo del treno " << train_number << " è aumentato di " << abs(mod_delay) << " minuti.\n";
    }
    else
    {
        cout << time << ": Il ritardo del treno " << train_number << " è diminuito di " << mod_delay << " minuti.\n";
    }
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