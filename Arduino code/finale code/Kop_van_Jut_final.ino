const int IRSensor = 2;
const int piezoPin = A0;
const int solenoidPin = 11;

const int numLEDs = 7;
const int ledPins[numLEDs] = {4, 5, 6, 7, 8, 9, 10};

unsigned long startTime = 0;
unsigned long endTime = 0;
bool timerRunning = false;
int lastSensorState = HIGH;

unsigned long lastTikTime = 0;
const unsigned long minTimeBetweenTiks = 5000;

float tMax = 750;  // ms
unsigned long interval = tMax / numLEDs;

unsigned long previousMillis = 0;
unsigned long ledOnTimes[numLEDs] = {0};
bool ledStates[numLEDs] = {false};
int currentLED = 0;

const int PIEZO_THRESHOLD = 30;

void setup() {
  Serial.begin(115200);
  pinMode(IRSensor, INPUT);
  pinMode(solenoidPin, OUTPUT);
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  int sensorStatus = digitalRead(IRSensor);

  // Start/stop timer
  if (sensorStatus == LOW && lastSensorState == HIGH) {
    if (!timerRunning) {
      startTime = millis();
      timerRunning = true;
      Serial.println("Timer gestart.");
      resetLEDs(); // Reset bij nieuwe start
      LED_SCORE();
    } else {
      endTime = millis();
      timerRunning = false;

      unsigned long duration = endTime - startTime;
      float t = duration / 1000.0;

      Serial.print("Timer gestopt. Tijd: ");
      Serial.print(duration);
      Serial.println(" ms");

      Serial.print("Tijd: ");
      Serial.print(t);
      Serial.println(" s");
      delay(300); // Debounce
    }
  }

  lastSensorState = sensorStatus;

  // LED-timing
  if (timerRunning) {
    LED_SCORE();
  }

  // Piezo check
  if (timerRunning) {
    int piezoValue = analogRead(piezoPin);
    if (piezoValue > PIEZO_THRESHOLD) {
      unsigned long now = millis();
      if (now - lastTikTime > minTimeBetweenTiks) {
        Serial.print("Impact gedetecteerd! Waarde: ");
        Serial.println(piezoValue);
        BEL();  // Bel laten rinkelen
        lastTikTime = now;

        // Zet tijdelijk alle LEDs aan
        for (int i = 0; i < numLEDs; i++) {
          digitalWrite(ledPins[i], HIGH);
          ledStates[i] = true;
        }

        delay(500);
        resetLEDs();
      }
    }
  }

  delay(10);
}

void LED_SCORE() {
  unsigned long currentMillis = millis();

  if (currentLED < numLEDs && (currentMillis - previousMillis >= interval)) {
    digitalWrite(ledPins[currentLED], HIGH);
    ledOnTimes[currentLED] = currentMillis;
    ledStates[currentLED] = true;
    previousMillis = currentMillis;
    currentLED++;
  }
}

void resetLEDs() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
    ledStates[i] = false;
    ledOnTimes[i] = 0;
  }
  currentLED = 0;
  previousMillis = millis();
}

void BEL() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(solenoidPin, HIGH);
    delay(500);
    digitalWrite(solenoidPin, LOW);
    delay(500);
  }
}
