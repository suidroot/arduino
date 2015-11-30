#include <Keypad.h>

int DEBUG = 0;

// Define 595 control pins
int dataPin = 11; 
int latchPin = 10;
int clockPin = 9;

// 4 DIGIT NIXIE TUBE Digit selctor
int d1 = A0;
int d2 = A1;
int d3 = A2;
int d4 = A3;

// Lights
int red = A4;
int green = A5;

// Define Keypad input
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// ************

int counter = 1;
int activecode = 0000;

// Define Access Codes
const int codequanity = 3;
int secretcode[codequanity] = {1234,2600,5170};


void setup(){
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

  //turn on red light
  digitalWrite(red, HIGH);
}


void loop() {

  displaynumber(activecode);

  char key = keypad.getKey();

  if (key == '*') {
    counter = 1;
    activecode = 0000;
    Serial.print("RESET ");
    displayinput(counter, activecode, key);


  } else if (counter == 5 && key == '#') {
    checkcode(activecode);
    counter = 1;
    activecode = 0000;
  } else if (key == '#') {
    delay(1);
  } else if (key != NO_KEY) {
    if (counter <= 4) {
      activecode = updatecode(counter, key, activecode);

      displayinput(counter, activecode, key);
      counter++;
    } else {
      // reset after max digits
      counter = 1;
      Serial.print("TOOMANY ");
      displayinput(counter, activecode, key);
      activecode = 0000;
    }
  }
}

void displayinput(int counter, int activecode, char key) {
  Serial.print(counter);
  Serial.print(" ");
  Serial.print(activecode);
  Serial.print(" ");
  Serial.println(key); 
}

int checkcode(int activecode) {
  int counter=0;
  int result=0;
  
  for (int c=0;c<codequanity; c++) {
    if (activecode == secretcode[c]) {
      Serial.println("SUCESS!");

      digitalWrite(red, LOW);
      digitalWrite(green, HIGH);
      delay(500);
      digitalWrite(red, HIGH);
      digitalWrite(green, LOW);

      result = 1;
      break;
    } 
  }

  // Execute results
  if (result == 1) {
    return 1;
  } else {
      Serial.println("FAIL!");  
      return 0;
    }
}

int updatecode(int counter, int key, int activecode) {

  // convert char int to numeral
  switch (key) {
    case 49: key = 1; break;
    case 50: key = 2; break;
    case 51: key = 3; break;
    case 52: key = 4; break;
    case 53: key = 5; break;
    case 54: key = 6; break;
    case 55: key = 7; break;
    case 56: key = 8; break;
    case 57: key = 9; break;
    case 48: key = 0; break;
  }

  if (counter == 1) {
    activecode = activecode + key;
  } else if (counter > 1) {
    activecode = (activecode * 10) + key;
  } 

  return activecode;
}


//
// Display routines
//
void clearLEDs() {
  int reset = B11111111;
  shiftOut(dataPin, clockPin, MSBFIRST, reset);
}

void pickDigit(int x) {

  // Set refresh Delay
  int del = 1;
  
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

int displaynumber(int n) {
  // Inteded to be run in a loop 
  int n1, n2, n3, n4;

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
  if (DEBUG == 1) {
    if (n != 0){
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
    }
  }
  // reset digits
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  
} 