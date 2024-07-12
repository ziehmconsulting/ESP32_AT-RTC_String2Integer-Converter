/*H********************************************************************************************
** FILENAME :        NetworkTimeConverter.cpp
** Version  :        1.0.0
** DESCRIPTION :
**       Converter from AT-Networktime as a String to integer. Used AT+Command is "AT+CCLK?"
**       (not implemented yet) Writing integers to global Varibles
**
** PUBLIC FUNCTIONS :
**       int    concat()
**       void   convertNetworkTime2Integer()
**       void   setSerials()
**       void   setupNT-Converter()
**       int    convertNT2Integer()
**
** NOTES :
**      The naming of the prefixes of the variables and functions results from the variable type or
**       the variable type of the return value.
**       "c" stands for "char", an "s" for "short",
**       "v" for "void", a "u" for "unsigned" and
**       "x" for all non-standard types
**
** AUTHOR :            Tjark Ziehm        START DATE :    06 Juli 2024
** Copyright&Licence:  Tjark Ziehm, 2024
** Convention:         <major>.<minor>.<patch>
** CHANGES :
**            v.0.0.1 add and test functions to read&seperate the char elements in array Networktime
**            v.1.0.0 add the converter and the concationation function to get as result integers
**            v.1.1.0 seperate the code to implementable feature -> convertNetworkTime2Integer(); from #include "NetworkTimeConverter.hpp"
**
*********************************************************************************************H*/
#include <Arduino.h>
#include <iostream> // std::cout
#include <string>   // std::string, std::stoi
#include "features/global_variables.hpp"

using namespace std;

int searchcharpos = 0;  // To determine where is the + in +CCLK in the char array
int pointingfinger = 0; // index of the array
char foundchar[6];      // small buffer that works as a shift register to hold only the command for comparison
char serialdata[256];   // Array to store the chars before parsing

char rtcy1[3];  // Current year     Format: yy\0
char rtcm1[3];  // Current month    Format: mm\0
char rtcd1[3];  // Current day      Format: dd\0
char rtch1[3];  // Current hour     Format: hh\0
char rtcmm1[3]; // Current minute   Format: mm\0
char rtcs1[3];  // Current second   Format: ss\0
char rtc_tz[3]; // time zone (indicates the difference, expressed in quarters of an hour, between the local time and GMT;In an range of -96 ~ +96). +8 TZ = +2 hours

int actual_local_hours;
int actual_local_minutes;
int actual_local_seconds;
int actual_local_year;
int actual_local_month;
int actual_local_day;

int counter = 0;
int ready2read = 0;

// concatinating to array elements of on set ( hour, minutes, etc )
int concat(int a, int b)
{
    // Convert both the integers to string
    string s1 = to_string(a);
    string s2 = to_string(b);

    // Concatenate both strings
    string s = s1 + s2;

    // Convert the concatenated string
    // to integer
    int c = stoi(s);

    // return the formed integer
    return c;
}

// find the position of the +CCLK in the Serial Output
void checkPositionCCLK()
{
}

void parseData()
{
}

void writeTime_Date2glob_Variables(int hours, int minutes, int seconds, int year, int month, int day)
{
    Serial.println("writeTime_Date2glob_Variables");
    NETWORK_HOUR = hours;
    NETWORK_MINUTE = minutes;
    NETWORK_SECOND = seconds;
    NETWORK_YEAR = year;
    NETWORK_MONTH = month;
    NETWORK_DAY = day;
    Serial.println("-DONE-writeTime_Date2glob_Variables-DONE-");
}

void sendSerialCCLK()
{
    Serial1.print("AT+CCLK?"); // ask for the time
}

void storeSerialData2Arrays()
{
    Serial.println("storeSerialData2Arrays");
    rtcy1[0] = serialdata[searchcharpos + 6]; // getting first char with its offset
    rtcy1[1] = serialdata[searchcharpos + 7];
    rtcy1[2] = '\0';
    // delay(100);
    rtcm1[0] = serialdata[searchcharpos + 9];
    rtcm1[1] = serialdata[searchcharpos + 10];
    rtcm1[2] = '\0';
    // delay(100);
    rtcd1[0] = serialdata[searchcharpos + 12];
    rtcd1[1] = serialdata[searchcharpos + 13];
    rtcd1[2] = '\0';
    // delay(100);
    rtch1[0] = serialdata[searchcharpos + 15];
    rtch1[1] = serialdata[searchcharpos + 16];
    rtch1[2] = '\0';
    // delay(100);
    rtcmm1[0] = serialdata[searchcharpos + 18];
    rtcmm1[1] = serialdata[searchcharpos + 19];
    rtcmm1[2] = '\0';
    // delay(100);
    rtcs1[0] = serialdata[searchcharpos + 21];
    rtcs1[1] = serialdata[searchcharpos + 22];
    rtcs1[2] = '\0';
    // delay(100);
    rtc_tz[0] = serialdata[searchcharpos + 24];
    rtc_tz[1] = serialdata[searchcharpos + 25];
    rtc_tz[2] = '\0';
    Serial.println("-DONE-storeSerialData2Arrays-DONE-");
}

String result;

