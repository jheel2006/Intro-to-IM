void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

    // start the handshake
//   while (Serial.available() <= 0) {
//     digitalWrite( , HIGH); // on/blink while waiting for serial data
//     Serial.println("0,0"); // send a starting message
//     delay(300);            // wait 1/3 second
//     digitalWrite( , LOW);
//     delay(50);
//   }
// }
// }

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead (A2);
  Serial.println (sensorValue);
  Serial.write(sensorValue);
  delay (1);
}