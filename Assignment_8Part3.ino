int ledPin = 13; // LED pin#
int sensorPin = A0;
int windValue;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  windValue = analogRead(sensorPin); // Read the sensor value
  Serial.println(windValue); // Send the value to the serial port
  // Serial.write(windValue);
  

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); // Read data until newline
    if (data == "bounce") {
      digitalWrite(ledPin, HIGH);
      delay(100); // Keep the LED on for 100ms
      digitalWrite(ledPin, LOW);
    }
  }
}
