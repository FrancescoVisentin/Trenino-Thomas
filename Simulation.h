#ifndef Simultaion_h
#define Simulation_h

#include "Train.h"
#include "Stazione1.h"  //QUESTA PER ORA ME LA SON FATTA IO
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Simulation
{
    public:
        //Costruttore:
        //Crea e salva tutte le stazioni valide presenti in line_descriptions.txt.
        //Crea e salva tutti i treni previsti in timetables.txt. Corregge eventuali valori errati/mancanti.
        Simulation(std::istream& timetable, std::istream& line);


        //Copy constructors/assignment operator disabilitati. (DA VALUTARE) 
        Simulation(const Simulation&) = delete;
        Simulation& operator = (const Simulation&) = delete;


        //Effettua l'intera simulazione simulando lo scorrere del tempo. Stampa a video le varie segnalazioni richieste.
        void simulate();


        //Per poter effettuare nuovamente la stessa simulazione è necessario "resettare" l'oggetto.
        void reset_simulation(std::istream& timetables, std::istream& line);

        //dealloca memoria occupata dai Train* e Stazione*.
        ~Simulation();

    private:
        //costanti usate per evitare "magic numbers" nel codice.
        static const int T_REGIONALE = 1;
        static const int T_VELOCE = 2;
        static const int T_SUPER = 3;
        static const int S_PRINCIPALE = 0;
        static const int S_LOCALE = 1;
        static const int INVALID_STATION = -1; //usata per segnalare stazioni troppo vicine.
        static const int MIN_DISTANCE = 20; //distanza minima tra due stazioni.


        std::vector<Train*> trains; //contiene tutti i treni presenti in timetables.txt
        std::vector<Stazione*> stations; //contiene tutti le stazioni presenti in line_description.txt
        std::vector<int> stations_distances;
        std::vector<bool> stations_type;
        int time = 0;
        bool ready = true; //L'oggetto può effettuare una simulazione.

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