#define IR_IN 8 // infrared receive

int Pulse_Width = 0;//storage pulse width
int ir_code = 0x00;// user code value
char adrL_code = 0x00;//Command code
char adrH_code = 0x00;// Command code base minus one's complement

void timer1_init(void)//timer initialization function 
{
    TCCR1A = 0X00;
    TCCR1B = 0X05;//set timer clock source
    TCCR1C = 0X00;
    TCNT1 = 0X00;
    TIMSK1=0X00; //Bantimerinterruptoverflow
}
void remote_deal(void)// Implement the decoding function 
{ // data presentation

    if (adrL_code != 0) {
        Serial.println(ir_code,HEX);//16 Into system show 
        Serial.println(adrL_code,HEX);//16 Into system show
    }
}

char logic_value()//Judgment logic value "0" and "1" son function 
{
    TCNT1 = 0X00; 
    while(!(digitalRead(IR_IN))); //if low wait 
    
    Pulse_Width=TCNT1;
    TCNT1=0; 
    
    if(Pulse_Width>=7&&Pulse_Width<=10)//low level 560us
    {
    
        while(digitalRead(IR_IN));//if high wait 
        
        Pulse_Width=TCNT1;
        TCNT1=0;
        
        if(Pulse_Width>=7&&Pulse_Width<=10)//high level 560us
            return 0;
        else if(Pulse_Width>=25&&Pulse_Width<=27) //high level 1.7ms
            return 1; 
    }
    return -1; 
}

void pulse_deal()//Receiving address code and command code pulse function 
{
    int i;
    int j;
    ir_code=0x00;// clear 
    adrL_code=0x00;// clear 
    adrH_code=0x00;// clear
    // Analysis of the remote control code user code value 
    for(i=0;i<16;i++)
    {
        if(logic_value() == 1) // if 1
            ir_code |= (1<<i);//Save key value
    }
        // Analytical remote control code commands in the code 
    for(i=0;i<8;i++)
    {
        if(logic_value() == 1) // if 1
            adrL_code |= (1<<i);//save key value
    }
    // Analysis of the remote control code user code value 
    for(j = 0 ; j < 8; j++)
    {
        if(logic_value() == 1) //if 1
            adrH_code |= (1<<j);//save key value
    } 
}

void remote_decode(void)// Decoding function 
{
    TCNT1=0X00; 
    while(digitalRead(IR_IN))// if high wait
    {
        if(TCNT1>=1563) // When high level lasted for more than 100 ms, shows that at the moment no key press
        {
            ir_code=0x00ff;// user code value 
            adrL_code=0x00;// a byte value before Key code 
            adrH_code=0x00;// a byte value after Key code
            return; 
        }
    }
    // If high level can’t last for more than 100 ms 
    TCNT1=0X00;
    
    while(!(digitalRead(IR_IN))); //if low wait 
    
    Pulse_Width=TCNT1;
    TCNT1=0;

    if(Pulse_Width>=140&&Pulse_Width<=141)// 9ms
    {
        while(digitalRead(IR_IN));//if high wait 

        Pulse_Width=TCNT1;
        TCNT1=0; 
        
        if(Pulse_Width>=68&&Pulse_Width<=72)// 4.5ms
        {
            pulse_deal();
            return; 
        }
        else if(Pulse_Width>=34&&Pulse_Width<=36)//2.25ms 
        {
            while(!(digitalRead(IR_IN)));// if low wait 

            Pulse_Width=TCNT1;
            TCNT1=0; 

            if(Pulse_Width>=7&&Pulse_Width<=10)// 560us
            {
                return;
            } 
        }
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(IR_IN,INPUT);// Set infrared receiving pin for input 
    Serial.flush();
}

void loop()
{
    timer1_init();//Timer initialization 
    while(1)
    {
        remote_decode(); // decode
        remote_deal(); // Executive decoding results 
    }
}