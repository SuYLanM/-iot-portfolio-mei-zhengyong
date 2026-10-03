const int POT_PIN = 34;
const int LED_PIN = 18;

void setup() {
  Serial.begin(115200);
  Serial.println("Simulation started");

  pinMode(LED_PIN, OUTPUT);

  // Final saved version: 10-bit ADC (0-1023)
  analogReadResolution(10);
}

void loop() {
  int adcValue = analogRead(POT_PIN);

  // Normal smooth mapping from ADC value to LED brightness
  int brightness = map(adcValue, 0, 1023, 0, 255);

  analogWrite(LED_PIN, brightness);

  Serial.print("ADC = ");
  Serial.print(adcValue);
  Serial.print("    Brightness = ");
  Serial.println(brightness);

  delay(200);
}
