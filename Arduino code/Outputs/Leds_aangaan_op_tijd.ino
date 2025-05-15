const int numLEDs = 7; // Aantal LEDs
const int ledPins[numLEDs] = {4, 5, 6, 7, 8, 9, 10}; // Pin nummers voor de LEDs

unsigned long max_time = 100; // Totale tijd in milliseconden (bv. 7000 ms = 7 sec)

void setup() {
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  LED_SCORE();
}
void LED_SCORE() {
  static unsigned long interval = max_time / numLEDs;
  static unsigned long previousMillis = 0;
  static unsigned long ledOnTimes[numLEDs] = {0};
  static bool ledStates[numLEDs] = {false};
  static int currentLED = 0;

  unsigned long currentMillis = millis();

  // Zet volgende LED aan
  if (currentLED < numLEDs && (currentMillis - previousMillis >= interval)) {
    digitalWrite(ledPins[currentLED], HIGH);
    ledOnTimes[currentLED] = currentMillis;
    ledStates[currentLED] = true;
    previousMillis = currentMillis;
    currentLED++;
  }

  // Reset na laatste LED
  if (currentLED >= numLEDs && (currentMillis - ledOnTimes[numLEDs - 1]) >= interval) {
    for (int i = 0; i < numLEDs; i++) {
      digitalWrite(ledPins[i], LOW);
      ledStates[i] = false;
      ledOnTimes[i] = 0;
    }
    currentLED = 0;
    previousMillis = currentMillis;
  }
}
