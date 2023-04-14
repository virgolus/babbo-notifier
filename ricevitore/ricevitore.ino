
#include <SoftwareSerial.h>

const byte rxPin = 11;
const byte txPin = 10;
char msg;
bool alarm = false;
int motor_pin = 2;
long last_alarm = 0;

SoftwareSerial ch12_serial(txPin, rxPin); //TX, RX

void setup() {
  Serial.begin(9600);             // Serial port to computer
  ch12_serial.begin(9600);               // Serial port to HC12
  pinMode(motor_pin, OUTPUT);           // set pin to input
}

void loop() {
  while (ch12_serial.available()) {        // If HC-12 has data
    msg = ch12_serial.read();
    Serial.write(msg);      // Send the data to Serial monitor
    alarm = true;
    last_alarm = millis();
  }

  if (alarm && ((millis() - 1000) < last_alarm)) {
    digitalWrite(motor_pin, HIGH);       // turn on pullup resistors
  } else {
    digitalWrite(motor_pin, LOW);       // turn on pullup resistors
    alarm = false;
  }
/*


  if (alarm && ((millis() - 3000) < last_alarm)) {
    digitalWrite(motor_pin, HIGH);       // turn on pullup resistors
  } else {
    digitalWrite(motor_pin, LOW);       // turn on pullup resistors
    alarm = false;
  }
  */
}
