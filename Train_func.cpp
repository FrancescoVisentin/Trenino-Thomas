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
    if(running == true)
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
    running = true;
    station_index++;
    set_current_velocity(80);   
    set_current_station(0);     //devo farmela passare
    set_current_binary(1);      //devo farmelo passare
}