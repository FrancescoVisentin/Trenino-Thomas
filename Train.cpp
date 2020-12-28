#include <vector>
using namespace std;

class Train 
{
private:
    int position;
    int delay;
    int velocity;
};

class Regional : public Train
{
public:
    Regional(bool origin, int train_type, vector<int> arrival_time);
private:
    int position;
    int delay;
    const int velocity = 160;
};

class HighV : public Train
{
public:
    HighV(bool origin, int train_type, vector<int> arrival_time);
private:
    int position;
    int delay;
    const int velocity = 240;
};

class HighV_s : public Train
{
public:
    HighV_s(bool origin, int train_type, vector<int> arrival_time);
private:
    int position;
    int delay;
    const int velocity = 300;
};

Regional::Regional(bool origin, int train_type, vector<int> arrival_time)
{
    if(origin = 0)
    {
        position = 0;
    }
    else
    {
        vector<int> a = get_distances();
        position = a.back();        //devo inserire l'ultimo valore del vector distanze
    }
    delay = 0;
}

HighV::HighV(bool origin, int train_type, vector<int> arrival_time)
{
    if(origin = 0)
    {
        position = 0;
    }
    else
    {
        vector<int> a = get_distances();
        position = a.back();        //devo inserire l'ultimo valore del vector distanze
    }
    delay = 0;
}

HighV_s::HighV_s(bool origin, int train_type, vector<int> arrival_time)
{
    if(origin = 0)
    {
        position = 0;
    }
    else
    {
        vector<int> a = get_distances();
        position = a.back();        //devo inserire l'ultimo valore del vector distanze
    }
    delay = 0;
}