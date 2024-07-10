#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <Arduino.h>

struct NetworkTime
{
    int networkhour;
    int networkminute;
    int networkseconds;
};

struct NetworkDate
{
    int networkyear;
    int networkmonth;
    int networkday;
};

extern int NETWORK_HOUR;
extern int NETWORK_MINUTE;
extern int NETWORK_SECOND;
extern int NETWORK_YEAR;
extern int NETWORK_MONTH;
extern int NETWORK_DAY;

#endif
