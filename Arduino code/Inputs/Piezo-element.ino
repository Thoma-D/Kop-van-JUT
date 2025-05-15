const int piezoPin = 3;
unsigned long laatsteTikTijd = 0;
const unsigned long minimumTijdTussenTikken = 2.000; // in milliseconden

void setup() {
  pinMode(piezoPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(piezoPin) == HIGH) {
    unsigned long huidigeTijd = millis();
    if (huidigeTijd - laatsteTikTijd > minimumTijdTussenTikken) {
      Serial.println("Impact gedetecteerd!");
      laatsteTikTijd = huidigeTijd;
    }
  }
}

