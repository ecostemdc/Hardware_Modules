
//Function: This procedure is based on ARDUINO MEGA 2560, to read the humidity temperature sensor DHT11, and serial display data.
//Time: August 17, 2012
#define DHT11  0   //Receiving the analog port A0
byte read_DHT11()//Receive 40 data
{
  byte i;
  byte temp=0;
  for(i=0;i<8;i++)
  {
    while(!(PINF & (_BV(DHT11))));//Wait for 50us whit low
    delayMicroseconds(30);//
    if(PINF & _BV(DHT11))//The high sustained after 26us-28us, if still high, the received data is '1 ', otherwise '0'.
      temp |=(1<<(7-i)); 
    while(PINF & _BV(DHT11));//wait '1' finish
  }
  return temp;
}
void setup()
{
  DDRF |= _BV(DHT11);//
  PORTF |= _BV(DHT11);
  Serial.begin(9600);
}
void loop()
{
  byte i,temp;
  byte dat[5];
  //Humidity_H = dat[0];
  //Humidity_L = dat[1];
  //Temperature_H = dat2];
  //Temperature_L = dat[3];
  //Checkdata = dat[4];
  DDRF |= _BV(DHT11);//output
  PORTF &=~_BV(DHT11); //low 
  delay(18);//More than 18ms of low
  PORTF |= _BV(DHT11);//high
  DDRF &=~ _BV(DHT11);//input
  delayMicroseconds(40);//delay 20us-40us
  while(!(PINF &  _BV(DHT11)));//In response to 80us low
  while(PINF &  _BV(DHT11));//In response to 80us high
  for(i=0;i<5;i++)//read data
    dat[i]=read_DHT11();
  temp = dat[0]+dat[1]+dat[2]+dat[3];//Check
  if(dat[4]==temp)//The correct checksum
  {
    Serial.print("Humidity:");
    Serial.print(dat[0]);
    Serial.print(".");
    Serial.print(dat[1]);
    Serial.print("%");
    Serial.println();
    Serial.print("Temperature:");
    Serial.print(dat[2]);
    Serial.print(".");
    Serial.print(dat[3]);
    Serial.print("C");
    Serial.println();
    Serial.println();
  }
  else  
  {
    Serial.print("Read failure");
    Serial.println();
  }
  delay(1000);
}
