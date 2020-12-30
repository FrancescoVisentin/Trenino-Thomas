#ifndef Train_h
#define Train_h

#include <vector>
using namespace std;

class Train 
{
protected:    
    Train(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances);
    int position;
    int delay;
    const int velocity;
};

class Regional : public Train
{
public:
    Regional(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances);
};

class HighV : public Train
{
public:
    HighV(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances);
};

class HighV_s : public Train
{
public:
    HighV_s(bool origin, int train_type, vector<int> arrival_times, bool stations_type, vector<int> stations_distances);
};

int whatVelocity(int train_type);
void checkArrivalTimes(vector<int>& arrival_times, vector<int> distances, int velocity, int num_of_stations);


#endif /* endif of: Train_h */