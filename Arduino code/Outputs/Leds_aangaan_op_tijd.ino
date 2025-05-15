// --- LED-instellingen ---
const int numLEDs = 7;                            // Aantal LEDs
const int ledPins[numLEDs] = {4, 5, 6, 7, 8, 9, 10}; // De digitale pinnen waarop de LEDs zijn aangesloten

// --- Tijdsinstelling ---
unsigned long max_time = 100;                     // Totale tijd waarin alle LEDs aan moeten gaan (in milliseconden)

void setup() {
  // Zet alle LED-pinnen als output en zet ze uit
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  LED_SCORE(); // Voer het LED-patroon continu uit
}

void LED_SCORE() {
  static unsigned long interval = max_time / numLEDs; // Tijd tussen elke LED
  static unsigned long previousMillis = 0;            // Tijd van de vorige LED-actie
  static unsigned long ledOnTimes[numLEDs] = {0};     // Opslaan wanneer elke LED aan ging
  static bool ledStates[numLEDs] = {false};           // Opslaan of een LED aan is
  static int currentLED = 0;                          // Index van de huidige LED

  unsigned long currentMillis = millis();             // Huidige tijd in milliseconden

  // --- Zet de volgende LED aan ---
  if (currentLED < numLEDs && (currentMillis - previousMillis >= interval)) {
    digitalWrite(ledPins[currentLED], HIGH);          // Zet de huidige LED aan
    ledOnTimes[currentLED] = currentMillis;           // Sla de tijd op
    ledStates[currentLED] = true;                     // Markeer deze LED als 'aan'
    previousMillis = currentMillis;                   // Update de tijd
    currentLED++;                                     // Ga naar de volgende LED
  }

  // --- Reset alles nadat de laatste LED klaar is ---
  if (currentLED >= numLEDs && (currentMillis - ledOnTimes[numLEDs - 1]) >= interval) {
    for (int i = 0; i < numLEDs; i++) {
      digitalWrite(ledPins[i], LOW);                  // Zet alle LEDs uit
      ledStates[i] = false;
      ledOnTimes[i] = 0;
    }
    currentLED = 0;                                   // Begin opnieuw
    previousMillis = currentMillis;
  }
}
