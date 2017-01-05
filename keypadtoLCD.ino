#include <Keypad.h>
// include the library code:
#include <LiquidCrystal.h>

int DEBUG = 0;

// Define 595 control pins
int dataPin = 11; 
int latchPin = 10;
int clockPin = 9;



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A2, A3, A4, A5, A6, A7);


// Lights
int red = 11;
int green = 12;

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
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Enter Code:");
 Serial.begin(9600);
  //turn on red light
  digitalWrite(red, HIGH);
}


void loop() {


  char key = keypad.getKey();

  if (key == '*') {
    counter = 1;
    activecode = 0000;
    resetdisplay();
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
      resetdisplay();
      displayinput(counter, activecode, key);
      activecode = 0000;
    }
  }
}

void resetdisplay() {

    // Print a message to the LCD.
    lcd.print("RESET");
    delay(1);
    lcd.clear();
    lcd.print("Enter Code:");
    Serial.print("RESET ");
}

void displayinput(int counter, int activecode, char key) {
  Serial.print(counter);
  Serial.print(" ");
  Serial.print(activecode);
  Serial.print(" ");
  Serial.println(key); 

  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(activecode);
}

int checkcode(int activecode) {
  int counter=0;
  int result=0;
  
  for (int c=0;c<codequanity; c++) {
    if (activecode == secretcode[c]) {
      lcd.clear();
      lcd.print("SUCCESS!");

      digitalWrite(red, LOW);
      digitalWrite(green, HIGH);
      delay(1000);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      resetdisplay();

      result = 1;
      break;
    } 
  }

  // Execute results
  if (result == 1) {
    return 1;
  } else {
      lcd.clear();
      lcd.print("Enter Code:");
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

