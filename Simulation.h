//Visentin Francesco matricola 1216419

#ifndef Simultaion_h
#define Simulation_h

#include "Train.h"
#include "Stazione.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>


class Simulation
{
    public:
        //Costruttore:
        //Crea e salva tutte le stazioni valide presenti in line_descriptions.txt.
        //Crea e salva tutti i treni previsti in timetables.txt. Corregge eventuali valori errati/mancanti.
        Simulation(std::istream& timetable, std::istream& line);


        //Copy constructors/assignment operator disabilitati. Non sono ritenute operazioni valide. 
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
        static const int MIN_TRAIN_DIST = 13; //distanza minima tra due treni. Impostata a 13 per
                                              //evitare errrori dovuti alla granularità del tempo.

        std::vector<Train*> forward_trains; //contiene tutti i treni che partono dall'origne
        std::vector<Train*> backward_trains; //contiene tutti i treni che partono dal capolinea
        std::vector<Train*> boxed_trains;
        std::vector<Stazione*> stations; //contiene tutti le stazioni presenti in line_description.txt
        std::vector<int> stations_distances;
        std::vector<bool> stations_type;
        int time = 0;
        bool ready = true; //indica se l'oggetto può effettuare una simulazione.


        //indici dei prossimi treni che devono partire
        int next_forward = 0;
        int next_backward = 0;


        //funzioni usate dal costruttore per ricavare i dati dai due file di testo.
        void parse_line(std::istream& line);
        void parse_timetable(std::istream& timetables, std::vector<Train*>& vec);


        //funzioni usate da simulate() per riprodurre ed analizzare lo stato della linea ferroviaria nel tempo.
        void check_trains_distance();
        void check_box();
        void check_new_trains();
        void update_position();

        //funzione usata da check_new_trains()
        void check_priority(int a, int b, std::vector<Train*>& v);

        //funzioni usate da check_position()
        void check_position();
        bool at_station(Train* train, int train_state);
        bool at_five(Train* train, int train_state);
        bool at_twenty(Train* train);
        bool at_destination(std::vector<Train*>& vec, int index, int& counter);
        int get_index(Train* train);
        int get_distance(int index, bool origin);
};

#endif