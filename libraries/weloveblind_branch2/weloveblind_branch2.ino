
#include <SD.h>
#include <SPI.h>
#include <arduino.h>
#include <MusicPlayer.h>
#include <cstring>
#include <Wire.h>

const int SLAVE_ADDRESS = 1;
char incomingByte = 0;

char last_char1,last_char2;
char current_play_order_1,current_play_order_2;
char order_c1,order_c2;
char order[100];
char currentSongName[100];
char current_state[100];

void setup(void)
{
	Serial.begin(9600);
	player.begin();
	player.keyEnable();
    player.setVolume(10);
	Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
    Wire.onReceive(receiveEvent); // register event
    strcpy(current_state,"stop");
    memset(currentSongName,0,sizeof(currentSongName));
	// write Song Name to SongName
}

void play_new_song()
{
    /*
    if (order.length() !=0 )
    {
        char *p = strdup(currentSongName.c_str());
        free(p);
    }
    */
    if (strlen(currentSongName)!=0)
    {
        player.opStop();
        delay(100);
        player.deleteSong(currentSongName);
        Serial.print("start delete: ");
        Serial.println(currentSongName);
        Serial.println("delete..");        
        delay(100);
    }
    strcpy(current_state,"play");
    strcpy(currentSongName,order);
    player.playOne(order);
    Serial.println(order);
    player.opPlay();        
    delay(100);
    player.opNextSong();
    delay(100);
        // i don't know why i should opPlay and opNextSong, but it seems work.
        // if there is two songs in the list, it will play 2nd first.
}

