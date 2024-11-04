#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = 11;           // The on-board Arduino LED, close to PIN 13
int digitalLED = 8;           // Pin for the blue LED to indicate the setup's active state
int Threshold = 550;          // Determine which Signal to "count as a beat" and which to ignore
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
int intensity = 255; // Maximum brightness for the LEDs

int count = 0; // Counter to keep track of button presses
const int button = 12; // Pin for the button input
int val; // Variable to store the button's current state

// Custom delay function
void myDelay(unsigned long delayTime) {
  unsigned long startMillis = millis();  // get the current time

  // // Keeps checking the elapsed time
  while (millis() - startMillis < delayTime) {
    val = digitalRead(button); // // Reads the state of the button during the delay
  }
}

void setup() {   

  Serial.begin(115200);          // For Serial Monitor

  // Configures the PulseSensor object, by assigning our variables to it
  pulseSensor.analogInput(PulseWire);   
  // pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.

  pinMode(button, INPUT);   // Sets the button pin as an input
  pinMode(LED, OUTPUT);   // Sets the yellow LED pin as an output
  pinMode(digitalLED, OUTPUT);  // Sets the blue LED pin as an output
  pulseSensor.setThreshold(Threshold);   // Sets the threshold for the pulse sensor to detect a heartbeat

  // Double-check the "pulseSensor" object was created and "began" seeing a signal
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  // Prints confirmation message once when setup is complete  
  }
}

bool flag = false; // Flag to track whether the setup is active (on or off)
bool flag2 = false; // Secondary flag to ensure button presses are only registered once per press

void loop() {


  val = digitalRead(button); // Reads the current state of the button
  Serial.println(val); // Prints the button state for debugging

  // Checks if button is pressed and if it should toggle the setup's state
  if (val == 1 && count %2 == 1 && flag2 == true) {
    flag = true; // Activates the setup
    count+=1; // Increments the counter to track toggle state
    flag2 = false; // Resets flag2 to prevent multiple counts for a single press
  }

  else if (val == 1 && count %2 == 0 && flag2 == true){
    digitalWrite(digitalLED, LOW); // Turns off the blue LED when setup is deactivated
    flag = false; // Deactivates the setup
    count+=1; // Increments the counter to track toggle state
    flag2 = false; // Resets flag2 to prevent multiple counts for a single press
  }

  if (val == 0) {
    flag2 = true; // Resets flag2 when the button is released
  }
  
  // Checks if the setup is active
  if (flag == true){ 
    digitalWrite(digitalLED, HIGH); // Turns on the blue LED to indicate active state
    if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
      int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                                    // "myBPM" hold this BPM value now. 
      Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
      Serial.print("BPM: ");                        // Print phrase "BPM: " 
      Serial.println(myBPM);                        // Print the value inside of myBPM. 
      int val = map(myBPM, 0, 180,0, 255);          // Maps the BPM value to an LED brightness level
      Serial.print("val:");
      Serial.println(val);                          // Prints the mapped brightness value
      analogWrite(LED, val);                        // Sets the brightness of the yellow LED based on the BPM
      myDelay(100);                                 // Waits for 100 milliseconds
      analogWrite(LED, 0);                          // Turns off the yellow LED to create a pulsing effect
    }
  }
  
  myDelay(100); // Adds a 100ms delay at the end of each loop iteration.

}