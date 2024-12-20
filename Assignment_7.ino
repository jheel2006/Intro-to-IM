/*************************************************** 
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin


const int sensorPin = A0;  // Analog input pin connected to the voltage divider
int sensorValue = 0;       // Variable to store the analog reading
bool soundOn = false;



// Define song list
const char *songs[] = {"/track001.mp3", "/track002.mp3", "/track003.mp3"};
int songIndex = 0; // Current song index
int lastButtonState = 0;

// int count = 0; // Counter to keep track of button presses
const int buttonPin = 2;
bool buttonPressed = false;
int buttonState;

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


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


void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  pinMode(buttonPin, INPUT);
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
  musicPlayer.setVolume(5,5);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
  musicPlayer.startPlayingFile("/track001.mp3");
  // Play another file in the background, REQUIRES interrupts!
  // Serial.println(F("Playing track 002"));
  // musicPlayer.startPlayingFile("/track002.mp3");
}



void loop() {
  sensorValue = analogRead(sensorPin);  // Read the analog input
  Serial.print("Light Level: ");
  Serial.println(sensorValue);          // Print the light level (0-1023)
  if (sensorValue > 50) {
    musicPlayer.pausePlaying(false);
    soundOn = true;
  }
  else {
    musicPlayer.pausePlaying(true);
    soundOn = false;
  }
  // Serial.println(soundOn);
   

  // Check if button was pressed
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState); // Prints the button state for debugging
  Serial.println(songs[songIndex]);

  if (buttonState == 1 && lastButtonState == 0) {  // Button press detected
    // musicPlayer.pausePlaying(true);
    musicPlayer.stopPlaying();
    delay(50);  // Debounce delay
    songIndex = (songIndex + 1) % 3;  // Move to the next song in the array
    musicPlayer.startPlayingFile(songs[songIndex]);  // Play the next song
    // Serial.println(songs[songIndex]);
    
  }
  lastButtonState = buttonState;

  // File is playing in the background
  if (musicPlayer.stopped()) {
    musicPlayer.startPlayingFile(songs[songIndex]);
  }

  if (Serial.available()) {
    char c = Serial.read();
    
  }

  delay(100);
}





