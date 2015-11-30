/* 
 * Count from 1 to 9999 and display on Nixie Tube
 * Utilizes 595 Serial controller to set digit
 *
 */


int dataPin = 11; //(37 white)
int latchPin = 10; //(39 green)
int clockPin = 9; //(40 blue)

// 4 DIGIT NIXIE TUBE Digit selctor
int d1 = A0;
int d2 = A1;
int d3 = A2;
int d4 = A3;

int counter = 1;

void setup() {
  Serial.begin(9600);

  // 595 pin mode setup
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 

  // NIXIE Tube pin setup
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT); 

  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

}

void clearLEDs() {
  int reset = B11111111;
  shiftOut(dataPin, clockPin, MSBFIRST, reset);
}


void pickDigit(int x) {

  // Set refresh Delay
  int del = 5;
  
  switch(x)
  {
    case 1: digitalWrite(d1, LOW); delay(del); digitalWrite(d1, HIGH); break;
    case 2: digitalWrite(d2, LOW); delay(del); digitalWrite(d2, HIGH); break;
    case 3: digitalWrite(d3, LOW); delay(del); digitalWrite(d3, HIGH); break;
    default: digitalWrite(d4, LOW); delay(del); digitalWrite(d4, HIGH); break;
  }
}

void pickNumber(char value) {
  // Define Digit Sequence
  int numberal_1 = B00000110;
  int numberal_2 = B01011011;
  int numberal_3 = B01001111;
  int numberal_4 = B01100110;
  int numberal_5 = B01101101;
  int numberal_6 = B01111101;
  int numberal_7 = B00000111;
  int numberal_8 = B01111111;
  int numberal_9 = B01100111;
  int numberal_0 = B00111111;
  int reset_number = B00000000;

  digitalWrite(latchPin, LOW);

  // write binary data to 595 serial controller
  switch (value) {
    case 1: shiftOut(dataPin, clockPin, MSBFIRST, numberal_1); break;
    case 2: shiftOut(dataPin, clockPin, MSBFIRST, numberal_2); break;
    case 3: shiftOut(dataPin, clockPin, MSBFIRST, numberal_3); break;
    case 4: shiftOut(dataPin, clockPin, MSBFIRST, numberal_4); break;
    case 5: shiftOut(dataPin, clockPin, MSBFIRST, numberal_5); break;
    case 6: shiftOut(dataPin, clockPin, MSBFIRST, numberal_6); break;
    case 7: shiftOut(dataPin, clockPin, MSBFIRST, numberal_7); break;
    case 8: shiftOut(dataPin, clockPin, MSBFIRST, numberal_8); break;
    case 9: shiftOut(dataPin, clockPin, MSBFIRST, numberal_9); break;
    case 0: shiftOut(dataPin, clockPin, MSBFIRST, numberal_0); break;
    default: shiftOut(dataPin, clockPin, MSBFIRST, reset_number); break;
  }

  digitalWrite(latchPin,HIGH);

}

void loop() {
  
    //int number;
    int del = 55;
    int x=100;
    int n1, n2, n3, n4;

    for (int n=0; n<10000; n++) {

      // Separate number into 4 digits
      // n4 n3 n2 n1 
      //
      // Find and display 1st digit
      n1 = n%10;
      clearLEDs();
      pickNumber(n1); 
      pickDigit(1); 

      // Find and display 2nd digit
      n2 = (n/10)%10;
      clearLEDs();
      pickNumber(n2); 
      pickDigit(2);

      // Find and display 3rd digit
      n3 = (n/10/10)%10;
      clearLEDs();
      pickNumber(n3); 
      pickDigit(3); 

      // Find and display 4th digit
      n4 = ((n/10/10/10)%10);
      clearLEDs();
      pickNumber(n4); 
      pickDigit(4); 

      // Output debug information
      Serial.print(n4);
      Serial.print(" ");
      Serial.print(n3);
      Serial.print(" ");
      Serial.print(n2);
      Serial.print(" ");
      Serial.print(n1);
      Serial.print(" ");
      Serial.print(n);
      Serial.println("");
      //delay(400);
      delayMicroseconds(200);

      // reset digits
      digitalWrite(d1, HIGH);
      digitalWrite(d2, HIGH);
      digitalWrite(d3, HIGH);
      digitalWrite(d4, HIGH);
      
    }

}
