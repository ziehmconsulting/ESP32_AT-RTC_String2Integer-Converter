/*H********************************************************************************************
** FILENAME :        main.cpp
** Version  :        2.0.0
** DESCRIPTION :
**       Converter from Networktime as a String to integer
**
** PUBLIC FUNCTIONS :
**       int    concat()
**       void setSerials()
**       void setupNT-Converter()
**       int convertNT2Integer()
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
**            v.2.0.0 add configuration feauter for DS3231 RTC (SCL:GPIO22; SDA:GPIO21)
*********************************************************************************************H*/
#include <Arduino.h>
#include <iostream> // std::cout
#include <string>   // std::string, std::stoi
#include "features/NetworkTimeConverter.hpp"
#include "features/global_variables.hpp"
#include "features/rtcConfigurator.hpp"

using namespace std;

TaskHandle_t Task0;
TaskHandle_t Task1;
/**********************************************************************************************************************************/
// Modem Serial Pins
#define RX_PIN 4       // ESP32 RX pin
#define TX_PIN 2       // ESP32 TX pin
#define BAUD_RATE 9600 // Baud rate for ESP32 UART communication
#define GSM_RESET 19
#define GSM_ENABLE 18

bool setRTCTime = false;
bool gotNetworkTime = false;
u_int8_t iterations = 10;

void setSerials()
{
  Serial.begin(BAUD_RATE);
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
  Serial.println("Serial configured");
  delay(500);
  Serial1.print("ATE0"); // Disable echo !!! Important to avoide overhead of regular calcluation
  //! TODO: add feedback "if ok" function
  delay(500);
  Serial1.print("ATE0");
  delay(3000);
  //! TODO: add  get time from network to RTC Module
  // Serial1.print("ATE0"); // Disable echo

  setupRTC();
  delay(500);
  /*
  xTaskCreatePinnedToCore(
      showNetworkTimeTask,       // Function that should be called
      "show time from variable", // Name of the task (for debugging)
      2048,                      // Stack size (bytes)
      NULL,                      // Parameter to pass
      1,                         // Task priority
      &Task0,                    // Task handle
      0                          // run on Core 1
  );

  xTaskCreatePinnedToCore(
      getNetworkTimeTask,      // Function that should be called
      "get time from Network", // Name of the task (for debugging)
      2048,                    // Stack size (bytes)
      NULL,                    // Parameter to pass
      1,                       // Task priority
      &Task1,                  // Task handle
      1                        // run on Core 1
  );
  delay(500);
  */
}

int counter = 0;
void loop()
{
  if (!gotNetworkTime)
  {
    for (;;)
    {
      getNetworkTime();

      if (NETWORK_TIME_GLOBAL != false)
      {
        Serial.println("got time from Modem");
        gotNetworkTime = true;
        // goto label;
        delay(1000);
        break;
      }
    }
  }

  if (!setRTCTime)
  {
    adjustRTC_Time(NETWORK_HOUR, NETWORK_MINUTE, NETWORK_SECOND, NETWORK_YEAR, NETWORK_MONTH, NETWORK_DAY);
    setRTCTime = true;
  }

  if (setRTCTime != false && gotNetworkTime != false)
  {
    delay(100);
    getTimeRTC();
    delay(1000);
  }
}
