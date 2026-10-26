#define NTC_PIN     34
#define RELAY_PIN   26
#define BUZZER_PIN  25

// Temperature limit
#define HIGH_TEMP   50.0

// NTC parameters
#define SERIES_RESISTOR     10000.0
#define NOMINAL_RESISTANCE  10000.0
#define NOMINAL_TEMP       25.0
#define BETA_COEFFICIENT   3950.0

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  noTone(BUZZER_PIN);

  analogReadResolution(12);
}

void loop()
{
  int adcValue = analogRead(NTC_PIN);

  // Prevent division by zero
  if (adcValue == 0)
    adcValue = 1;

  // Convert ADC value to resistance
  float resistance = SERIES_RESISTOR *
                     ((4095.0 / adcValue) - 1.0);

  // Calculate temperature
  float steinhart;

  steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);
  steinhart /= BETA_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMP + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  Serial.print("Temperature: ");
  Serial.print(steinhart);
  Serial.println(" °C");

  // HIGH TEMPERATURE
  if (steinhart >= HIGH_TEMP)
  {
    // Turn ON fan
    digitalWrite(RELAY_PIN, HIGH);

    // Buzzer ON
    tone(BUZZER_PIN, 2000);

    Serial.println("WARNING! HIGH TEMPERATURE");
    Serial.println("Fan ON");
    Serial.println("Buzzer ON");

    delay(500);

    // Buzzer OFF
    noTone(BUZZER_PIN);

    delay(500);
  }

  // NORMAL TEMPERATURE
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    noTone(BUZZER_PIN);

    Serial.println("Temperature Normal");
    Serial.println("Fan OFF");
    Serial.println("Buzzer OFF");

    delay(1000);
  }
}
