#ifndef Train_h
#define Train_h

#include <vector>
using namespace std;

class Train 
{
public: 
    //funcions
    int start_time();
    bool isRegional();
    bool isRunning();
    bool isNearby();
    void start();
    bool has_arrived();
    bool past_station();
    bool past_five();
    bool past_twenty();
    void update();
    void update_delay(int time);
    void update_station_index();
    int count_prec_secondary();
    
    //getter
    bool get_origin(){return origin1;}
    int get_train_type(){return train_type1;}
    int get_position(){return position;}
    int get_delay(){return delay;}
    vector<int> get_stations_distances(){return stations_distances1;}
    vector<bool> get_stations_type(){return stations_type1;}
    vector<int> get_distances(){return distances1;}
    vector<int> get_arrival_times(){return arrival_times1;}
    vector<int> get_signal_points(){return signal_points1;}
    const int get_max_velocity(){return max_velocity;}
    
    //altri getter
    int get_station_index(){return station_index;}
    bool get_state(){return state;}
    int get_current_velocity(){return current_velocity;}
    //int get_current_station(){return current_station;}
    int get_current_rail(){return current_rail;}
    
    //setter
    void set_position(int pos){position = pos;}
    void set_delay(int del){delay = del;}
    void set_state(bool sta){state = sta;}
    void set_current_velocity(int c_vel){current_velocity = c_vel;}
    //void set_current_station(int c_sta){current_station = c_sta;}
    void set_current_rail(int c_rai){current_rail = c_rai;}

protected:    
    Train(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances);
    
    //principali
    int origin1;
    int train_type1;
    double position;       //deve essere modificata in runtime
    int delay;          //deve essere modificata in runtime
    vector<int> stations_distances1;
    vector<bool> stations_type1;
    vector<int> distances1;
    vector<int> arrival_times1;
    vector<int> signal_points1;
    const int max_velocity;
    
    //altre (devono essere modificate in runtime)
    int station_index;      //indica la prossima stazione d'arrivo (deve aumentare anche quando si transita)
    bool state;           //fermo = -1, box = 0, movimento/transita = 1
    int current_velocity;
    //int current_station;        //-1 se non è in una stazione -------> sistemare station index nel costruttore e in start
    int current_rail;         //-1 se non è su un binario
};

class Regional : public Train
{
public:
    Regional(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances);
};

class HighV : public Train
{
public:
    HighV(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances);
};

class HighV_s : public Train
{
public:
    HighV_s(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances);
};

//funzioni utili al costruttore
int whatVelocity(int train_type);
int countPrincipalStations(vector<bool>& stations_type);
void invertStations(vector<int>& stations_distances, vector<bool>& stations_type);
void checkTwentyKilometres(vector<int>& stations_distances, vector<bool>& stations_type, vector<int>& arrival_times, int train_type);
void createDistances(vector<int>& stations_distances, vector<bool>& stations_type, vector<int>& distances, int train_type);
void checkArrivalTimes(int num_of_all_stations, int num_of_principal_stations, vector<bool>& stations_type, vector<int>& distances, vector<int>& arrival_times, int velocity, int train_type);
void createSignalPoints(vector<int>& stations_distances, vector<int>& signal_points);

#endif /* endif of: Train_h */