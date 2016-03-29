#include <SoftwareSerial.h>
SoftwareSerial RFID(2, 3); // RX and TX
// ardunio pin 2 -> RDM6300 pin 1 RX
// ardunio pin 3 -> RDM6300 pin 2 TX
int ledPIN = 7;

String comdata = "";  
void setup()
{  
   RFID.begin(9600); // start serial to RFID reader
   Serial.begin(9600);
   pinMode(ledPIN,OUTPUT);
  digitalWrite(ledPIN,LOW);


}
 
void loop()
{
    
     while (RFID.available() > 0)  
        {
            comdata += char(RFID.read());
            delay(2);
         }

     if (comdata.length() > 0) {
        digitalWrite(ledPIN,HIGH);
          Serial.println(comdata);
          comdata = "";
          
     }
                
}
