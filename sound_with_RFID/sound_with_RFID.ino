/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/* 
This library works with the Adafruit NFC breakout (in SPI mode)
  ----> https://www.adafruit.com/products/364  
*/
 

//LIBRARIES===========================================================

//SOUND---------------------------------------------------------------
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

//RFID ---------------------------------------------------------------
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

//PINS================================================================

//SOUND---------------------------------------------------------------
// define the pins used for sound
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example (sound)
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield (sound)
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

//RFID ---------------------------------------------------------------
#define PN532_SCK  (2)
#define PN532_MOSI (6)
#define PN532_SS   (7)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (A4)
#define PN532_RESET (A5)  // Not connected by default on the NFC Shield


//INITIALIZE===============================================================

//SOUND---------------------------------------------------------------
Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  //Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

//RFID ---------------------------------------------------------------
// Use this line for a breakout with a software SPI connection (recommended):
 Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif


void setup() {

//RFID ---------------------------------------------------------------
 #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin(); 

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");

//SOUND---------------------------------------------------------------
 
// original sound data Serial.begin(9600);
//  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
//  Serial.println(F("Playing track 001"));
//  musicPlayer.playFullFile("track001.mp3");
  // Play another file in the background, REQUIRES interrupts!

  
}

void loop() {

//RFID ---------------------------------------------------------------
  uint8_t success;
  uint8_t list;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
   success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
      if (uid[0] == 146) {
        Serial.println("Found card 146 - Golden Gate Park");
        musicPlayer.playFullFile("track002.mp3");
      }
      if (uid[0] == 28) {
        Serial.println("Found card 28 - Playing beach");
        musicPlayer.playFullFile("track003.mp3");
      }     
      if (uid[0] == 236) {
        Serial.println("Found card 236");
      }
      if (uid[0] == 60) {
        Serial.println("Found card 60");
      }  
       if (uid[0] == 34) {
        Serial.println("Found card 34");
      }
  }
  delay(1000);

//SOUND ---------------------------------------------------------------

  // File is playing in the background
  if (musicPlayer.stopped()) {
    Serial.println("Done playing music");  
    
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
    if (success) {
      if (uid[0] == 146) {
        Serial.println("Found card 146 - Golden Gate Park");
        musicPlayer.playFullFile("track002.mp3");
      }
      if (uid[0] == 28) {
        Serial.println("Found card 28 - Playing beach");
        musicPlayer.playFullFile("track003.mp3");
      }     
      if (uid[0] == 236) {
        Serial.println("Found card 236");
      }
      if (uid[0] == 60) {
        Serial.println("Found card 60");
      }  
       if (uid[0] == 34) {
        Serial.println("Found card 34");
      }
  }
  delay(1000);
    while (1);
  }
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }

  delay(100);
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

