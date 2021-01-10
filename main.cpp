#include "Simulation.h"

int main()
{
    std::fstream timetable {"timetables3.txt",std::ios_base::in};
    std::fstream stations {"line_description2.txt", std::ios_base::in};

    Simulation sim {timetable, stations};
    sim.simulate();

    std::cout<<"non sono esploso\n";

    timetable.close();
    stations.close();

    return 0;
} 
