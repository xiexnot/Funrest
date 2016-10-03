#include <../MFRC522.h>

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


#define ledPinOpen    2
#define ledPinClosed  3


void setup() {
  pinMode(ledPinOpen  , OUTPUT);   
  pinMode(ledPinClosed, OUTPUT);   
  SPI.begin();// Init SPI bus
  Serial.begin(115200);//Initialize serial communications with the PC
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
}

#define AUTHORIZED_COUNT 6 /*If you want more Authorized of cards set the count here, and then add the serials below*/
byte Authorized[AUTHORIZED_COUNT][6] = {
                            {0x7C,0x41,0x9B,0x32}
                            ,{0x7C,0x2C,0x63,0x32}
                            ,{0x7C,0x9E,0xCD,0x54}
                            ,{0x44,0xB8,0x45,0x08}
                            ,{0x14,0x0D,0x46,0x08}
                            ,{0xD4,0x5E,0x24,0xAB}
                          };
                          
void printSerial(byte *serial);
boolean isSame(byte *key, byte *serial);
boolean isAuthorized(byte *serial);



byte a[4]={0x7C,0x41,0x9B,0x32};
byte b[4]={0x7C,0x2C,0x63,0x32};
byte c[4]={0x7C,0x9E,0xCD,0x54};
byte d[4]={0x44,0xB8,0x45,0x08};
byte e[4]={0x14,0x0D,0x46,0x08};
byte f[4]={0xD4,0x5E,0x24,0xAB};


//pre music
boolean preTag=true;
byte preSerial[5];

//state
int state=2;

void loop() {
  
  Wire.begin();  
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  boolean Opening = false;
  digitalWrite(ledPinOpen, Opening);
  digitalWrite(ledPinClosed, !Opening);
  status = nfc.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);
    
    Wire.beginTransmission(SLAVE_ADDRESS);
    if(isSame(a,serial)&&state!=0)
    {
      Wire.write('a');
      state=0;
    }
    if(isSame(b,serial)&&state!=1)
    {
      Wire.write('b');
      state=1;
    }
    if(isSame(c,serial)&&state!=2)
    {
      Wire.write('c');
      state=2;
    }
    if(isSame(d,serial)&&state!=3)
    {
      Wire.write('d');
      state=3;
    }
    if(isSame(e,serial)&&state!=4)
    {
      Wire.write('e');
      state=4;
    }
    if(isSame(f,serial)&&state!=5)
    {
      Wire.write('f');
      state=5;
    }
    
    
    Wire.endTransmission();
    
    if(isAuthorized(serial))
    { 
      //Serial.println("Authenticated");
      Opening = true;
      
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
          printSerial(serial);
          Serial.println("playinnnnnng~~~");
         
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
    
    nfc.haltTag();
    digitalWrite(ledPinOpen, Opening);
    digitalWrite(ledPinClosed, !Opening);
    
    
    
    //delay(2000);
  }//if (status == MI_OK)
  
  delay(500);
  // stop the music
  if(!Opening&&state!=2)
    {
      Serial.println("stoping");
      state=2;
      Wire.beginTransmission(SLAVE_ADDRESS);
      Wire.write('2');
      Wire.endTransmission();
    }
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
