#ifndef GravityTracker_H
#define GravityTracker_H
#define NumberOfSlots 72
#define InvalidValue 0
#define AveragePeriod  3600

class GravityTracker
{
    int _idx;
    int16_t _record[NumberOfSlots];
    uint32_t _currentStartTime;
    int16_t _lastValue;

    void addRecord(uint16_t v){
        _record[_idx++]=v;
        if(_idx >= NumberOfSlots) _idx=0;
    }

public:
    GravityTracker(void){
        for(int i=0;i<NumberOfSlots;i++){
            _record[i]=InvalidValue;
        }
        _idx=0;
        _currentStartTime=0;
        _lastValue=0;
    }

    bool stable(int duration,int to=1){
        int current = _idx -1;
        if(current < 0) current =NumberOfSlots-1;
        int previous = NumberOfSlots + _idx - duration ;
        while(previous >= NumberOfSlots) previous -= NumberOfSlots;
        return (_record[previous] - _record[current]) <= to;
    }

    void add(float fgravity,uint32_t time){
        uint16_t gravity =round(fgravity * 1000.0);
        uint32_t timediff = time - _currentStartTime;

        if(timediff > AveragePeriod){
           addRecord(gravity);
           DBG_PRINTF("add %d @%ld\n",gravity,time);
           if(_lastValue >0){
               timediff -= AveragePeriod;
               while(timediff > AveragePeriod){
                    timediff -=AveragePeriod;
                    addRecord(_lastValue);
                }
            }
            _currentStartTime=time;
            _lastValue=gravity;
        }
    }
};

extern GravityTracker gravityTracker;
#endif
