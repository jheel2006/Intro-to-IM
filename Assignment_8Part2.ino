int ledPin = 9; // PWM pin connected to LED

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int bright;

  if (Serial.available() > 0) {
    bright = Serial.parseInt();
    analogWrite(ledPin, bright);
  }
}
