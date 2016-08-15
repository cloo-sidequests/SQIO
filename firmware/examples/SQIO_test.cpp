#include <sqio.h>

SQIO sqio;

void setup() {
  //Serial.begin(9600);
  sqio.setMode(D0, OUTPUT);
  sqio.setMode(D9, OUTPUT);
  sqio.setMode(D10, OUTPUT);
  sqio.setMode(D11, OUTPUT);
  sqio.setMode(D4, INPUT);
}

void loop() {
  sqio.update();
  if (sqio.read(D4) == LOW) {

    sqio.set(D0, HIGH, 1000);
    sqio.set(D11, HIGH, 1000);
    sqio.set(D10, HIGH, 1000);
    sqio.set(D9, HIGH, 1000);
  }
  Serial.println(sqio.read(A0, true, true));
}
