
#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library

SdFat sd; // Create object to handle SD functions

SFEMP3Shield MP3player; // Create Mp3 library object
const uint8_t volume = 0; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 3;  // Mono setting 0=off, 3=max

int AnzahlTitel = 0;

void setup()
{
  Serial.begin(115200);
  
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
  
  readFiles();
  Serial.println(AnzahlTitel);
  MP3player.stopTrack();
}

// All the loop does is continuously step through the trigger
//  pins to see if one is pulled low. If it is, it'll stop any
//  currently playing track, and start playing a new one.
void loop()
{
  if (!MP3player.isPlaying()){
      MP3player.available();
      PlayFile(11);
  }
    switch (MP3player.getState()) {
    case uninitialized:
      Serial.print(F("uninitialized"));
      break;
    case initialized:
      Serial.print(F("initialized"));
      break;
    case deactivated:
      Serial.print(F("deactivated"));
      break;
    case loading:
      Serial.print(F("loading"));
      break;
    case ready:
      Serial.print(F("ready"));
      break;
    case playback:
      Serial.print(F("playback"));
      break;
    case paused_playback:
      Serial.print(F("paused_playback"));
      break;
    case testing_memory:
      Serial.print(F("testing_memory"));
      break;
    case testing_sinewave:
      Serial.print(F("testing_sinewave"));
      break;
    }
    Serial.println();
    delay(1000);

      /* If another track is playing, stop it: */
//      if (MP3player.isPlaying())
//        MP3player.stopTrack();

}

// initSD() initializes the SD card and checks for an error.
void initSD()
{
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) 
    sd.initErrorHalt();
  if(!sd.chdir("/")) 
    sd.errorHalt("sd.chdir");
}

// initMP3Player() sets up all of the initialization for the
// MP3 Player Shield. It runs the begin() function, checks
// for errors, applies a patch if found, and sets the volume/
// stero mode.
void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  if(result != 0) // check result, see readme for error codes.
  {
    // Error checking can go here!
  }
  MP3player.setVolume(volume, volume);
  MP3player.setMonoMode(monoMode);
}

void readFiles()
{
  SdFile file;
  char filename[13];
  sd.chdir("/",true);
  while (file.openNext(sd.vwd(),O_READ))
  {
    file.getFilename(filename);
    if ( isFnMusic(filename) ) {
      Serial.print(F("Index "));
      Serial.print(AnzahlTitel);
      Serial.print(F(": "));
      Serial.println(filename);
      AnzahlTitel++;
    } // if isFnMusic
    file.close();
  }//while end
}

void PlayFile(int number)
{
  SdFile file;
  char filename[13];
  sd.chdir("/",true);
  int count=0;
  while (file.openNext(sd.vwd(),O_READ))
  {
    file.getFilename(filename);
    if ( isFnMusic(filename) ) {
      if (count==number){
        Serial.println(filename);
        int8_t result = MP3player.playMP3(filename,0);
        //check result, see readme for error codes.
        if(result != 0) {
          Serial.print(F("Error code: "));
          Serial.print(result);
          Serial.println(F(" when trying to play track"));
        }
      }
      count++;
    } // if isFnMusic
    file.close();
  }//while end
}
