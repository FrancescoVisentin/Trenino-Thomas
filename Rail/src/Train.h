
#ifndef Train_h
#define Train_h

#include <vector>
using namespace std;

class Train 
{
public:
    static const int RUNNING = 1;
    static const int BOXED = 0;
    static const int STOPPED = -1;
    //funcions
    int start_time();
    int eol();
    virtual bool isRegional() = 0;
    bool isRunning();
    bool isNearby();
    void start();
    void has_restarted();
    bool has_arrived(int time);
    bool past_station();
    bool past_five();
    bool past_twenty();
    void update();
    virtual void update_delay(int time) = 0;
    void update_station_index();
    int count_prec_secondary();
    
    //getter
    bool get_origin(){return origin;}
    int get_train_number(){return train_number;}
    int get_train_type(){return train_type;}
    int get_position(){return position;}
    int get_delay(){return delay;}
    vector<int> get_stations_distances(){return stations_distances;}
    vector<bool> get_stations_type(){return stations_type;}
    vector<int> get_distances(){return distances;}
    vector<int> get_arrival_times(){return arrival_times;}
    vector<int> get_signal_points(){return signal_points;}
    const int get_max_velocity(){return max_velocity;}
    
    //altri getter
    int get_station_index(){return station_index;}
    int get_state(){return state;}
    int get_current_velocity(){return current_velocity;}
    int get_current_rail(){return current_rail;}
    
    //setter
    void update_index(){station_index++;}
    void set_position(int pos){position = pos;}
    void set_delay(int del){delay = del;}
    void set_state(int sta){state = sta;}
    void set_current_velocity(int c_vel){current_velocity = c_vel;}
    void set_current_rail(int c_rai){current_rail = c_rai;}
    
    int countPrincipalStations(vector<bool>& stations_type);
    void invertStations(vector<int>& stations_distances, vector<bool>& stations_type);
    void createSignalPoints(vector<int>& stations_distances, vector<int>& signal_points);

protected:
    Train(){};    
    //virtual int whatVelocity() = 0;
    virtual void checkTwentyKilometres() = 0;
    virtual void createDistances(vector<int>& distances) = 0;
    void checkArrivalTimes(int velocty, int num_of_stations);
    

    //principali
    int origin;
    int train_number;
    int train_type;
    double position;       //deve essere modificata in runtime (mi da la posizione da origine o da capolinea)
    int delay;          //deve essere modificata in runtime
    int prec_delay;
    vector<int> stations_distances;
    vector<bool> stations_type;
    vector<int> distances;
    vector<int> arrival_times;
    vector<int> signal_points;
    int max_velocity;       //non è const perchè non è possibile inizializzare una var. membro tramite 
                            //init. list di un costruttore di una classe 
    
    //altre (devono essere modificate in runtime)
    int station_index;      //indica la prossima stazione d'arrivo (deve aumentare anche quando si transita)
    int state;           //fermo = -1, box = 0, movimento/transita = 1
    int current_velocity;
    int current_rail;         //-1 se non è su un binario
    int restarted;
};

class Regional : public Train
{
public:
    Regional(int t_number, bool ori, vector<int> a_times, vector<bool> s_type, vector<int> s_distances);
    //int whatVelocity() override;
    void checkTwentyKilometres() override;
    void createDistances(vector<int>& distances) override;
    //void checkArrivalTimes() override;
    //const int max_velocity = 160;
    bool isRegional() override;
    void update_delay(int time) override;
};

class HighV : public Train
{
public:
    HighV(int t_number, bool ori, vector<int> a_times, vector<bool> s_type, vector<int> s_distances);
    //int whatVelocity() override;
    void checkTwentyKilometres() override;
    void createDistances(vector<int>& distances) override;
    //void checkArrivalTimes() override;
    //const int max_velocity = 240;
    bool isRegional() override;
    void update_delay(int time) override;
};

class HighV_s : public Train
{
public:
    HighV_s(int t_number, bool ori, vector<int> a_times, vector<bool> s_type, vector<int> s_distances);
    //int whatVelocity() override;
    void checkTwentyKilometres() override;
    void createDistances(vector<int>& distances) override;
    //void checkArrivalTimes() override;
    //const int max_velocity = 300;
    bool isRegional() override;
    void update_delay(int time) override;
};

#endif /* endif of: Train_h */