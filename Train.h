#ifndef Train_h
#define Train_h

#include <vector>
using namespace std;

class Train 
{
public: 
    //costanti (per var. state)
    static const int RUNNING = 1;
    static const int BOXED = 0;
    static const int STOPPED = -1;

    //funzioni
    int start_time() const;                     //restituisce l'orario di partenza
    int eol() const;                            //restituisce la poszione dell'ultima stazione (end-of-line)
    virtual bool isRegional() const = 0;        //true se il treno è regionale
    bool isRunning() const;                     //true se lo stato è RUNNING
    void start();                               //fa partire il treno
    void has_restarted();
    bool has_arrived() const;                   //true se il treno è arrivato a destinazione
    bool past_station() const;                  //true se il treno si trova in stazione (con margine)
    bool past_five() const;                     //true se il treno si trova a 5km dalla prossima stazine (con margine)
    bool past_twenty() const;                   //true se il treno si trova a 20km dalla prossima stazione (con margine)
    void update();
    virtual void update_delay(int time) = 0;    //aggiorna il ritardo
    int count_prec_secondary() const;           //conta le stazioni secondarie precedenti alla posizone corrente
    
    //getter
    bool get_origin() const{return origin;}
    int get_train_number() const{return train_number;}
    int get_train_type() const{return train_type;}
    int get_position() const{return position;}
    int get_delay() const{return delay;}
    vector<int> get_stations_distances() const{return stations_distances;}
    vector<bool> get_stations_type() const{return stations_type;}
    vector<int> get_distances() const{return distances;}
    vector<int> get_arrival_times() const{return arrival_times;}
    vector<int> get_signal_points() const{return signal_points;}
    int get_max_velocity() const{return max_velocity;}
    int get_station_index() const{return station_index;}
    int get_state() const{return state;}
    int get_current_velocity() const{return current_velocity;}
    int get_current_rail() const{return current_rail;}
    
    //setter
    void update_index(){station_index++;}
    void set_position(int pos){position = pos;}
    void set_delay(int del){delay = del;}
    void set_state(int sta){state = sta;}
    void set_current_velocity(int c_vel){current_velocity = c_vel;}
    void set_current_rail(int c_rai){current_rail = c_rai;}

protected:
    Train(){};
    virtual void checkTwentyKilometres() = 0;
    virtual void createDistances() = 0;
    void checkArrivalTimes(int num_of_stations);

    //principali
    int origin;             //partenza dall'origine = 0, partenza dal capolinea = 1
    int train_number;       //contiene il numero del treno
    int train_type;         //contiene il tipo di treno
    double position;        //deve essere modificata in runtime (mi da la posizione da origine o da capolinea)
    int delay;              //contine il ritardo registrato alla stazione attuale
    int prec_delay;         //contiene il ritardo registrato alla stazione precedente (utile al calcolo della modifica di ritardo) 
    vector<int> stations_distances;     //contiene le distanze delle stazioni dall'origine o dal capolinea
    vector<bool> stations_type;         //contiene i tipi di stazione (partendo dall'origine o dal capolinea)
    vector<int> distances;              //contiene le distanze TRA le stazioni
    vector<int> arrival_times;          //contiene i tempi di arrivo alle stazioni
    vector<int> signal_points;          //contiene le posizioni in cui bisogna inviare una segnalazione
    int max_velocity;       //non è const perchè non è possibile inizializzare una var. membro tramite 
                            //init. list di un costruttore di una classe 
    int station_index;      //indica la prossima stazione d'arrivo
    int state;              //STOPPED = -1, BOXED = 0, RUNNING = 1
    int current_velocity;   //indica la velocità corrente
    int current_rail;       //indica su che binario si trova il treno (-1 se non è su un binario)
    int restarted;
};

class Regional : public Train
{
public:
    Regional(int t_number, bool ori, const vector<int>& a_times, const vector<bool>& s_type, const vector<int>& s_distances);
    void checkTwentyKilometres() override;
    void createDistances() override;
    bool isRegional() const override;
    void update_delay(int time) override;
};

class HighV : public Train
{
public:
    HighV(int t_number, bool ori, const vector<int>& a_times, const vector<bool>& s_type, const vector<int>& s_distances);
    void checkTwentyKilometres() override;
    void createDistances() override;
    bool isRegional() const override;
    void update_delay(int time) override;
};

class HighV_s : public Train
{
public:
    HighV_s(int t_number, bool ori, const vector<int>& a_times, const vector<bool>& s_type, const vector<int>& s_distances);
    void checkTwentyKilometres() override;
    void createDistances() override;
    bool isRegional() const override;
    void update_delay(int time) override;
};

    //funzioni esterne
    int countPrincipalStations(const vector<bool>& stations_type);
    void invertStations(vector<int>& stations_distances, vector<bool>& stations_type);
    void createSignalPoints(const vector<int>& stations_distances, vector<int>& signal_points);

#endif /* endif of: Train_h */