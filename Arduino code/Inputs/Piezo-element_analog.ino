// --- Pin- en variabeleninstellingen ---
const int PIEZO_PIN = A0;   // De piezosensor is aangesloten op analoge pin A0
int nummer = 0;             // Variabele om de hoogste gemeten waarde op te slaan

void setup() {
  Serial.begin(115200);     // Start seriële communicatie met 115200 baud voor debug
}

void loop() {
  // Lees de analoge waarde van de piezo in (0 tot 1023)
  float piezoADC = analogRead(PIEZO_PIN);

  // Controleer of deze nieuwe waarde hoger is dan de vorige hoogste waarde
  if (piezoADC > nummer) {
    nummer = piezoADC;          // Update de hoogste waarde
    Serial.println(nummer);     // Print de nieuwe hoogste waarde
  }

  // Controleer of de piezo-waarde boven de drempel van 30 komt
  if (piezoADC >= 30) {
    Serial.println("gelukt");   // Print een succesbericht
    delay(100);                 // Korte vertraging om herhaling te voorkomen (debounce)
  }
}
