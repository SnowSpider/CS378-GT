//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef TIMES_HPP
#define TIMES_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <cstdlib>

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace std;

class Times
{
public:
    time_t time;
    int cellID;

    Times(){
        time = 0;
        cellID = 0;
    }

    Times(const time_t temptime, const int tempcellID){
        time = temptime;
        cellID = tempcellID;
    }

    Times& operator=(const Times& f){
        time = f.time;
        cellID = f.cellID;
        return *this;
    }

    bool operator==(const Times& v) const {
        if(time==v.time && cellID==v.cellID) return true;
        else return false;
    }

    bool operator!=(const Times &other) const {
        return !(*this == other);
    }

    bool operator()( Times j1,Times j2)
    {
        if(j2.time < j1.time)
            return true;

            return false;
    }

};
class CompareTimes
{
public:
    bool operator()( Times j1,Times j2)
    {
        if(j2.time < j1.time)
            return true;

            return false;
    }
};
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
