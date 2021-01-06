#include "Train.h"

#include <cmath>

//functions

int Train::start_time()
{
    return arrival_times1.at(0);
}

bool Train::isRegional()
{
    if(train_type1 == 1)
    {return true;}
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

bool Train::isNearby()
{
    if(abs((position-(stations_distances1.at(station_index)))) <= 5)
    {
        return true;
    }
    return false;
}

void Train::start()
{
    //il trenino thomas è pronto a schiantarsi
    int pos = 0;
    if(origin1 == 0)         //se l'origine è 0 la posizione è al km 0
    {
        pos = 0;
    }
    else                    //se l'origine è 1 la posizione è al km massimo dalla stazione di origine
    {
        pos = stations_distances1.back();
    }
    set_position(pos);
    set_delay(0);
    state = 1;
    station_index++;
    set_current_velocity(80);   
    //set_current_station(0);     //devo farmela passare
    set_current_rail(1);      //devo farmelo passare
}

bool Train::has_arrived()
{
    if(position >= stations_distances1.back())
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
    if((position >= stations_distances1.at(station_index)) && (position < (stations_distances1.at(station_index) + 3)))
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
    if((position >= stations_distances1.at(station_index) - 5) && (position < (stations_distances1.at(station_index) - 2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Train::past_twenty()    //cout
{
    if((position >= stations_distances1.at(station_index) - 20) && (position < (stations_distances1.at(station_index) - 5)))
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
    position = position + (current_velocity/60);
}

void Train::update_delay(int time)      //da sistemare
{
    if(train_type1 == 1)
    {
        delay = time - arrival_times1.at(station_index);
    }
    else
    {
        int arrival_index = station_index - count_prec_secondary();
        delay = time - arrival_times1.at(arrival_index);
    }
    
}

void Train::update_station_index()
{
    //meglio farlo da simulation
}

int Train::count_prec_secondary()       //da sistemare
{
    int c = 0;
    for(int i=0; i < stations_distances1.at(station_index); i++)
    {
        if(stations_type1.at(i) == 1)
        {
            c++;
        }
    }
    return c;
}