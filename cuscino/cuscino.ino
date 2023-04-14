// Cuscino
// autor Ezio Vergine

#include <SoftwareSerial.h>

long last_standup = 0;
long last_sitdown = 0;

long time_msg = 3000;
long time_wait = 3000;

const byte rxPin = 11;
const byte txPin = 10;

bool send_alarm = false;
bool wait_after_alarm = false;
bool wait_before_alarm = false;

SoftwareSerial ch12_serial(txPin, rxPin); //TX, RX

void setup() {
  Serial.begin(9600);
  ch12_serial.begin(9600);

  // initial wait
  delay(2000);

  // check if antenna is started
  if(ch12_serial.isListening()) {
    Serial.println("START");
  } else {
    Serial.println("antenna doesn't work");
  }

  // Led blink
  TCCR1A = 0;
  TCCR1B = 0;
  bitSet(TCCR1B, CS12);  // 256 prescaler
  bitSet(TIMSK1, TOIE1); // timer overflow interrupt
  pinMode(LED_BUILTIN, OUTPUT);
}

// Led blink
ISR(TIMER1_OVF_vect) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void loop() {
 
  // READ sensore. Valori sopra il 600 indicano pressione sul cuscino
  int sensore_val = analogRead(A11);

  // Se il gorilla si alza
  if (sensore_val < 600) {
    if (!send_alarm && !wait_after_alarm & wait_before_alarm) {
      last_standup = millis();
    }

    // se allarme anche dopo il tempo di attesa
    if (((millis() - time_wait) < last_standup)) {
      Serial.println("Wait IF alarm");
      wait_before_alarm = false;
    } else if ((millis() - time_msg - time_wait) < last_standup) {
      // dopo il tempo di attesa, per i primi 10 secondi manda l'infinito
      Serial.println("send alarm");
      send_alarm = true;
      wait_after_alarm = false;
      wait_before_alarm = false;
    } else {
      // se passano 10 secondi basta allarme e va in standby
      Serial.println("standby");
      delay(1000);
      send_alarm = false;
      wait_after_alarm = true;
      wait_before_alarm = false;
    }
  } else {
    send_alarm = false;
    wait_after_alarm = false;
    wait_before_alarm = true;
    last_sitdown = millis();
  }

  // SEND alarm
  if (send_alarm) {//Read from HC-12 and send to serial monitor
    ch12_serial.println("alarm");
  }

  delay(100);
}