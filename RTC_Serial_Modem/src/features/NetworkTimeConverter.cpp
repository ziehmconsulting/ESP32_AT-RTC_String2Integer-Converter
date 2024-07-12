/*H********************************************************************************************
** FILENAME :        NetworkTimeConverter.cpp
** Version  :        1.0.0
** DESCRIPTION :
**       Converter from AT-Networktime as a String to integer. Used AT+Command is "AT+CCLK?"
**       (not implemented yet) Writing integers to global Varibles
**
** PUBLIC FUNCTIONS :
**      uint8_t concatinateTime(uint8_t a, uint8_t b)
**      void printSerialTime()  -> Debug option
**          Debug_Option:     bool print_Time = false; //print time generate in the function convertTime2Int
**      void printArrayTime()
**      void writeTime_Date2glob_Variables(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t year, uint8_t month, uint8_t day)
**      void printGlobalTime()
**        Debug_Option: bool print_global_Time = true;
**      void convertTime2Int()
**      void getNetworkTime()  -> Main Function
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
**            v.1.x.y ....
*********************************************************************************************H*/
#include <Arduino.h>
#include <iostream>  // std::cout
#include <string>    // std::string, std::stoi
#include "NetworkTimeConverter.hpp"
#include "global_variables.hpp"

using namespace std;

//Debug-Option
bool print_Time = true; //print time generate in the function convertTime2Int
bool print_global_Time = true;

//using unint8_t or unsigned char instead of byte with namespace std
//byte searchcharpos = 0;  //To determine where is the + in +CCLK in the char array
uint8_t searchcharpos = 0;  //To determine where is the + in +CCLK in the char array

char serialdata[256];  //Array to store the chars before parsing
char array_year[3];//Current year  Format: yy\0
char array_month[3];   //Current month  Format: mm\0
char array_day[3];   //Current day  Format: dd\0
char array_hour[3];   //Current hour  Format: hh\0
char array_minute[3];  //Current minute  Format: mm\0
char array_second[3];   //Current second  Format: ss\0
char rtc_tz[3];  // time zone (indicates the difference, expressed in quarters of an hour, between the local time and GMT;In an range of -96 ~ +96). +8 TZ = +2 hours


//byte pointingfinger = 0;  //index of the array
uint8_t pointingfinger = 0;  //index of the array

char enabledtime = 0;  //this variable is set to 1 every second and set to 0 after parsing is done

char ready2read = 0;  //this is set to 1 when is finished reading from serial

char foundchar[6];  //small buffer that works as a shift register to hold only the command for comparison

const long interval = 1000;
static long currentMillis;

//*********************************************************************************************
uint8_t concatinateTime(uint8_t a, uint8_t b) {
  // Convert both the integers to string
  string s1 = to_string(a);
  string s2 = to_string(b);

  // Concatenate both strings
  string s = s1 + s2;

  // Convert the concatenated string
  // to integer
  uint8_t c = stoi(s);

  // return the formed integer
  return c;
}

//*********************************************************************************************
//Debug-Feature
//print the actual stored Serial Data
// her till to position 0-26 ( is related to the original output of the modem)
//may differ from the modem depending on the model
void printSerialTime() {
  Serial.println("Print serialdata:");
  Serial.print(serialdata[searchcharpos]);
  Serial.print(serialdata[searchcharpos + 1]);
  Serial.print(serialdata[searchcharpos + 2]);
  Serial.print(serialdata[searchcharpos + 3]);
  Serial.print(serialdata[searchcharpos + 4]);
  Serial.print(serialdata[searchcharpos + 5]);
  Serial.print(serialdata[searchcharpos + 6]);
  Serial.print(serialdata[searchcharpos + 7]);
  Serial.print(serialdata[searchcharpos + 8]);
  Serial.print(serialdata[searchcharpos + 9]);
  Serial.print(serialdata[searchcharpos + 10]);
  Serial.print(serialdata[searchcharpos + 11]);
  Serial.print(serialdata[searchcharpos + 12]);
  Serial.print(serialdata[searchcharpos + 13]);
  Serial.print(serialdata[searchcharpos + 14]);
  Serial.print(serialdata[searchcharpos + 15]);
  Serial.print(serialdata[searchcharpos + 16]);
  Serial.print(serialdata[searchcharpos + 17]);
  Serial.print(serialdata[searchcharpos + 18]);
  Serial.print(serialdata[searchcharpos + 19]);
  Serial.print(serialdata[searchcharpos + 20]);
  Serial.print(serialdata[searchcharpos + 21]);
  Serial.print(serialdata[searchcharpos + 22]);
  Serial.print(serialdata[searchcharpos + 23]);
  Serial.print(serialdata[searchcharpos + 24]);
  Serial.print(serialdata[searchcharpos + 25]);
  Serial.print(serialdata[searchcharpos + 26]);
  //Serial.print(serialdata[searchcharpos + 27]);
  //Serial.print(serialdata[searchcharpos + 28]);
  //Serial.print(serialdata[searchcharpos + 29]);
  //Serial.print(serialdata[searchcharpos + 30]);
  Serial.println("");
  Serial.println("==============================================");
  //Serial1.print("AT+CCLK?\r"); //ask for the time
  //delay(50);
}

