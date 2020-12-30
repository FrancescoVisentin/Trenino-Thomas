#ifndef Simultaion_h
#define Simulation_h

#include "Train.h"
#include "Station.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Simulation
{
    public:
        /**
         * @brief Costruttore. Crea e salva tutti i treni previsti nella timetable.
         * 
         * @param timetable Lista dei treni in partenza.
         * @param line Descrizione linea ferroviaria
         */
        Simulation(std::istream& timetable, std::istream& line);


        /**
         * @brief Effettua l'intera simulazione. Stampa a video le varie segnalazioni.
         * 
         */
        void simulate();

        //dealloca tutto quello che è rimasto in giro(non dovrebbe servire).
        ~Simulation();//DA FARE

    private:
        std::vector<Train*> trains;
        std::vector<Station*> stations; 
        std::vector<int> stations_distances;
        std::vector<bool> stations_type;
        int time = 0;

        //funzioni usate dal costruttore per ricavare i dati dai due file di testo.
        void parse_timetable(std::istream& timetables);
        void parse_line(std::istream& line);


        //funzioni usate da simulate per riprodurre ed analizzare lo stato della linea ferroviaria nel tempo.
        void check_new_trains(){}; //DA FARE
        void check_incoming_trains(){}; //DA FARE
        void check_trains_distance(){}; //DA FARE
        void check_starting_trains(){}; //DA FARE
};

#endif