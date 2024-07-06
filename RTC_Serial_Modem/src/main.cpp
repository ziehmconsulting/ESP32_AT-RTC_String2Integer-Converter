#include <Arduino.h>
const long interval = 1000;
static long currentMillis;

byte searchcharpos = 0;  //To determine where is the + in +CCLK in the char array

char serialdata[256];  //Array to store the chars before parsing

char rtcy1[3];   //Current year  Format: yy\0
char rtcm1[3];   //Current month  Format: mm\0
char rtcd1[3];   //Current day  Format: dd\0
char rtch1[3];   //Current hour  Format: hh\0char rtcmm1[3];  //Current minute  Format: mm\0
char rtcmm1[3];  //!formater for rtch1 ??
char rtcs1[3];   //Current second  Format: ss\0

byte pointingfinger = 0;  //index of the array
char enabledtime = 0;  //this variable is set to 1 every second and set to 0 after parsing is done
char readenable = 0;  //this is set to 1 when is finished reading from serial
char foundchar[6];  //small buffer that works as a shift register to hold only the command for comparison

/**********************************************************************************************************************************/
// Modem Serial Pins
#define RX_PIN 4        // ESP32 RX pin
#define TX_PIN 2        // ESP32 TX pin
#define BAUD_RATE 9600  // Baud rate for ESP32 UART communication
#define GSM_RESET 19
#define GSM_ENABLE 18

void setSerials() {
  Serial.begin(9600);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
}

void setup() {
  setSerials();
  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);
  pinMode(GSM_ENABLE, OUTPUT);
  digitalWrite(GSM_ENABLE, HIGH); /* Turn ON output*/
  delay(500);
  Serial1.print("ATE0");  //Disable echo
}

void loop() {
  if (millis() - currentMillis >= interval)  //This is done every second
  {
    Serial.println("***********************************");
    Serial1.println("AT+CCLK?");
    Serial.println("AT+CCLK?");
    Serial.println("Printing Serial-Data Information");
    Serial.println("***********************************");

    int SerialDataPositions = 26;
    for (int i = 0; i <= SerialDataPositions;i++) {
      Serial.print(serialdata[i]);
    }
    Serial.println("");
    Serial.println("***********************************");
    //Serial1.print("AT+CCLK?\r"); //ask for the time
    //delay(50);
    //Serial.println(millis());

    Serial.println("YEAR: ");
    Serial.print(rtcy1[0]);
    Serial.println(rtcy1[1]);
    Serial.println("MONTH: ");
    Serial.print(rtcm1[0]);
    Serial.println(rtcm1[1]);
    Serial.println("DAY: ");
    Serial.print(rtcd1[0]);
    Serial.println(rtcd1[1]);
    Serial.println("HOUR: ");
    Serial.print(rtch1[0]);
    Serial.println(rtch1[1]);
    Serial.println("MINUTE: ");
    Serial.print(rtcmm1[0]);
    Serial.println(rtcmm1[1]);
    Serial.println("SECOND: ");
    Serial.print(rtcs1[0]);
    Serial.println(rtcs1[1]);
    /*Serial.println("Shift: ");
    Serial.print(rtcs1[0]);
    Serial.println(rtcs1[1]);
    */
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
        Serial.println("Network-Time found in Serial-Ouput");
        Serial.println("____________________________________");
        readenable = 1;
        pointingfinger = 0;
      }

      if (readenable == 1) {
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
        readenable = 0;
        //pointingfinger = 0;
        enabledtime = 0;
        rtcy1[0] = serialdata[searchcharpos + 6];  //getting first char with its offset
        rtcy1[1] = serialdata[searchcharpos + 7];
        rtcy1[2] = '\0';
        rtcm1[0] = serialdata[searchcharpos + 9];
        rtcm1[1] = serialdata[searchcharpos + 10];
        rtcm1[2] = '\0';
        rtcd1[0] = serialdata[searchcharpos + 12];
        rtcd1[1] = serialdata[searchcharpos + 13];
        rtcd1[2] = '\0';
        rtch1[0] = serialdata[searchcharpos + 15];
        rtch1[1] = serialdata[searchcharpos + 16];
        rtch1[2] = '\0';
        rtcmm1[0] = serialdata[searchcharpos + 18];
        rtcmm1[1] = serialdata[searchcharpos + 19];
        rtcmm1[2] = '\0';
        rtcs1[0] = serialdata[searchcharpos + 21];
        rtcs1[1] = serialdata[searchcharpos + 22];
        rtcs1[2] = '\0';
      }
    }
  }
}
