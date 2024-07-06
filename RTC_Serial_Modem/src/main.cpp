#include <Arduino.h>
#include <iostream> // std::cout
#include <string>   // std::string, std::stoi

using namespace std;

const long interval = 1000;
static long currentMillis;

int searchcharpos = 0; // To determine where is the + in +CCLK in the char array

char serialdata[256]; // Array to store the chars before parsing

char rtcy1[3];  // Current year  Format: yy\0
char rtcm1[3];  // Current month  Format: mm\0
char rtcd1[3];  // Current day  Format: dd\0
char rtch1[3];  // Current hour  Format: hh\0char rtcmm1[3];  //Current minute  Format: mm\0
char rtcmm1[3]; //! formater for rtch1 ??
char rtcs1[3];  // Current second  Format: ss\0
char rtc_tz[3]; // time zone (indicates the difference, expressed in quarters of an hour, between the local time and GMT;In an range of -96 ~ +96). +8 TZ = +2 hours

int pointingfinger = 0; // index of the array
char enabledtime = 0;   // this variable is set to 1 every second and set to 0 after parsing is done
char readenable = 0;    // this is set to 1 when is finished reading from serial
char foundchar[6];      // small buffer that works as a shift register to hold only the command for comparison

/**********************************************************************************************************************************/
// Modem Serial Pins
#define RX_PIN 4       // ESP32 RX pin
#define TX_PIN 2       // ESP32 TX pin
#define BAUD_RATE 9600 // Baud rate for ESP32 UART communication
#define GSM_RESET 19
#define GSM_ENABLE 18

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

void setSerials()
{
  Serial.begin(9600);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
}

void setup()
{
  setSerials();
  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);
  pinMode(GSM_ENABLE, OUTPUT);
  digitalWrite(GSM_ENABLE, HIGH); /* Turn ON output*/
  delay(500);
  Serial1.print("ATE0"); // Disable echo
}

