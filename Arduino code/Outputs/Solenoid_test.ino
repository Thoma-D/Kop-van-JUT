const int relaisPin = 8; // IN-pin van relaismodule

void setup() {
  pinMode(relaisPin, OUTPUT);
  digitalWrite(relaisPin, LOW); // Zorg dat relais uitstaat bij start
}

void loop() {
  // Zet solenoïde aan
  digitalWrite(relaisPin, HIGH); // Relais schakelt, solenoïde krijgt stroom
  delay(500);                    // 0.5 seconde aan

  // Zet solenoïde uit
  digitalWrite(relaisPin, LOW);  // Relais verbreekt, solenoïde uit
  delay(500);                    // 0.5 seconde uit
}
