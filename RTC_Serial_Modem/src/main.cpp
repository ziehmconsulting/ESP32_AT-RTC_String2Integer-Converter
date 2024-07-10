/*H********************************************************************************************
** FILENAME :        main.cpp
** Version  :        1.0.1
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
**
*********************************************************************************************H*/
#include <Arduino.h>
#include <iostream> // std::cout
#include <string>   // std::string, std::stoi
#include "features/NetworkTimeConverter.hpp"
#include "features/global_variables.hpp"

using namespace std;

/**********************************************************************************************************************************/
// Modem Serial Pins
#define RX_PIN 4       // ESP32 RX pin
#define TX_PIN 2       // ESP32 TX pin
#define BAUD_RATE 9600 // Baud rate for ESP32 UART communication
#define GSM_RESET 19
#define GSM_ENABLE 18

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
  Serial1.print("ATE0\r");        // Disable echo
  // Serial1.print("ATE0"); // Disable echo
}

void loop()
{
  convertNetworkTime2Integer();
  Serial.println(NETWORK_SECOND);
}