void loop()
{
  if (millis() - currentMillis >= interval) // This is done every second
  {
    Serial.println("***********************************");
    Serial1.println("AT+CCLK?");
    Serial.println("AT+CCLK?");
    Serial.println("Printing Serial-Data Information");
    Serial.println("***********************************");

    int SerialDataPositions = 26;
    for (int i = 0; i <= SerialDataPositions; i++)
    {
      Serial.print(serialdata[i]);
    }
    Serial.println("");
    Serial.println("***********************************");
    // Serial1.print("AT+CCLK?\r"); //ask for the time
    // delay(50);
    // Serial.println(millis());
    //-----------------------
    Serial.println("YEAR: ");
    Serial.print(rtcy1[0]);
    Serial.println(rtcy1[1]);
    //-----------------------
    Serial.println("MONTH: ");
    Serial.print(rtcm1[0]);
    Serial.println(rtcm1[1]);
    //-----------------------
    Serial.println("DAY: ");
    Serial.print(rtcd1[0]);
    Serial.println(rtcd1[1]);
    //-----------------------
    Serial.println("HOUR: ");
    Serial.print(rtch1[0]);
    Serial.println(rtch1[1]);
    //-----------------------
    Serial.println("MINUTE: ");
    Serial.print(rtcmm1[0]);
    Serial.println(rtcmm1[1]);
    //-----------------------
    Serial.println("SECOND: ");
    Serial.print(rtcs1[0]);
    Serial.println(rtcs1[1]);
    //------time zone--------
    Serial.println("TimeZone: ");
    Serial.print(rtc_tz[0]);
    Serial.println(rtc_tz[1]);
    Serial.println("Hours to add:");
    char timezone_part1 = rtc_tz[0];
    int i_timezone_part1 = (int)timezone_part1 - 48;
    char timezone_part2 = rtc_tz[1];
    int i_timezone_part2 = (int)timezone_part2 - 48;
    int concatinatedTimeZone = concat(i_timezone_part1, i_timezone_part2);     //! find more precisly name for result
    int hours2add = concatinatedTimeZone/4;            // has to be divided by 4 to find difference  between the local time and GMT
    Serial.println(hours2add);

    Serial.println("actual HOURS:");
    //create integer representing the hours from char rtc_tz[]
    char hours_part1 = rtch1[0];
    char hours_part2 = rtch1[1];
    int i_hours_part1 = (int)hours_part1 - 48;
    int i_hours_part2 = (int)hours_part2 - 48;
    int i_concatinated_hours = concat(i_hours_part1, i_hours_part2);
    int actual_local_hours = i_concatinated_hours +hours2add;
    //Serial.println(actual_local_hours);
    //-----------------------
    Serial.println("actual MINUTE: ");
    char minutes_part1 = rtcmm1[0];
    char minutes_part2 = rtcmm1[1];
    int i_minutes_part1 = (int)minutes_part1 - 48;
    int i_minutes_part2 = (int)minutes_part2 - 48;
    int i_concatinated_minutes = concat(i_minutes_part1, i_minutes_part2);
    int actual_local_minutes = i_concatinated_minutes;
    //-----------------------
    Serial.println("actual SECOND: ");
    Serial.print(rtcs1[0]);
    Serial.println(rtcs1[1]);
    char seconds_part1 = rtcs1[0];
    char seconds_part2 = rtcs1[1];
    int i_seconds_part1 = (int)seconds_part1 - 48;
    int i_seconds_part2 = (int)seconds_part2 - 48;
    int i_concatinated_seconds = concat(i_seconds_part1, i_seconds_part2);
    int actual_local_seconds = i_concatinated_seconds;
    Serial.printf("Local Time: %d:%d:%d",actual_local_hours,actual_local_minutes,actual_local_seconds);
    Serial.println("");
    //------value to add to hours--------
    // char first_char = rtc_tz[0];
    // std::string str_1 = first_char[0];
    // char second_char = rtc_tz[1];
    // int number1 = std::stoi(first_char,nullptr,0);
    // int number2 = std::stoi(second_char);
    /*Serial.println("Time to add: ");
    const char* ch = rtc_tz[3];
    std::string str_hex = ch;
    int i_hex = std::stoi (str_hex,nullptr,16);
    Serial.println(i_hex);*/
    enabledtime = 1;
    currentMillis = millis();
  }

  if (enabledtime == 1)
  {
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
        Serial.println("Network-Time found in Serial-Ouput");
        Serial.println("____________________________________");
        readenable = 1;
        pointingfinger = 0;
      }

      if (readenable == 1)
      {
        while (foundchar[0] != '\n')
        {
          if (Serial1.available() > 0)
          {
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
        readenable = 0;
        // pointingfinger = 0;
        enabledtime = 0;
        rtcy1[0] = serialdata[searchcharpos + 6]; // getting first char with its offset
        rtcy1[1] = serialdata[searchcharpos + 7];
        rtcy1[2] = '\0';
        delay(100);
        rtcm1[0] = serialdata[searchcharpos + 9];
        rtcm1[1] = serialdata[searchcharpos + 10];
        rtcm1[2] = '\0';
        delay(100);
        rtcd1[0] = serialdata[searchcharpos + 12];
        rtcd1[1] = serialdata[searchcharpos + 13];
        rtcd1[2] = '\0';
        delay(100);
        rtch1[0] = serialdata[searchcharpos + 15];
        rtch1[1] = serialdata[searchcharpos + 16];
        rtch1[2] = '\0';
        delay(100);
        rtcmm1[0] = serialdata[searchcharpos + 18];
        rtcmm1[1] = serialdata[searchcharpos + 19];
        rtcmm1[2] = '\0';
        delay(100);
        rtcs1[0] = serialdata[searchcharpos + 21];
        rtcs1[1] = serialdata[searchcharpos + 22];
        rtcs1[2] = '\0';
        delay(100);
        rtc_tz[0] = serialdata[searchcharpos + 24];
        rtc_tz[1] = serialdata[searchcharpos + 25];
        rtc_tz[2] = '\0';
        delay(100);
      }
    }
  }
}
