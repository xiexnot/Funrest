
#include <SD.h>
#include <SPI.h>
#include <arduino.h>
#include <MusicPlayer.h>
#include <cstring>
#include <Wire.h>

const int SLAVE_ADDRESS = 1;
char incomingByte = 0;

char last_char;
char order_char;
String order;
String currentSongName;

void setup(void)
{
	Serial.begin(9600);
	player.begin();
	player.keyEnable();
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
        Wire.onReceive(receiveEvent); // register event
        player.setVolume(10);
        player.play();
	// write Song Name to SongName
}


int currentState; // 0 = pause; 1 = play

void pause_current_song(void)
{
	currentState = 0;
        player.opPause();
}

void continue_play_current_song(void)
{
	currentState = 1;
  player.opPlay();        
}

void play_new_song(String order)
{
	//player.opStop();
	if (order.length() !=0 )
  {
    char *p = strdup(currentSongName.c_str());
    //player.deleteSong(p);
    // there should be a comment
    free(p);
  }
  char *q = strdup(order.c_str());
	currentSongName = order;
	player.playOne(q);
        //player.addToPlaylist(q);
        player.opPlay();        
        player.opNextSong();
        // i don't know why i should opPlay and opNextSong, but it seems work.
        // if there is two songs in the list, it will play 2nd first.
	currentState = 1;
        free(q);

}

void get_order(char order_c)
{
	// '0' equals pause  '1' equals play
       
        if (order_c== 'd')
          order = "pig.mp3";
        if (order_c== 'e')
          order = "cat.mp3";
        if (order_c== 'f')
          order = "duck.mp3";
        if (order_c== 'a')
          order = "dog.mp3";
        if (order_c== 'b')
          order = "elephant.mp3";
        if (order_c == 'c')
          order = "sheep.mp3";
        if (order_c == '2')
          order = "stop";
        if (order_c == 'g')
          order = "turtle.mp3";
        if (order_c == 'h')
          order = "rabbit.mp3"; 
        // order = "turtle.mp3" g
}

void loop(void)
{
player.play();
	fflush(stdin);
        if (last_char != incomingByte )
	{
                order_char = incomingByte;
                get_order(order_char);
		if (order == "pause")
		{
			pause_current_song();
			Serial.print("Here comes a pause.");
		}
		if (order == "play")
		{
			continue_play_current_song();
			Serial.print("continue play the current song.");
		}
                if (order == "stop")
                {
                      player.opStop();
                      currentState = 0;
                }
		if (order != "pause" && order != "play" && order !="stop")
		{
			play_new_song(order);
			//Serial.print(order);
		}
                last_char = order_char;
	}
        delay(100);
}

void receiveEvent(int howMany)
{
  while (Wire.available()) 
  {
    // receive one byte from Master
    incomingByte = Wire.read();
    Serial.print(incomingByte);
  }  
}