// take data from serial and store in array
void getDataFromSerial()
{
    sendSerialCCLK();
    if (Serial1.available() > 0)
    {

        foundchar[0] = foundchar[1];
        foundchar[1] = foundchar[2];
        foundchar[2] = foundchar[3];
        foundchar[3] = foundchar[4];
        foundchar[4] = foundchar[5];
        foundchar[5] = Serial1.read();

        if (foundchar[0] == '+' && foundchar[1] == 'C' && foundchar[2] == 'C' && foundchar[3] == 'L' && foundchar[4] == 'K' && foundchar[5] == ':')
        {
            Serial.println("+CCLK found in array. set pointfinger to 0");
            pointingfinger = 0;
            ready2read = 1;
        }
        /*if (ready2read == 1)
        {
            // move the foundchar array elements from serial till \n is at place 0
            while (foundchar[0] != '\n')
            {
                // Serial.println("check foundchar if is not new line");
                if (Serial1.available() > 0)
                {
                    Serial.println("foundshare[0] is not n");
                    serialdata[pointingfinger] = foundchar[0];
                    serialdata[pointingfinger + 1] = 0;
                    pointingfinger++;
                    foundchar[0] = foundchar[1];
                    foundchar[1] = foundchar[2];
                    foundchar[2] = foundchar[3];
                    foundchar[3] = foundchar[4];
                    foundchar[4] = foundchar[5];
                    foundchar[5] = Serial1.read();
                }
            }
        }*/
        Serial.println("end shift  array");
        storeSerialData2Arrays();
    }
    else
    {
        Serial.println("No Data avaible");
    }
}

void readFromArray2TimeVariables()
{
    Serial.println("readFromArray2TimeVariables");
    //------Time Shift--------
    char timezone_part1 = rtc_tz[0];
    char timezone_part2 = rtc_tz[1];
    int i_timezone_part1 = (int)timezone_part1 - 48;
    int i_timezone_part2 = (int)timezone_part2 - 48;
    int concatinatedTimeZone = concat(i_timezone_part1, i_timezone_part2); //! find more precisly name for result
    int hours2add = concatinatedTimeZone / 4;                              // has to be divided by 4 to find difference  between the local time and GMT
    // Serial.println(hours2add);

    // create integer representing the hours from char rtc_tz[]
    char hours_part1 = rtch1[0];
    char hours_part2 = rtch1[1];
    int i_hours_part1 = (int)hours_part1 - 48;
    int i_hours_part2 = (int)hours_part2 - 48;
    int i_concatinated_hours = concat(i_hours_part1, i_hours_part2);
    actual_local_hours = i_concatinated_hours + hours2add;

    //-----------------------
    char minutes_part1 = rtcmm1[0];
    char minutes_part2 = rtcmm1[1];
    int i_minutes_part1 = (int)minutes_part1 - 48;
    int i_minutes_part2 = (int)minutes_part2 - 48;
    int i_concatinated_minutes = concat(i_minutes_part1, i_minutes_part2);
    actual_local_minutes = i_concatinated_minutes;

    //-----------------------
    char seconds_part1 = rtcs1[0];
    char seconds_part2 = rtcs1[1];
    int i_seconds_part1 = (int)seconds_part1 - 48;
    int i_seconds_part2 = (int)seconds_part2 - 48;
    int i_concatinated_seconds = concat(i_seconds_part1, i_seconds_part2);
    actual_local_seconds = i_concatinated_seconds;

    //-----------------------
    char year_part1 = rtcy1[0];
    char year_part2 = rtcy1[1];
    int i_year_part1 = (int)year_part1 - 48;
    int i_year_part2 = (int)year_part2 - 48;
    int i_concatinated_year = concat(i_year_part1, i_year_part2);
    actual_local_year = i_concatinated_year;

    //-----------------------
    char month_part1 = rtcm1[0];
    char month_part2 = rtcm1[1];
    int i_month_part1 = (int)month_part1 - 48;
    int i_month_part2 = (int)month_part2 - 48;
    int i_concatinated_month = concat(i_month_part1, i_month_part2);
    actual_local_month = i_concatinated_month;

    //-----------------------
    char day_part1 = rtcd1[0];
    char day_part2 = rtcd1[1];
    int i_day_part1 = (int)day_part1 - 48;
    int i_day_part2 = (int)day_part2 - 48;
    int i_concatinated_day = concat(i_day_part1, i_day_part2);
    actual_local_day = i_concatinated_day;
    Serial.println("-DONE-readFromArray2TimeVariables-DONE-");
}

void printFinalResult()
{
    Serial.printf("Local Time: %d:%d:%d", actual_local_hours, actual_local_minutes, actual_local_seconds);
    Serial.println("");
    Serial.printf("Local Date: year: %d month: %d day: %d", actual_local_year, actual_local_month, actual_local_day);
    Serial.println("");
}

// Main-Function of the Feature
void convertNetworkTime2Integer()
{

    getDataFromSerial();
    storeSerialData2Arrays();
    readFromArray2TimeVariables();
    printFinalResult();
    // writeTime_Date2glob_Variables(actual_local_hours, actual_local_minutes, actual_local_seconds, actual_local_year, actual_local_month, actual_local_day);
};