void get_order()
{
    memset(order,0,sizeof(order));
    
    // cb bc
    if (order_c1 == 'c' && order_c2 == 'b') 
        strcpy(order,"SE.mp3");
    if (order_c1 == 'b' && order_c2 == 'c') 
        strcpy(order,"ES.mp3");
        
    //ac ca
    if (order_c1 == 'c' && order_c2 == 'a') 
        strcpy(order,"DS.mp3");
    if (order_c1 == 'a' && order_c2 == 'c') 
        strcpy(order,"SD.mp3");
    
    //ad da
    if (order_c1 == 'd' && order_c2 == 'a') 
        strcpy(order,"DP.mp3");
    if (order_c1 == 'a' && order_c2 == 'd') 
        strcpy(order,"PD.mp3");
        
        //ae ea
    if (order_c1 == 'e' && order_c2 == 'a') 
        strcpy(order,"DC.mp3");
    if (order_c1 == 'a' && order_c2 == 'e') 
        strcpy(order,"CD.mp3");
        
     //af fa
    if (order_c1 == 'f' && order_c2 == 'a') 
        strcpy(order,"DDuck.mp3");
    if (order_c1 == 'a' && order_c2 == 'f') 
        strcpy(order,"DuckD.mp3");
        
     //ag ga
    if (order_c1 == 'g' && order_c2 == 'a') 
        strcpy(order,"DT.mp3");
    if (order_c1 == 'a' && order_c2 == 'g') 
        strcpy(order,"TD.mp3");
        
        //ah ha
    if (order_c1 == 'h' && order_c2 == 'a') 
        strcpy(order,"DR.mp3");
    if (order_c1 == 'a' && order_c2 == 'h') 
        strcpy(order,"RD.mp3");
        
    // bg gb
    if (order_c1 == 'g' && order_c2 == 'b')
        strcpy(order,"TE.mp3");
    if (order_c1 == 'b' && order_c2 == 'g')
        strcpy(order,"ET.mp3");
    
    // ba ab
    if (order_c1 == 'b' && order_c2 == 'a')
        strcpy(order,"ED.mp3");
    if (order_c1 == 'a' && order_c2 == 'b')
        strcpy(order,"DE.mp3");

    // bf fb
    if (order_c1 == 'f' && order_c2 == 'b')
        strcpy(order,"DuckE.mp3");
    if (order_c1 == 'b' && order_c2 == 'f')
        strcpy(order,"EDuck.mp3");   

    // bd db
    if (order_c1 == 'd' && order_c2 == 'b')
        strcpy(order,"PE.mp3");
    if (order_c1 == 'b' && order_c2 == 'd')
        strcpy(order,"EP.mp3");        
   
    // be eb
    if (order_c1 == 'e' && order_c2 == 'b')
        strcpy(order,"CE.mp3");
    if (order_c1 == 'b' && order_c2 == 'e')
        strcpy(order,"EC.mp3");
        
    // ah ha
    if (order_c1 == 'a' && order_c2 == 'h')
        strcpy(order,"DR.mp3");
    if (order_c1 == 'h' && order_c2 == 'a')
        strcpy(order,"RD.mp3");
    
    //bh hb
    if (order_c1 == 'h' && order_c2 == 'b')
        strcpy(order,"RE.mp3");
    if (order_c1 == 'b' && order_c2 == 'h')
        strcpy(order,"ER.mp3");
        
    // hd dh
    if (order_c1 == 'd' && order_c2 == 'h')
        strcpy(order,"PR.mp3");
    if (order_c1 == 'h' && order_c2 == 'd')
        strcpy(order,"RP.mp3");

    // hc ch
    if (order_c1 == 'c' && order_c2 == 'h')
        strcpy(order,"SR.mp3");
    if (order_c1 == 'h' && order_c2 == 'c')
        strcpy(order,"RS.mp3");
   
    // cd dc 
    if (order_c1 == 'c' && order_c2 == 'd')
        strcpy(order,"SP.mp3");
    if (order_c1 == 'd' && order_c2 == 'c')
        strcpy(order,"PS.mp3");

    // cg gc
    if (order_c1 == 'c' && order_c2 == 'g')
        strcpy(order,"ST.mp3");
    if (order_c1 == 'g' && order_c2 == 'c')
        strcpy(order,"TS.mp3");  

    // df fd
    if (order_c1 == 'd' && order_c2 == 'f')
        strcpy(order,"PDuck.mp3");
    if (order_c1 == 'f' && order_c2 == 'd')
        strcpy(order,"DuckP.mp3");  
        
            // dg gd
    if (order_c1 == 'd' && order_c2 == 'g')
        strcpy(order,"PT.mp3");
    if (order_c1 == 'g' && order_c2 == 'd')
        strcpy(order,"TP.mp3");  
        
    // gh hg
    if (order_c1 == 'g' && order_c2 == 'h')
        strcpy(order,"TR.mp3");
    if (order_c1 == 'h' && order_c2 == 'g')
        strcpy(order,"RT.mp3");

    // cf fc
    if (order_c1 == 'f' && order_c2 == 'c')
        strcpy(order,"SDuck.mp3");
    if (order_c1 == 'c' && order_c2 == 'f')
        strcpy(order,"DuckS.mp3");


    // de ed
    if (order_c1 == 'd' && order_c2 == 'e')
        strcpy(order,"PC.mp3");
    if (order_c1 == 'e' && order_c2 == 'd')
        strcpy(order,"CP.mp3");
        
    //ef fe
    if (order_c1 == 'e' && order_c2 == 'f')
        strcpy(order,"CDuck.mp3");
    if (order_c1 == 'f' && order_c2 == 'e')
        strcpy(order,"DuckC.mp3");

            //gf fg
    if (order_c1 == 'g' && order_c2 == 'f')
        strcpy(order,"TDuck.mp3");
    if (order_c1 == 'f' && order_c2 == 'g')
        strcpy(order,"DuckT.mp3");

            //fh hf
    if (order_c1 == 'h' && order_c2 == 'f')
        strcpy(order,"RDuck.mp3");
    if (order_c1 == 'f' && order_c2 == 'h')
        strcpy(order,"DuckR.mp3");
            
    // eh he
    if (order_c1 == 'e' && order_c2 == 'h')
        strcpy(order,"CR.mp3");
    if (order_c1 == 'h' && order_c2 == 'e')
        strcpy(order,"RC.mp3");   
        
    // ce ec
    if (order_c1 == 'e' && order_c2 == 'c')
        strcpy(order,"CS.mp3");
    if (order_c1 == 'c' && order_c2 == 'e')
        strcpy(order,"SC.mp3");    
     
    // ge eg   
    if (order_c1 == 'e' && order_c2 == 'g')
        strcpy(order,"CT.mp3");
    if (order_c1 == 'g' && order_c2 == 'e')
        strcpy(order,"TC.mp3"); 
     
    delay(100);
}

