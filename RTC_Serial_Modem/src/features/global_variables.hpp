#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <Arduino.h>

struct NetworkTime
{
    uint8_t networkhour;
    uint8_t networkminute;
    uint8_t networkseconds;
};

struct NetworkDate
{
    uint8_t networkyear;
    uint8_t networkmonth;
    uint8_t networkday;
};

//default 42 see the first change. works for hour,month,day als logical indicator
extern uint8_t NETWORK_HOUR;
//default 42 see the first change. works for hour,month,day als logical indicator
extern uint8_t NETWORK_MINUTE;
//default 42 to see the first change. works for hour,month,day als logical indicator
extern uint8_t NETWORK_SECOND;
//default 42 to see the first change. works for hour,month,day als logical indicator
extern uint8_t NETWORK_YEAR;
//default 42 to see the first change. works for hour,month,day als logical indicator
extern uint8_t NETWORK_MONTH;
//default 42 to see the first change. works for hour,month,day als logical indicator
extern uint8_t NETWORK_DAY;

extern bool NETWORK_TIME_GLOBAL;

#endif
