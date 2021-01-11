//Visentin Francesco 1216419

#include "Simulation.h"

int main()
{
    std::fstream timetable {"timetables.txt",std::ios_base::in};
    std::fstream stations {"line_description.txt", std::ios_base::in};

    if(timetable && stations)
    {
        Simulation sim {timetable, stations};
        sim.simulate();
    }

    
    timetable.close();
    stations.close();

    return 0;
} 
