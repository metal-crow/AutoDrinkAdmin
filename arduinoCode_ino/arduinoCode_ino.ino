#include <OneWire.h>
#define IBUTTON_ID_SIZE 8

OneWire ds(4); // One Wire for iButton on pin 4

unsigned long int time;             // the current time unit
unsigned long timeOfLastPulseCoins; // the time of the last pulse from the coin reader
volatile int coinsValue;            // the value for the coin reader
int coinsChange;                    // 1 if the coin has had input, 0 otherwise
String result;                      // result from the iButton reader
byte ibutton_id[IBUTTON_ID_SIZE];   // data to hold the iButton ID

void setup()
{
  timeOfLastPulseCoins = millis();
  coinsChange = 0;
  coinsValue = 0;
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  delay(500);
  digitalWrite(9, LOW);
  attachInterrupt(0, coinInserted, RISING);
}

/*
 * interupt method for when the coin reader pulses
 */
void coinInserted() {
  coinsValue++;
  coinsChange = 1;
  timeOfLastPulseCoins = millis();
}

void loop()
{
  
  if (Serial.available()) { // if the computer writes to the arduino
    Serial.println(Serial.read());
  }
  
  // clears & initialzes the iButton buffer
  for (int i = 0 ; i < IBUTTON_ID_SIZE ; i++) {
    ibutton_id[i] = 0;
  }
  
  // reads the iButton ID, if present
  if (ds.search(ibutton_id)) {
    for (int i = IBUTTON_ID_SIZE - 1 ; i >= 0 ; i--) {
      if (ibutton_id[i] < 0x10)
        result += '0';
      result += String(ibutton_id[i], HEX);
    }
    Serial.print("i:" + result);
    delay(1000);
    result = '';
  } else {
    ds.reset_search();
  }
  
  if (coinsChange == 1 && millis() - timeOfLastPulseCoins > 1000) {
    coinsChange = 0;
    Serial.print("m:" + String(coinsValue));
    coinsValue = 0;
  }
  
}