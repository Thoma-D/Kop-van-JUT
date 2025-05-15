// --- Pin- en hardwaredefinities ---
const int IRSensor = 2;                // Infrarood sensor voor bewegingsdetectie
const int piezoPin = A0;               // Piezo sensor op analoge pin A0 voor impactdetectie
const int solenoidPin = 11;            // Solenoïde pin voor de bel (output)

const int numLEDs = 7;                 // Aantal LEDs in score-indicator
const int ledPins[numLEDs] = {4, 5, 6, 7, 8, 9, 10}; // Pins voor de LEDs

// --- Timer voor scoremeting ---
unsigned long startTime = 0;           // Starttijd van de meting
unsigned long endTime = 0;             // Eindtijd van de meting
bool timerRunning = false;             // Status van de timer (loopt of niet)
int lastSensorState = HIGH;            // Vorige status van de IR-sensor

// --- Timer voor impact tussenpauze ---
unsigned long lastTikTime = 0;                         // Laatste impacttijd
const unsigned long minTimeBetweenTiks = 5000;         // Minimale tijd tussen tikken in ms

// --- LED timing parameters ---
float tMax = 750;                       // Totale tijd waarin alle LEDs oplichten (ms)
unsigned long interval = tMax / numLEDs; // Interval tussen het aanzetten van elke LED

unsigned long previousMillis = 0;       // Tijd van laatste LED-activatie
unsigned long ledOnTimes[numLEDs] = {0}; // Tijdstip waarop elke LED aan ging
bool ledStates[numLEDs] = {false};       // Status (aan/uit) van elke LED
int currentLED = 0;                      // Index van de huidige LED die geactiveerd wordt

const int PIEZO_THRESHOLD = 30;         // Drempelwaarde voor impactdetectie

void setup() {
  Serial.begin(115200);                 // Start seriële monitor
  pinMode(IRSensor, INPUT);            // Zet IR-sensor als input
  pinMode(solenoidPin, OUTPUT);        // Zet solenoïde pin als output

  // Stel alle LEDs in als output en zet ze uit
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  int sensorStatus = digitalRead(IRSensor); // Lees IR-sensorstatus

  // Start of stop timer op basis van sensorverandering
  if (sensorStatus == LOW && lastSensorState == HIGH) {
    if (!timerRunning) {
      // Start de timer
      startTime = millis();
      timerRunning = true;
      Serial.println("Timer gestart.");
      resetLEDs();     // Begin met schone LED-status
      LED_SCORE();     // Start met LED-looptiming
    } else {
      // Stop de timer
      endTime = millis();
      timerRunning = false;

      // Bereken tijdsduur
      unsigned long duration = endTime - startTime;
      float t = duration / 1000.0;

      Serial.print("Timer gestopt. Tijd: ");
      Serial.print(duration);
      Serial.println(" ms");

      Serial.print("Tijd: ");
      Serial.print(t);
      Serial.println(" s");

      delay(300); // Kleine debounce om dubbeltrigger te vermijden
    }
  }

  lastSensorState = sensorStatus; // Update vorige sensorstatus

  // Als de timer loopt, activeer LEDs op basis van tijd
  if (timerRunning) {
    LED_SCORE();
  }

  // Impactdetectie tijdens actieve sessie
  if (timerRunning) {
    int piezoValue = analogRead(piezoPin); // Lees piezo-waarde
    if (piezoValue > PIEZO_THRESHOLD) {
      unsigned long now = millis();

      // Controleer of er genoeg tijd is verstreken sinds vorige impact
      if (now - lastTikTime > minTimeBetweenTiks) {
        Serial.print("Impact gedetecteerd! Waarde: ");
        Serial.println(piezoValue);

        BEL();               // Laat de bel afgaan
        lastTikTime = now;   // Update laatst bekende impacttijd

        // Zet alle LEDs kortstondig aan als reactie op impact
        for (int i = 0; i < numLEDs; i++) {
          digitalWrite(ledPins[i], HIGH);
          ledStates[i] = true;
        }

        delay(500);          // Laat LEDs kort branden
        resetLEDs();         // Zet LEDs terug uit
      }
    }
  }

  delay(10); // Kleine vertraging om ruis te onderdrukken
}

// --- LED-loopsysteem dat elke LED met een interval activeert ---
void LED_SCORE() {
  unsigned long currentMillis = millis();

  if (currentLED < numLEDs && (currentMillis - previousMillis >= interval)) {
    digitalWrite(ledPins[currentLED], HIGH);   // Zet volgende LED aan
    ledOnTimes[currentLED] = currentMillis;    // Bewaar aanzetmoment
    ledStates[currentLED] = true;              // Markeer als actief
    previousMillis = currentMillis;            // Update vorige activatietijd
    currentLED++;                              // Volgende LED voorbereiden
  }
}

// --- Zet alle LEDs uit en reset statusvariabelen ---
void resetLEDs() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);   // Zet LED uit
    ledStates[i] = false;            // Status naar uit
    ledOnTimes[i] = 0;               // Reset aanzetmoment
  }
  currentLED = 0;                    // Begin opnieuw bij eerste LED
  previousMillis = millis();        // Herstart tijdsmeting
}

// --- Bel-functie: laat de solenoïde 3x tikken als signaal ---
void BEL() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(solenoidPin, HIGH); // Solenoïde aan
    delay(500);                      // Wacht 0.5s
    digitalWrite(solenoidPin, LOW);  // Solenoïde uit
    delay(500);                      // Wacht weer 0.5s
  }
}
