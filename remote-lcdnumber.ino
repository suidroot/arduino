// Remote control display numbers


const int ON = HIGH;
const int OFF = LOW;


//pwr 5v - white
//number pwr 3.3v
// Setup 595
int latchPin = 5; //(39 green)
int clockPin = 4; //(40 blue)
int dataPin = 2; //(37 white)

// IR reader
#define IR_IN  8  //(5 yellow)

int Pulse_Width = 0;
int ir_code = 0x00;
char  adrL_code = 0x00;
char  adrH_code = 0x00;


// codes
// abcdefg(df)
// (df)gfedcba
int digital_1 = B00000110;
int digital_2 = B01011011;
int digital_3 = B01001111;
int digital_4 = B01100110;
int digital_5 = B01101101;
int digital_6 = B01111101;
int digital_7 = B00000111;
int digital_8 = B01111111;
int digital_9 = B01100111;
int digital_0 = B00111111;
int reset_number = B00000000;

int ledState = 0;

void setup() {

  // 595 setup
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  //updateLEDs(reset_number);


  // IR setup 
  pinMode(IR_IN,INPUT);

  // setup serial interface
  Serial.begin(9600);  
  Serial.flush();

}

void loop() {
  // Main Loop

  timer1_init();
  while(1)
  {
    remote_decode();  
    remote_deal();   
  }  
}

void timer1_init(void)
{
  TCCR1A = 0X00; 
  TCCR1B = 0X05;
  TCCR1C = 0X00;
  TCNT1 = 0X00;
  TIMSK1 = 0X00;    
}

void updateLEDs(int value) {
  // write binary data to 595 serial controller
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  digitalWrite(latchPin,HIGH);
}

void remote_deal(void) { 
    // Set the number based on IR code
    if (adrL_code != 0) {
      Serial.println(adrL_code,HEX);
    }

    if (adrL_code == 0X0C) {
        //1
        updateLEDs(digital_1);
        Serial.println("1");
        delay(200);
    } else if (adrL_code == 0X18) {
        //2
        updateLEDs(digital_2);
        Serial.println("2");
        delay(200);
    } else if (adrL_code == 0X5E) {
        //3
        updateLEDs(digital_3);
        Serial.println("3");
        delay(200);
    } else if (adrL_code == 0X08) {
        //4
        updateLEDs(digital_4);
        Serial.println("4");
        delay(200);
    } else if (adrL_code == 0X1C) {
        //5
        updateLEDs(digital_5);
        Serial.println("5");
        delay(200);
    } else if (adrL_code == 0X5A) {
        //6
        updateLEDs(digital_6);
        Serial.println("6");
        delay(200);
    } else if (adrL_code == 0X42) {
        //7
        updateLEDs(digital_7);
        Serial.println("7");
        delay(200);
    } else if (adrL_code == 0X52) {
        //8
        updateLEDs(digital_8);
        Serial.println("8");
        delay(200);
    } else if (adrL_code == 0X4A) {
        //9
        updateLEDs(digital_9);
        Serial.println("9");
        delay(200);
    } else if (adrL_code == 0X16) {
        //0
        updateLEDs(digital_0);
        Serial.println("0");
        delay(200);
    } else if (adrL_code == 0X07) {
        // reset
        updateLEDs(reset_number);
        Serial.println("reset");
        delay(200);

    } else if (adrL_code == 0X0D) {

      updateLEDs(digital_1);
      Serial.println("1");

      delay(300);
      updateLEDs(digital_2);
      Serial.println("2");
      
      delay(300);
      updateLEDs(digital_3);
      Serial.println("3");
      delay(300);
      updateLEDs(digital_4);
      Serial.println("4");
      delay(300);
      updateLEDs(digital_5);
      Serial.println("5");
      delay(300);
      updateLEDs(digital_6);
      Serial.println("6");
      delay(300);
      updateLEDs(digital_7);
      Serial.println("7");
      delay(300);
      updateLEDs(digital_8);
      Serial.println("8");
      delay(300);
      updateLEDs(digital_9);
      Serial.println("9");
      delay(300);
      updateLEDs(digital_0);
      
      Serial.println("0");
      delay(300);
        // reset
        updateLEDs(reset_number);
        Serial.println("reset");
        delay(200);

    }
}

char logic_value() {
  TCNT1 = 0X00;
  while(!(digitalRead(IR_IN))); 
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=7&&Pulse_Width<=10)
  {
    while(digitalRead(IR_IN));
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=7&&Pulse_Width<=10)
      return 0;
    else if(Pulse_Width>=25&&Pulse_Width<=27) 
      return 1;
  }
  return -1;
}

void pulse_deal() {
  int i;
  int j;
  ir_code=0x00;
  adrL_code=0x00;
  adrH_code=0x00;

 
  for(i = 0 ; i < 16; i++)
  {
    if(logic_value() == 1) 
        ir_code |= (1<<i);
  }
  
  for(i = 0 ; i < 8; i++)
  {
    if(logic_value() == 1) 
      adrL_code |= (1<<i);
  }
  
  for(j = 0 ; j < 8; j++)
  {
    if(logic_value() == 1) 
        adrH_code |= (1<<j);
  }
}


void remote_decode(void) {
  TCNT1=0X00;       
  while(digitalRead(IR_IN))
  {
    if(TCNT1>=1563)  
    {
      ir_code=0x00ff;
      adrL_code=0x00;
      adrH_code=0x00;
      return;
    }  
  }

  
  TCNT1=0X00;

  while(!(digitalRead(IR_IN))); 
  Pulse_Width=TCNT1;
  TCNT1=0;
  if(Pulse_Width>=140&&Pulse_Width<=141)//9ms
  {

    while(digitalRead(IR_IN));
    Pulse_Width=TCNT1;
    TCNT1=0;
    if(Pulse_Width>=68&&Pulse_Width<=72)//4.5ms
    {  
      pulse_deal();
      return;
    }
    else if(Pulse_Width>=34&&Pulse_Width<=36)//2.25ms
    {
      while(!(digitalRead(IR_IN)));
      Pulse_Width=TCNT1;
      TCNT1=0;
      if(Pulse_Width>=7&&Pulse_Width<=10)//560us
      {
        return; 
      }
    }
  }
}




