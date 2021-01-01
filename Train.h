#ifndef Train_h
#define Train_h

#include <vector>
using namespace std;

class Train 
{
protected:    
    Train(bool origin, int train_type, vector<int> arrival_times, vector<bool> stations_type, vector<int> stations_distances);
    int position;
    int delay;
    vector<int> stations_distances1;
    vector<bool> stations_type1;
    vector<int> distances1;
    vector<int> arrival_times1;
    vector<int> segnalation_points1;
    const int velocity;
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

int whatVelocity(int train_type);
int countPrincipalStations(vector<bool>& stations_type);
void checkTwentyKilometres(vector<int>& stations_distances, vector<bool>& stations_type, vector<int>& arrival_times, int train_type);
void createDistances(vector<int>& stations_distances, vector<bool>& stations_type, vector<int>& distances, int train_type);
void checkArrivalTimes(int num_of_all_stations, int num_of_principal_stations, vector<bool>& stations_type, vector<int>& distances, vector<int>& arrival_times, int velocity, int train_type);
void createSegnalationPoints(vector<int>& stations_distances, vector<int>& segnalation_points);

#endif /* endif of: Train_h */