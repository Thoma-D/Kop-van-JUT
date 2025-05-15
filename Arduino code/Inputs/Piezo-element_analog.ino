const int PIEZO_PIN = A0; // Piezo output
int nummer=0;
void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  float piezoADC = analogRead(PIEZO_PIN);
  if (piezoADC>nummer){
    nummer=piezoADC;
    Serial.println(nummer); // Print the voltage.
  }
  if (piezoADC >= 30) {
    Serial.println("gelukt");
    delay(100);
  }
}