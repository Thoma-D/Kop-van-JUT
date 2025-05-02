const int IRSensor = 2;     // IR sensor connected to pin D2
const int LED = 13;         // Onboard LED

// Bewerking variabelen
const float m = 0.05;       // Massa in kg
const float g = 9.81;       // Gravitatie in m/s²

// Timer variabelen
unsigned long startTime = 0;
unsigned long endTime = 0;
bool timerRunning = false;
int lastSensorState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(IRSensor, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  int sensorStatus = digitalRead(IRSensor);
  digitalWrite(LED, LOW); // LED altijd uit bij geen detectie

  if (sensorStatus == LOW && lastSensorState == HIGH) {
    if (!timerRunning) {
      startTime = millis();
      timerRunning = true;
      Serial.println("Timer gestart.");
    } else {
      endTime = millis();
      timerRunning = false;

      unsigned long duration = endTime - startTime;
      float t = duration / 1000.0;  // Tijd in seconden

      Serial.print("Timer gestopt. Tijd: ");
      Serial.print(duration);
      Serial.println(" ms");

      Serial.print("Tijd: ");
      Serial.print(t);
      Serial.println(" s");

      berekeningen(t);
    }
    delay(300); // Debounce
  }

  lastSensorState = sensorStatus;
}

void berekeningen(float t) {
  float v = g * (t/2);  
  float x = v * t - 0.5 * g * (t/2) * (t/2);   // Hoogte
               // Snelheid
  float f = (0.5 * m * v * v) / x;     // Kracht via Ekin / afstand

  Serial.print("Hoogte: ");
  Serial.print(x);
  Serial.println(" m");

  Serial.print("Snelheid: ");
  Serial.print(v);
  Serial.println(" m/s");

  Serial.print("Kracht: ");
  Serial.print(f);
  Serial.println(" N");
}