//*********************************************************************************************
//DEbug-Feature
void printArrayTime() {
  Serial.println("YEAR: ");
  Serial.print(array_year[0]);
  Serial.println(array_year[1]);
  Serial.println("MONTH: ");
  Serial.print(array_month[0]);
  Serial.println(array_month[1]);
  Serial.println("DAY: ");
  Serial.print(array_day[0]);
  Serial.println(array_day[1]);
  Serial.println("HOUR: ");
  Serial.print(array_hour[0]);
  Serial.println(array_hour[1]);
  Serial.println("MINUTE: ");
  Serial.print(array_minute[0]);
  Serial.println(array_minute[1]);
  Serial.println("SECOND: ");
  Serial.print(array_second[0]);
  Serial.println(array_second[1]);
}

//*********************************************************************************************
// Store converted result as integer to global variables
// used in function void convertTime2Int()
void writeTime_Date2glob_Variables(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t year, uint8_t month, uint8_t day) {
  NETWORK_HOUR = hours;
  NETWORK_MINUTE = minutes;
  NETWORK_SECOND = seconds;
  NETWORK_YEAR = year;
  NETWORK_MONTH = month;
  NETWORK_DAY = day;
  NETWORK_TIME_GLOBAL = true;  // global trigger to interupt in the mainloop
}

//*********************************************************************************************
//Converting time from array to char to integer
void convertTime2Int() {
  //------Time Shift--------
  //Serial.println("TimeShift:");
  char timezone_part1 = rtc_tz[0];
  char timezone_part2 = rtc_tz[1];
  uint8_t i_timezone_part1 = (uint8_t)timezone_part1 - 48;
  uint8_t i_timezone_part2 = (uint8_t)timezone_part2 - 48;
  uint8_t concatinatedTimeZone = concatinateTime(i_timezone_part1, i_timezone_part2);  //! find more precisly name for result
  uint8_t hours2add = concatinatedTimeZone / 4;                                        // has to be divided by 4 to find difference  between the local time and GMT
  //Serial.println(hours2add);

  // create integer representing the hours from char rtc_tz[]
  char hours_part1 = array_hour[0];
  char hours_part2 = array_hour[1];
  uint8_t i_hours_part1 = (uint8_t)hours_part1 - 48;
  uint8_t i_hours_part2 = (uint8_t)hours_part2 - 48;
  uint8_t i_concatinated_hours = concatinateTime(i_hours_part1, i_hours_part2);
  uint8_t actual_local_hours = i_concatinated_hours + hours2add;

  //-----------------------
  char minutes_part1 = array_minute[0];
  char minutes_part2 = array_minute[1];
  uint8_t i_minutes_part1 = (uint8_t)minutes_part1 - 48;
  uint8_t i_minutes_part2 = (uint8_t)minutes_part2 - 48;
  uint8_t i_concatinated_minutes = concatinateTime(i_minutes_part1, i_minutes_part2);
  uint8_t actual_local_minutes = i_concatinated_minutes;

  //-----------------------
  char seconds_part1 = array_second[0];
  char seconds_part2 = array_second[1];
  uint8_t i_seconds_part1 = (uint8_t)seconds_part1 - 48;
  uint8_t i_seconds_part2 = (uint8_t)seconds_part2 - 48;
  uint8_t i_concatinated_seconds = concatinateTime(i_seconds_part1, i_seconds_part2);
  uint8_t actual_local_seconds = i_concatinated_seconds;

  //-----------------------
  char year_part1 = array_year[0];
  char year_part2 = array_year[1];
  uint8_t i_year_part1 = (uint8_t)year_part1 - 48;
  uint8_t i_year_part2 = (uint8_t)year_part2 - 48;
  uint8_t i_concatinated_year = concatinateTime(i_year_part1, i_year_part2);
  uint8_t actual_local_year = i_concatinated_year;

  //-----------------------
  char month_part1 = array_month[0];
  char month_part2 = array_month[1];
  uint8_t i_month_part1 = (uint8_t)month_part1 - 48;
  uint8_t i_month_part2 = (uint8_t)month_part2 - 48;
  uint8_t i_concatinated_month = concatinateTime(i_month_part1, i_month_part2);
  uint8_t actual_local_month = i_concatinated_month;

  //-----------------------
  char day_part1 = array_day[0];
  char day_part2 = array_day[1];
  uint8_t i_day_part1 = (uint8_t)day_part1 - 48;
  uint8_t i_day_part2 = (uint8_t)day_part2 - 48;
  uint8_t i_concatinated_day = concatinateTime(i_day_part1, i_day_part2);
  uint8_t actual_local_day = i_concatinated_day;

  writeTime_Date2glob_Variables(actual_local_hours, actual_local_minutes, actual_local_seconds, actual_local_year, actual_local_month, actual_local_day);

  if (print_Time == true) {
    Serial.println("***************************************************************");
    Serial.printf("Local Time: %d:%d:%d", actual_local_hours, actual_local_minutes, actual_local_seconds);
    Serial.println("");
    Serial.printf("Local Date: %d:%d:%d", actual_local_year, actual_local_month, actual_local_day);
    Serial.println("");
    Serial.println("***************************************************************");
  }
}

