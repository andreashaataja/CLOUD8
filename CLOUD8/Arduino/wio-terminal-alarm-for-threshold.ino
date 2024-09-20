// Function to play tone on the buzzer
void playBuzzer() {
  tone(BUZZER_PIN, 1000); // Generate a tone of 1000Hz on the buzzer pin
  delay(buzzerDuration * 1000); // Keep the tone playing for the specified duration
  noTone(BUZZER_PIN); 
}

void setup() {

  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as output
}

void loop() {
  // If thresholds are met and enough time has passed since the last buzzer alert
  if ((temperatureThresholdMet || humidityThresholdMet) && (millis() - lastBuzzerTime) >= (buzzerInterval * 1000)) {
    // Activate the buzzer for the specified duration
    playBuzzer();

    // Update the last buzzer alert time
    lastBuzzerTime = millis();
  }

}
