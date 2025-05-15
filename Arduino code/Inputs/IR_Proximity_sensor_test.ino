// --- Pin- en hardwaredefinities ---
const int IRSensor = 2;     // IR-sensor aangesloten op digitale pin D2
const int LED = 13;         // Onboard LED op pin 13

// --- Fysische constanten (voor latere berekeningen indien nodig) ---
const float m = 0.05;       // Massa in kg (niet gebruikt in deze code, maar voorbereid)
const float g = 9.81;       // Zwaartekrachtversnelling in m/s²

// --- Variabelen voor tijdsmeting ---
unsigned long startTime = 0;     // Tijdstip wanneer het object de sensor passeert (start)
unsigned long endTime = 0;       // Tijdstip wanneer het object opnieuw passeert (stop)
bool timerRunning = false;       // Geeft aan of de timer actief is
int lastSensorState = HIGH;      // Vorige toestand van de IR-sensor (om veranderingen te detecteren)

void setup() {
  Serial.begin(115200);          // Start seriële communicatie voor debug
  pinMode(IRSensor, INPUT);      // Zet IR-sensor pin als input
  pinMode(LED, OUTPUT);          // Zet LED pin als output
}

void loop() {
  int sensorStatus = digitalRead(IRSensor);  // Lees de huidige status van de IR-sensor

  // Zet de LED uit als standaard
  digitalWrite(LED, LOW);

  // Detecteer overgang van object (van HIGH naar LOW)
  if (sensorStatus == LOW && lastSensorState == HIGH) {
    if (!timerRunning) {
      // Eerste detectie → start timer
      startTime = millis();
      timerRunning = true;
      Serial.println("Timer gestart.");
    } else {
      // Tweede detectie → stop timer en bereken tijdsduur
      endTime = millis();
      timerRunning = false;

      // Bereken tijd in milliseconden en seconden
      unsigned long duration = endTime - startTime;
      float t = duration / 1000.0;

      // Toon resultaten via seriële monitor for better testing
      Serial.print("Timer gestopt. Tijd: ");
      Serial.print(duration);
      Serial.println(" ms");

      Serial.print("Tijd: ");
      Serial.print(t);
      Serial.println(" s");
    }

    // Debounce delay: voorkomt dubbele metingen
    delay(300);
  }

  // Sla huidige sensorstatus op voor volgende vergelijking
  lastSensorState = sensorStatus;
}