//*********************************************************************************************
// Debug-Feature
void printGlobalTime() {
  Serial.println("------------------------------------------");
  Serial.printf("Year: %d\r\n", NETWORK_YEAR);
  Serial.printf("MONTH: %d\r\n", NETWORK_MONTH);
  Serial.printf("DAY: %d\r\n", NETWORK_DAY);
  Serial.printf("Hour: %d\r\n", NETWORK_HOUR);
  Serial.printf("Minutes: %d\r\n", NETWORK_MINUTE);
  Serial.printf("Seconds: %d\r\n", NETWORK_SECOND);
  Serial.println("------------------------------------------");
}


bool getTime = true;

//*********************************************************************************************
// Main-function to read Network time from modem and make it avaible with global access  to store regular at rtc-module
void getNetworkTime() {
  //if (millis() - currentMillis >= interval)  //This is done every second
  if (getTime)  //This is done every call aslong as getTime is true
  {
    Serial.println("***********************************");
    //works
    Serial1.println("AT+CCLK?");
    //works
    //Serial1.println("AT+CCLK?\r\n");  // TODO: understand difference between both working commands
    Serial.println("Sent Command: AT+CCLK?");
    Serial.println("***********************************");
    //printSerialTime();
    //Serial.println("Millis: ");
    //Serial.println(millis());
    //Serial.println("***********************************");
    //printArrayTime();
    enabledtime = 1;
    currentMillis = millis();
  }

  if (enabledtime == 1) {
    if (Serial1.available() > 0) {
      foundchar[0] = foundchar[1];
      foundchar[1] = foundchar[2];
      foundchar[2] = foundchar[3];
      foundchar[3] = foundchar[4];
      foundchar[4] = foundchar[5];
      foundchar[5] = Serial1.read();

      if (foundchar[0] == '+' && foundchar[1] == 'C' && foundchar[2] == 'C' && foundchar[3] == 'L' && foundchar[4] == 'K' && foundchar[5] == ':') {
        ready2read = 1;
        pointingfinger = 0;
      }
      if (ready2read == 1) {
        while (foundchar[0] != '\n') {
          if (Serial1.available() > 0) {
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
        ready2read = 0;
        //pointingfinger = 0;
        enabledtime = 0;
        //variable x may differ from the modem depending on the modeland output $serialdata[searchcharpos + x]
        //add new Serial Data to Array for the year
        array_year[0] = serialdata[searchcharpos + 6];  // getting first char with its offset
        array_year[1] = serialdata[searchcharpos + 7];
        array_year[2] = '\0';
        //add new Serial Data to Array for the month
        array_month[0] = serialdata[searchcharpos + 9];
        array_month[1] = serialdata[searchcharpos + 10];
        array_month[2] = '\0';
        //add new Serial Data to Array for the day
        array_day[0] = serialdata[searchcharpos + 12];
        array_day[1] = serialdata[searchcharpos + 13];
        array_day[2] = '\0';
        //add new Serial Data to Array for the hours
        array_hour[0] = serialdata[searchcharpos + 15];
        array_hour[1] = serialdata[searchcharpos + 16];
        array_hour[2] = '\0';
        //add new Serial Data to Array for the minutes
        array_minute[0] = serialdata[searchcharpos + 18];
        array_minute[1] = serialdata[searchcharpos + 19];
        array_minute[2] = '\0';
        //add new Serial Data to Array for the seconds
        array_second[0] = serialdata[searchcharpos + 21];
        array_second[1] = serialdata[searchcharpos + 22];
        array_second[2] = '\0';
        //delay(100);
        //add new Serial Data to Array for the time zone
        rtc_tz[0] = serialdata[searchcharpos + 24];
        rtc_tz[1] = serialdata[searchcharpos + 25];
        rtc_tz[2] = '\0';
        //print the latest time ( Function for testing purpuse to get the char output)
        //printSerialTime();
        //printArrayTime();
        convertTime2Int();
        //delay(200);
        if (print_global_Time == true){
          printGlobalTime();
        }
      }
    }
  }
}
