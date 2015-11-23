String comdata = "";  
void setup()
{  
   pinMode(2,OUTPUT);  
   pinMode(3,OUTPUT);
   digitalWrite(2,HIGH);
   digitalWrite(3,HIGH);
   Serial.begin(9600);
}
 
void loop()
{
 
     digitalWrite(2,HIGH);
     digitalWrite(3,HIGH);     
     while (Serial.available() > 0)  
        {
            comdata += char(Serial.read());
            delay(2);
         }
     if(comdata.length() > 0&&comdata=="0500ADDF1443")
       {   
          while(  comdata.length() > 0  )
           {
              digitalWrite(3,LOW);
              delay(100);
              while (Serial.available() > 0)  
                 {
                    comdata += char(Serial.read());
                    delay(2);
                  }
            comdata = "";
           }    
         }
     else if (comdata.length() > 0)
        {   
           while(comdata.length() > 0)
           {
             digitalWrite(2,LOW);
             delay(100);    
            while (Serial.available() > 0)  
             {
                comdata += char(Serial.read());
                delay(2);
                }       
        
            comdata = "";
           }
        }                 
}

