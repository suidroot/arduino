/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 9.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * NEC: 61A0F00F tv power
 * NEC: 48B710EF auto sleep
 * NEC: 48B7609F  auto
 * NEC: 77E1505B apple tv up
 */

#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  if (Serial.read() != -1) {
    for (int i = 0; i < 3; i++) {
      irsend.sendNEC(0x77E1505B, 32); // Sony TV power code
      Serial.println("send");
      delay(4000);

    }
  }
}
