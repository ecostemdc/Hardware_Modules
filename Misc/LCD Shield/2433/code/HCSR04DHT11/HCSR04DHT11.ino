int DI = 12;
int RW = 11;
int DB[] = {3, 4, 5, 6, 7, 8, 9, 10};//Use an array to define the bus 
int Enable = 2;
int TrigPin = 30;
int EchoPin = 32;
int DHpin = 34;	
byte dat[5];
char str[]="0123456789";
char str1[]=" .-:";
char str3[]="   juli:   CM";
char str4[]="  H:  %  T:  C  ";

byte read_data()
{
byte data;
for(int i=0; i<8; i++)
{
if(digitalRead(DHpin) == LOW)
{
while(digitalRead(DHpin) == LOW); //Waiting for 50 us
delayMicroseconds(30); //Determine the duration of the high level, in order to determine the data is' 0 'or' 1 '
if(digitalRead(DHpin) == HIGH)
data |= (1<<(7-i)); //High in the former, in the low

while(digitalRead(DHpin) == HIGH); //If the data is '1', wait for a reception
}
}
return data;
}
void start_test()
{
digitalWrite(DHpin,LOW);//Pull down the bus, the starting signal
delay(30);//Delay more than 18 ms, so that DHT11 can detect the start signal
digitalWrite(DHpin,HIGH);
delayMicroseconds(40);//wait for DHT11 response
pinMode(DHpin,INPUT);
while(digitalRead(DHpin) == HIGH);
delayMicroseconds(80); //DHT11 response, lower bus 80 us
if(digitalRead(DHpin) == LOW);
delayMicroseconds(80); //DHT11 up bus after 80 us start sending data
for(int i=0;i<4;i++) //Receive the temperature and humidity data, check digit does not consider;
dat[i] = read_data();
pinMode(DHpin,OUTPUT);

digitalWrite(DHpin,HIGH); //Released after sending a data bus, wait for the host of the next start signal;
}
void LcdCommandWrite(int value) {
	// 定义所有引脚
        int i = 0;
        digitalWrite(DI, LOW);
	digitalWrite(RW, LOW);
	for (i=DB[0]; i<=DB[7]; i++) //Bus assignment
	{
	   digitalWrite(i,value & 0x01);//Because 1602 liquid crystal signal recognition is D7 - do (not do - D7), here is used for inversion of the signal.
	   value >>= 1;
	}
        delayMicroseconds(1);  
	digitalWrite(Enable,HIGH);
	delayMicroseconds(1);  
	digitalWrite(Enable,LOW);
	delayMicroseconds(1);  
        digitalWrite(DI,HIGH);
        digitalWrite(RW,HIGH);
	}
	
void LcdDataWrite(int value) {
	// 定义所有引脚
	int i = 0;
	digitalWrite(DI, HIGH);
	digitalWrite(RW, LOW);
	for (i=DB[0]; i<=DB[7]; i++) {
	   digitalWrite(i,value &0x01);
	   value >>= 1;
	}
	delayMicroseconds(1);
	digitalWrite(Enable,HIGH);
	delayMicroseconds(1);
	digitalWrite(Enable,LOW);
	delayMicroseconds(1);  
        digitalWrite(DI,LOW);
        digitalWrite(RW,HIGH);
	}

void setup (void) {
	int i = 0;
	for (i=Enable; i <= DI; i++) {
	   pinMode(i,OUTPUT);
	}
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  pinMode(DHpin,OUTPUT);
	delay(100);
	LcdCommandWrite(0x38);  // Set to 8 - bit interface, line 2, according to 5 x7 size                    
	delay(64);                      
	LcdCommandWrite(0x38);  // Set to 8 - bit interface, line 2, according to 5 x7 size                     
	delay(50);                      
	LcdCommandWrite(0x38);  // Set to 8 - bit interface, line 2, according to 5 x7 size                       
	delay(20);                      
	LcdCommandWrite(0x06);  // Input mode  Automatic incremental, shows no shift
	delay(20);                      
	LcdCommandWrite(0x0E);   // Open to open screen, the cursor displays, flicker-free
	delay(20);                      
	LcdCommandWrite(0x01);  // clear screen, the cursor position return to zero  
	delay(100);                      
	LcdCommandWrite(0x80);   // Open the screen, the cursor displays, flicker-free
	delay(20);                      
	}		
void loop (void) {
  int i;
  int distance,duration;
 
  digitalWrite(TrigPin,HIGH);//TrigPin prepare high of more than 10us
  delayMicroseconds(11);
  digitalWrite(TrigPin,LOW);

   duration = pulseIn(EchoPin, HIGH);//EchoPin received high start counting until the receiver to the low,return to the count valu
    duration = duration/29/2;//Calculating the distance  cm
                           // The speed of sound is 340 m/s or 29 microseconds per centimeter.
     
          LcdCommandWrite(0x01);  // clear screen, the cursor position return to zero 
          delay(50);
	
     
    LcdCommandWrite(0x80);
      for(i=0;i<=12;i++)
     {
       LcdDataWrite(str3[i]);
       }
     LcdCommandWrite(0x80+8);  
     if( duration<=9)    //Distance is less than 9 cm
     {
     LcdDataWrite(str1[0]);
     LcdDataWrite(str1[0]);
     LcdDataWrite(str[duration]);
     }
     else if(duration<100&&duration>=10) // Distance is less than 100 cm
     {
      LcdDataWrite(str1[0]);
      LcdDataWrite(str[duration/10]);
      LcdDataWrite(str[duration%10]);
     }
     else                              // Distance is more than 100 cm
      {
      LcdDataWrite(str[duration/100]);
      LcdDataWrite(str[duration%100/10]);
      LcdDataWrite(str[duration%100%10]);
     }
      LcdCommandWrite(0xC0);
 for(i=0;i<=15;i++)
 {  LcdDataWrite(str4[i]);
 }
     start_test();      //Read the temperature and humidity
     LcdCommandWrite(0xC0+4);
     LcdDataWrite(str[dat[0]/10]);      //display the humidity
     LcdDataWrite(str[dat[0]%10]);
     LcdCommandWrite(0xC0+11);
     LcdDataWrite(str[dat[2]/10]);      //display the temperature
     LcdDataWrite(str[dat[2]%10]);
     
    delay(1000);
}

