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


#define ledPin 6
#define ledPin2  7

//for switch
#define pausePin 8
#define modePin 9

int prePauseState;
int readingPauseState;

int modeState;

//end for switch


boolean op=false;
///
#define NCC 3
#define RST1 4
MFRC522 nfc2(NCC, RST1);

//char preLeft='z';
//char preRight='z';
void setup() {
  pinMode(ledPin  , OUTPUT);   
  pinMode(ledPin2, OUTPUT); 
  
  pinMode(pausePin,INPUT);
  pinMode(modePin,INPUT);
  
  SPI.begin();// Init SPI bus
  Serial.begin(115200);//Initialize serial communications with the PC
  Serial.println("Looking for MFRC522.");
  
  nfc.begin();// Init MFRC522 card
  nfc2.begin();
  
  byte version = nfc.getFirmwareVersion();
  if (! version) {
    Serial.print("rc1 didn't find MFRC522 board.");
    while(1); //halt
  }

//
 version = nfc2.getFirmwareVersion();
if (! version) {
    Serial.print("rc2 didn't find MFRC522 board.");
    while(1); //halt
  }

  Serial.print("both Found chip MFRC522 ");
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");
  
  //digitalWrite(pausePin,HIGH);
  prePauseState=1;
//  preModeState=readingModeState=digitalRead(modePin);
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

boolean preTag2=true;
byte preSerial2[5];

//state
int state=9;
int state2=9;

int tick=0;

boolean sentTag=false;
boolean stopTag=true;

char left;
char right;

long time=0;
void loop() {
  
  Wire.begin();  
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  boolean Opening = false;
  
  
  status = nfc.requestTag(MF1_REQIDL, data);
  ////
  boolean Opening2=false;
  byte status2;
  byte data2[MAX_LEN];
  byte serial2[5];
  status2 = nfc2.requestTag(MF1_REQIDL, data2);
  
 // digitalWrite(ledPinOpen, Opening&&Opening2);
 // digitalWrite(ledPinClosed, !(Opening&&Opening2));
 Wire.beginTransmission(SLAVE_ADDRESS);
  //for pause switch
  digitalWrite(pausePin,HIGH);
  readingPauseState=digitalRead(pausePin);
  //Serial.println(readingPauseState);
  if(readingPauseState==0&&prePauseState==1&&modeState)
  {
    Wire.write('p');
    Serial.println('p');
  }
  prePauseState=readingPauseState;
  
  //for mode switch
  modeState=digitalRead(modePin);
 // Serial.println("modeState");
 //Serial.println(modeState);
  
  if (status == MI_OK) 
  {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);
   // printSerial(serial);
    
   // Serial.println("begin");
    if(isSame(a,serial)&&state!=0)
    {
      //Serial.println("left dog");
      left='a';
      //Serial.println('a out');
      state=0;
    }
    if(isSame(b,serial)&&state!=1)
    {
     // Serial.println("left elephant");
      //Wire.write("lb");
      left='b';
      //Serial.println('b out');
      state=1;
    }
    if(isSame(c,serial)&&state!=2)
    {
      //Serial.println("left sheep");
      //Wire.write("lc");
      left='c';
      state=2;
    }
    if(isSame(d,serial)&&state!=3)
    {
      //Serial.println("left pig");
      //Wire.write("ld");
      left='d';
      state=3;
    }
    if(isSame(e,serial)&&state!=4)
    {
      //Wire.write("le");
      left='e';
      state=4;
    }
    if(isSame(f,serial)&&state!=5)
    {
     // Wire.write("lf");
     left='f';
      state=5;
    }
    if(isSame(g,serial)&&state!=6)
    {
     // Wire.write("lf");
     left='g';
      state=6;
    }
    if(isSame(h,serial)&&state!=7)
    {
     // Wire.write("lf");
     left='h';
      state=7;
    }
    
    
   // Wire.endTransmission();
   // Serial.println("k");
    if(isAuthorized(serial))
    { 
      //Serial.println("Authenticated");
      Opening = true;
      tick++;
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
         // printSerial(serial);
         // Serial.println("playinnnnnng~~~");
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
   // digitalWrite(ledPinOpen, Opening);
   // digitalWrite(ledPinClosed, !Opening);
    //delay(2000);
  }//if (status == MI_OK)
  /////
  else
  Opening=false;
  
     if (status2 == MI_OK) 
     {
       status2= nfc2.antiCollision(data2);
       memcpy(serial2, data2, 5);
       
      // Serial.println("begin");
       if(isSame(a,serial2)&&state2!=0)
      {
         //Serial.println("right dog");
        // Wire.write("ra");
        right='a';
        //Serial.println('a out');
         state2=0;
      }
      if(isSame(b,serial2)&&state2!=1)
      {
        // Serial.println("right elephant");
        // Wire.write("rb");
        right='b';
      //Serial.println('b out');
         state2=1;
     }
    if(isSame(c,serial2)&&state2!=2)
    {
       //Serial.println("right sheep");
       //Wire.write("rc");
       right='c';
       state2=2;
    }
    if(isSame(d,serial2)&&state2!=3)
    {
      //Serial.println("right pig");
      //Wire.write("rd");
      right='d';
      state2=3;
    }
    if(isSame(e,serial2)&&state2!=4)
    {
     // Wire.write("re");
     right='e';
      state2=4;
    }
    if(isSame(f,serial2)&&state2!=5)
    {
      //Wire.write("rf");
      right='f';
      state2=5;
    }
    
    if(isSame(g,serial2)&&state2!=6)
    {
      //Wire.write("rf");
      right='g';
      state2=6;
    }
    if(isSame(h,serial2)&&state2!=7)
    {
      //Wire.write("rf");
      right='h';
      state2=7;
    }
    
    
    //Wire.endTransmission();
   // Serial.println("k");
    if(isAuthorized(serial2))
    { 
      //Serial.println("Authenticated");
      Opening2 = true;
      tick++;
      //
      if(preTag2)
      {
        memcpy(preSerial2,data2,5);
        preTag2=false;
        
      }
      else
      {
        if(isSame(preSerial2,serial2))
        {
          //keep or start to playing;
          //printSerial(serial2);
         // Serial.println("playinnnnnng~~~");
         
        }
        else
        {
          memcpy(preSerial2,data2,5);
          
        }
      }
    }
    else
    { 
      printSerial(serial2);
      Serial.println(" r2 is NOT authenticated");
      Opening2 = false;
    }
  
  }//ok
  else
  Opening2=false;
  
    /////
    //nfc2.haltTag();
  //  digitalWrite(ledPinOpen, Opening&&Opening2);
   // digitalWrite(ledPinClosed, !(Opening&&Opening2));
    
    
    
    
 // delay(500);
  // stop the music
    
   if(isSame(preSerial2,serial2)&&isSame(preSerial,serial)&&Opening==true&&Opening2==true&&!sentTag&&millis()-time>1000)
   {
    // Serial.println("same");
      char send[2];
     // Wire.beginTransmission(SLAVE_ADDRESS);
      //send[0]=left;
     // send[1]=right;
      Wire.write(left);
      delay(100);
      Wire.write(right);
      //Wire.write(send);
      //Wire.endTransmission();
      sentTag=true;
      stopTag=false;
     // Serial.println(send);
      Serial.println(left);
      Serial.println(right);
      op=true;
     // preLeft=left;
     // preRight=right;
     time=millis();
   }
  if((Opening==false||Opening2==false)&&stopTag==false&&sentTag==true)
  {
  // Serial.println("up mode 0");
    sentTag=false;
    //stopTag=false;
  }
 // if(Opening==false&&Opening2==true&&stopTag==false&&sentTag==true&&modeState)
  //{
  // Serial.println("up mode 0");
  //  sentTag=false;
    //stopTag=false;
 // }
  //if(Opening==true&&Opening2==false&&stopTag==false&&sentTag==true&&modeState)
  //{
  // Serial.println("up mode 0");
   // sentTag=false;
    //stopTag=false;
 // }
 // if(Opening==false&&Opening2==false&&stopTag==false&&modeState)
   // {
      //tick=0;
      
    //  Serial.println("pause");
     // state=9;
     // state2=9;
     // Wire.beginTransmission(SLAVE_ADDRESS);
    //  Wire.write('p');
     // Wire.endTransmission();
      //Opening=false;
    //  Opening2=false;
     // stopTag=true;
      //sentTag=false;
      //memcpy(preSerial2,"12345",5);
     // memcpy(preSerial,"23456",5);
     // memcpy(serial,"34567",5);
     // memcpy(serial2,"45678",5);
      //op=false;
   // }
    nfc2.haltTag();
    digitalWrite(ledPin, Opening);
    digitalWrite(ledPin2, Opening2);
    Wire.endTransmission();
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