void modeB()
{

    if (strcmp(current_state,"play")==0)
    {
        //Serial.println(current_state);
        Serial.print(last_char1);
        Serial.print(last_char2);
        Serial.println(" - last char");
        Serial.print(current_play_order_1);
        Serial.print(current_play_order_2);
        Serial.println(" - current char");
        Serial.print(order_c1);
        Serial.print(order_c2);
        Serial.println(" - order char");
        if (order_c1 != 'p' && order_c2 !='p' && ((last_char1 != order_c1) || (last_char2 != order_c2)) ) // new CPO (not pause/play/signal)
        {
            if (order_c1!=0 && order_c2!=0 && (order_c1 != current_play_order_1 || order_c2 != current_play_order_2 ))
            {
                Serial.println("new song oh oh from play~~~");
                get_order();
                Serial.println(order);
                play_new_song();
                strcpy(current_state,"play");
                last_char1 = order_c1;
                last_char2 = order_c2;
                current_play_order_1 = order_c1;
                current_play_order_2 = order_c2;
                order_c1 = 0;
                order_c2 = 0;
                delay(100);
                //Serial.print("current_state = ");
                //Serial.println(current_state);
            }
        }

        // get pause/paly signal
        // when considering order_c1 != last_char, what I focus on is the possibility of receiving command repeatly.

        if (order_c1 == 'p') 
        {
            Serial.println("here comes a pause. 1");
            strcpy(current_state,"pause");
            player.opPause();
            delay(100);
            last_char1 = order_c1;
            last_char2 = order_c2;
            order_c1 = 0;
            order_c2 = 0;
            //Serial.println("status: pause 1 ...");
            //Serial.print("current_state = ");
            //Serial.println(current_state);
        }
    }
    /*---------------------------------------------------------------------*/
    if (strcmp(current_state,"pause")==0)
    {

        if (order_c1 != 'p' && !(last_char1 == order_c1 && last_char2 == order_c2) ) // new CPO (not pause/play/signal)
        //if (order_c1 != 'p' && order_c2 !='p' )
        {
            if (order_c1!=0 && order_c2!=0 && !(order_c1 == current_play_order_1 && order_c2 == current_play_order_2 ))
            {
                Serial.println("new song oh oh from pause~~~");
                get_order();
                Serial.println(order);
                play_new_song();
                delay(100);
                strcpy(current_state,"play");
                last_char1 = order_c1;
                last_char2 = order_c2;
                //strcpy(current_state,"play");
                current_play_order_1 = order_c1;
                current_play_order_2 = order_c2;
            }
            if (order_c1!=0 && order_c2!=0 && (order_c1 == current_play_order_1 && order_c2 == current_play_order_2 ))
            {
              Serial.println("here comes a /'continue/' play command. 2");
                
              strcpy(current_state,"play");
              
              player.opPlay();
              delay(100);
              last_char1 = order_c1;
              last_char2 = order_c2;
              order_c1 = 0;
              order_c2 = 0;
            }
        }
  
        if (order_c1 == 'p')
        {
            Serial.println("here comes a pause. 2");
            Serial.println("I will ignore it..");
            //strcpy(current_state,"play");
            /*
            strcpy(current_state,"play");
            player.opPlay();
            delay(100);
            last_char1 = order_c1;
            last_char2 = order_c2;
            */
            order_c1 = 0;
            order_c2 = 0;
            //Serial.println("status: pause...");
            //Serial.println("status: pause 2 ...");
            //Serial.print("current_state = ");
            //Serial.println(current_state);
        }
        return ;
    }
    /*---------------------------------------------------------------------*/
    if (strcmp(current_state,"stop")==0)
    {
             // Serial.println(current_state);
         if (order_c1 != 'p' && order_c1 != 0 ) // new CPO (not pause/play/signal)
        {
            if (order_c1 != current_play_order_1 || order_c2 != current_play_order_2)
            {
                get_order();
                play_new_song();
                delay(100);
                last_char1 = order_c1;
                last_char2 = order_c2;
                strcpy(current_state,"play");
                current_play_order_1 = order_c1;
                current_play_order_2 = order_c2;
                order_c1 = 0;
                order_c2 = 0;
            }
            
        }
    }
}

void loop(void)
{
    player.play();
    fflush(stdin);
    /*
        change the playing mode without any further proc.
    */
    //Serial.println("judge");
    modeB();
    delay(100);
}

void receiveEvent(int howMany)
{
    while (Wire.available()) 
    {
        // receive one byte from Master
        incomingByte = Wire.read();
        /*
            p = play/pause
            q,r = two different control modes
        */
        if (incomingByte == 'p') 
        {
            order_c1 = incomingByte;
            order_c2 = incomingByte;
        }
        else
        {
            if (order_c1!=0 && order_c2==0)
            {
                order_c2 = incomingByte;
            }
            else
            {
                if ((order_c1==0) && (order_c2==0))
                {
                    order_c1 = incomingByte;
                }
                else
                  if ((order_c1 !=0) && (order_c2 !=0 ))
                  {
                      order_c1=incomingByte;
                      order_c2 = 0;
                  }
            }
            
            /*
            if (order_c1==0)
              order_c1 = incomingByte;
            else
              if (order_c2==0)
                order_c2 = incomingByte;
                */
        }
        Serial.println(incomingByte);
    }  
}

