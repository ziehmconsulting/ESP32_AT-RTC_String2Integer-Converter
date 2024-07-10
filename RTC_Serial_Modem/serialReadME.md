#define RX0 4
#define TX0 2

#define GSM_RESET 19
#define GSM_ENABLE 18
#define BAUD 9600

void setup() {
  // initialize both serial ports:
  Serial.begin(BAUD);
  Serial2.begin(BAUD, SERIAL_8N1, RX0, TX0);
  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);
  pinMode(GSM_ENABLE, OUTPUT);
  digitalWrite(GSM_ENABLE, HIGH); /* Turn ON output*/
}

void loop() {

  if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }
  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }
}
