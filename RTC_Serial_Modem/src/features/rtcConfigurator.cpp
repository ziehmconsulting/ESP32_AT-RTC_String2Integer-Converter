#include "RTClib.h"

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void adjustRTC(u)
{
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    rtc.adjust(DateTime(2017, 7, 16, 16, 35, 20));
}

void getTime()
{
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

void setupRTC()
{
    // Serial.begin(115200);

    // start the connexion to the RTC

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ; // can't go further
    }

    // Uncomment this code to set the time of your RTC to the last compile time of this code
    // if you compile and upload in one go, that will be close enough to a good time
    // this is convenient as you don't need to manually mess around with the time manually

    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // or uncomment this line to decide what you want as a date/time for your RTC
    // format is year, month, day, hours, minutes, seconds
    // but you need to set that manually, take into account it takes a bit of time to compile, upload and run
    // so give yourself some padding in the seconds and check your wall clock and press reset on your arduino
    // when it's the right time.

    // rtc.adjust(DateTime(2017, 7, 16, 16, 35, 20));

    Serial.println("RTC date is set");
}
