//Christian Marchiori matr. 1218317

#ifndef Train_h
#define Train_h

#include <iostream>
#include <vector>
#include <cmath>

//classe base
class Train 
{
public: 
    
    //costanti (per state)
    static const int RUNNING = 1;
    static const int BOXED = 0;
    static const int STOPPED = -1;

    //funzioni
    int start_time() const;                     //restituisce l'orario di partenza
    int eol() const;                            //restituisce la poszione dell'ultima stazione (end-of-line)
    virtual bool isRegional() const = 0;        //true se il treno è regionale
    bool isRunning() const;                     //true se lo stato è RUNNING
    void start();                               //fa partire il treno
    void has_restarted();                       //imposta restarted a -1 quando il treno esce dal limite di velocità
    bool has_arrived() const;                   //true se il treno è arrivato a destinazione
    bool past_station() const;                  //true se il treno si trova in stazione (con margine)
    bool past_five() const;                     //true se il treno si trova a 5km dalla prossima stazine (con margine)
    bool past_twenty() const;                   //true se il treno si trova a 20km dalla prossima stazione (con margine)
    void update();                              //aggiorna restarted e position
    virtual void update_delay(int time) = 0;    //aggiorna il ritardo
    int count_prec_secondary() const;           //conta le stazioni secondarie precedenti alla posizone corrente
    
    //getter
    bool get_origin() const{return origin;}
    int get_train_number() const{return train_number;}
    int get_train_type() const{return train_type;}
    int get_position() const{return position;}
    int get_delay() const{return delay;}
    std::vector<int> get_stations_distances() const{return stations_distances;}
    std::vector<bool> get_stations_type() const{return stations_type;}
    std::vector<int> get_distances() const{return distances;}
    std::vector<int> get_arrival_times() const{return arrival_times;}
    std::vector<int> get_signal_points() const{return signal_points;}
    int get_max_velocity() const{return max_velocity;}
    int get_station_index() const{return station_index;}
    int get_state() const{return state;}
    int get_current_velocity() const{return current_velocity;}
    int get_current_rail() const{return current_rail;}
    
    //setter
    void set_position(int pos){position = pos;}
    void set_delay(int del){delay = del;}
    void set_state(int sta){state = sta;}
    void set_current_velocity(int c_vel){current_velocity = c_vel;}
    void set_current_rail(int c_rai){current_rail = c_rai;}
    void update_index(){station_index++;}
    
    Train(const Train& old) = delete;
    Train& operator=(const Train& old) = delete;
    ~Train(){}

protected:
    Train(){};
    virtual void checkTwentyKilometres() = 0;
    virtual void createDistances() = 0;
    void checkArrivalTimes(int num_of_stations);

    //variabili membro
    int origin;             //partenza dall'origine = 0, partenza dal capolinea = 1
    int train_number;       //contiene il numero del treno
    int train_type;         //contiene il tipo di treno
    double position;        //contiene la posizione dall'origine o dal capolinea
    int delay;              //contine il ritardo registrato alla stazione attuale
    int prec_delay;         //contiene il ritardo registrato alla stazione precedente (utile al calcolo della modifica di ritardo) 
    std::vector<int> stations_distances;     //contiene le distanze delle stazioni dall'origine o dal capolinea
    std::vector<bool> stations_type;         //contiene i tipi di stazione (partendo dall'origine o dal capolinea)
    std::vector<int> distances;              //contiene le distanze TRA le stazioni
    std::vector<int> arrival_times;          //contiene i tempi di arrivo alle stazioni
    std::vector<int> signal_points;          //contiene le posizioni in cui bisogna inviare una segnalazione
    int max_velocity;       //non è const perchè non è possibile inizializzare una var. membro tramite 
                            //init. list di un costruttore di una classe 
    int station_index;      //indica la prossima stazione d'arrivo
    int state;              //STOPPED = -1, BOXED = 0, RUNNING = 1
    int current_velocity;   //indica la velocità corrente
    int current_rail;       //indica su che binario si trova il treno (-1 se non è su un binario)
    int restarted;          //da 0 a 4 se il treno si trova entro i 5km successivi alla stazione, -1 altrimenti
};

//classi derivate
class Regional : public Train
{
public:
    Regional(int t_number, bool ori, const std::vector<int>& a_times, const std::vector<bool>& s_type, const std::vector<int>& s_distances);
    bool isRegional() const override;
    void update_delay(int time) override;

private:
    void checkTwentyKilometres() override;
    void createDistances() override;
};

class HighV : public Train
{
public:
    HighV(int t_number, bool ori, const std::vector<int>& a_times, const std::vector<bool>& s_type, const std::vector<int>& s_distances);
    bool isRegional() const override;
    void update_delay(int time) override;

private:
    void checkTwentyKilometres() override;
    void createDistances() override;
};

class HighV_s : public Train
{
public:
    HighV_s(int t_number, bool ori, const std::vector<int>& a_times, const std::vector<bool>& s_type, const std::vector<int>& s_distances);
    bool isRegional() const override;
    void update_delay(int time) override;

private:
    void checkTwentyKilometres() override;
    void createDistances() override;
};

    //funzioni esterne
    int countPrincipalStations(const std::vector<bool>& stations_type);
    void invertStations(std::vector<int>& stations_distances, std::vector<bool>& stations_type);
    void createSignalPoints(const std::vector<int>& stations_distances, std::vector<int>& signal_points);

#endif /* endif of: Train_h */