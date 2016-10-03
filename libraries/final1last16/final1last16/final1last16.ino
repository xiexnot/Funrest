#include <MFRC522.h>

#include <Wire.h>

const int SLAVE_ADDRESS = 1;
char incomingByte = 0;

#include <SPI.h>

/*
Pins	SPI	   UNO  Mega2560  Leonardo
1 (NSS) SAD (SS)   10     53        10
2       SCK        13     52        SCK1
3       MOSI       11     51        MOSI1
4       MISO       12     50        MISO1
5       IRQ        *      *         *
6       GND       GND     GND       GND
7       RST        5      ?         Reset
8      +3.3V (VCC) 3V3     3V3       3.3V
* Not needed
1 on ICPS header
*/
#define SAD 10
#define RST 5
MFRC522 nfc(SAD, RST);//Create MFRC522 instance


#define ledPinOpen    6
#define ledPinClosed  7

const int pauseButtonPin=3;
boolean pauseState=false;
int prePauseState=LOW;
long pauseTime=0;
int pauseReading;
int debounce=2000;

void setup() {
  pinMode(ledPinOpen  , OUTPUT);   
  pinMode(ledPinClosed, OUTPUT);   
  pinMode(pauseButtonPin,INPUT);
  SPI.begin();// Init SPI bus
  Serial.begin(9600);//Initialize serial communications with the PC
  Serial.println("Looking for MFRC522.");
  nfc.begin();// Init MFRC522 card
  byte version = nfc.getFirmwareVersion();
  if (! version) {
    Serial.print("Didn't find MFRC522 board.");
    while(1); //halt
  }

  Serial.print("Found chip MFRC522 ");
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");
  
  prePauseState=digitalRead(pauseButtonPin);
  
}

#define AUTHORIZED_COUNT 8 /*If you want more Authorized of cards set the count here, and then add the serials below*/
byte Authorized[AUTHORIZED_COUNT][6] = {
                            {0xB0,0x28,0xD5,0xE5}
                            ,{0x97,0x57,0xD5,0xE5}
                            ,{0x90,0x5D,0xD5,0xE5}
                            ,{0xB0,0x85,0x80,0x0D}
                            ,{0x50,0x78,0xD8,0xE5}
                            ,{0x86,0x3E,0xD8,0xE5}
                            ,{0xE0,0x74,0x84,0x0D}
                            ,{0xF4,0x55,0x37,0x08}
                          };
                          
void printSerial(byte *serial);
boolean isSame(byte *key, byte *serial);
boolean isAuthorized(byte *serial);



byte a[4]={0xB0,0x28,0xD5,0xE5};
byte b[4]={0x97,0x57,0xD5,0xE5};
byte c[4]={0x90,0x5D,0xD5,0xE5};
byte d[4]={0xB0,0x85,0x80,0x0D};
byte e[4]={0x50,0x78,0xD8,0xE5};
byte f[4]={0x86,0x3E,0xD8,0xE5};
byte g[4]={0xE0,0x74,0x84,0x0D};
byte h[4]={0xF4,0x55,0x37,0x08};


//pre music
boolean preTag=true;
byte preSerial[5];

//state
int state=9;

void loop() {
  
  
  
  
  
  Wire.begin();  
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  boolean Opening = false;
  
  pauseReading=digitalRead(pauseButtonPin);
 // Serial.println(millis());
// Serial.println(pauseReading);
  if(pauseReading!=prePauseState)
  {
    pauseState=true;
   // Serial.println('9');
   Wire.write('p');
  }
  prePauseState=pauseReading;
  
  
  //digitalWrite(ledPinOpen, Opening);
  //digitalWrite(ledPinClosed, !Opening);
  //delay(1000);
  status = nfc.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);
   // printSerial(serial);
    Wire.beginTransmission(SLAVE_ADDRESS);
   // Serial.println("begin");
    if(isSame(a,serial)&&state!=0)
    {
      Serial.println('a');
      Wire.write('a');
      Serial.println('a out');
      state=0;
    }
    if(isSame(b,serial)&&state!=1)
    {
      Serial.println('b');
      Wire.write('b');
     // Serial.println('b out');
      state=1;
    }
    if(isSame(c,serial)&&state!=2)
    {
      Serial.println('c');
      Wire.write('c');
      state=2;
    }
    if(isSame(d,serial)&&state!=3)
    {
      Serial.println('d');
      Wire.write('d');
      state=3;
    }
    if(isSame(e,serial)&&state!=4)
    {
      Serial.println('e');
      Wire.write('e');
      state=4;
    }
    if(isSame(f,serial)&&state!=5)
    {
      Serial.println('f');
      Wire.write('f');
      state=5;
    }
    if(isSame(g,serial)&&state!=6)
    {
      Serial.println('g');
      Wire.write('g');
      state=6;
    }
    if(isSame(h,serial)&&state!=7)
    {
      Serial.println('h');
      Wire.write('h');
      state=7;
    }
    
    
    Wire.endTransmission();
   // Serial.println("k");
    if(isAuthorized(serial))
    { 
      //Serial.println("Authenticated");
     // Opening = true;
      
      //
      if(preTag)
      {
        memcpy(preSerial,data,5);
        preTag=false;
      }
      else
      {
        if(isSame(preSerial,serial))
        {
          //keep or start to playing;
          //printSerial(serial);
          //Serial.println("playinnnnnng~~~");
         Opening=true;
        }
        else
        {
          memcpy(preSerial,data,5);
        }
      }
    }
    else
    { 
      printSerial(serial);
      Serial.println("is NOT authenticated");
      Opening = false;
    }
    
    
    
    
    
    
    //delay(2000);
  }//if (status == MI_OK)
  
  delay(500);
  // stop the music
  if(!Opening&&state!=9)
    {
      Serial.println("stoping");
      state=9;
      Wire.beginTransmission(SLAVE_ADDRESS);
      Wire.write('2');
      Wire.endTransmission();
      Opening=false;
    }
    nfc.haltTag();
      digitalWrite(ledPinOpen, Opening);
     digitalWrite(ledPinClosed, !Opening);
     //delay(1000);
}//void loop()

boolean isSame(byte *key, byte *serial)
{
    for (int i = 0; i < 4; i++) {
      if (key[i] != serial[i])
      { 
        return false; 
      }
    }
    
    return true;

}

boolean isAuthorized(byte *serial)
{
    for(int i = 0; i<AUTHORIZED_COUNT; i++)
    {
      if(isSame(serial, Authorized[i]))
        return true;
    }
   return false;
}

void printSerial(byte *serial)
{
        Serial.print("Serial:");
    for (int i = 0; i < 4; i++) {
      Serial.print(serial[i], HEX);
      Serial.print(" ");
    }
}
