#include "TFT_eSPI.h" // TFT LCD library 
#include "DHT.h" // DHT library 

#define DHTPIN 0 // Define signal pin of DHT sensor 
#define DHTTYPE DHT11 // Define DHT sensor type 

// Thresholds for temperature and humidity
int temperatureThresholdHigh = 30; // Default high temperature threshold
int temperatureThresholdLow = 20; // Default low temperature threshold
int humidityThresholdHigh = 70; // Default high humidity threshold
int humidityThresholdLow = 30; // Default low humidity threshold

// Buzzer settings
int buzzerInterval = 60; // Default buzzer interval (1 minute) in seconds
int buzzerDuration = 5; // Default buzzer duration (5 seconds) in seconds
unsigned long lastBuzzerTime = 0; // Variable to store the last time the buzzer sounded

// Initializations
DHT dht(DHTPIN, DHTTYPE); // Initializing DHT sensor
TFT_eSPI tft; // Initializing TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); // Initializing buffer

// Macro Define
#define BUZZER_PIN WIO_BUZZER /* sig pin of the buzzer */

// Flag to track if thresholds are met for temperature and humidity
bool temperatureThresholdMet = false;
bool humidityThresholdMet = false;

void setup() {
  Serial.begin(9600); // Start serial communication

  // Delay to allow serial buffer to initialize properly
  delay(100);

  // Print each prompt message with a delay
  Serial.println("Please set temperature, humidity thresholds, and buzzer settings:");
  delay(100);

  // Set temperature and humidity thresholds
  setThresholds();

  // Set buzzer settings
  setBuzzerSettings();

  Serial.println("Settings configured successfully.");
  Serial.println("Monitoring will start shortly...");
  Serial.println();
  delay(1000);

  dht.begin(); // Start DHT sensor 
  tft.begin(); // Start TFT LCD
  tft.setRotation(3); // Set TFT LCD rotation
  spr.createSprite(TFT_HEIGHT, TFT_WIDTH); // Create buffer

  // Set buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
}


void loop() {
  int t = dht.readTemperature(); // Assign variable to store temperature 
  int h = dht.readHumidity(); // Assign variable to store humidity 

  // Background color for temperature and humidity status
  uint16_t tempBackgroundColor = TFT_WHITE;
  uint16_t humidityBackgroundColor = TFT_WHITE;

  // Check if temperature is above high or below low thresholds
  if (t > temperatureThresholdHigh) {
    // Set background color to red if temperature threshold is exceeded
    temperatureThresholdMet = true;
    tempBackgroundColor = TFT_RED;
  } else if (t < temperatureThresholdLow) {
    // Set background color to blue if temperature threshold is below
    temperatureThresholdMet = true;
    tempBackgroundColor = TFT_BLUE;
  } else {
    temperatureThresholdMet = false;
  }

  // Check if humidity is above high or below low thresholds
  if (h > humidityThresholdHigh) {
    // Set background color to yellow if humidity threshold is exceeded
    humidityThresholdMet = true;
    humidityBackgroundColor = TFT_YELLOW;
  } else if (h < humidityThresholdLow) {
    // Set background color to green if humidity threshold is below
    humidityThresholdMet = true;
    humidityBackgroundColor = TFT_GREEN;
  } else {
    humidityThresholdMet = false;
  }

  // Setting the title header 
  spr.fillSprite(TFT_WHITE); // Fill background with white color
  spr.setTextColor(TFT_BLACK); // Setting text color
  spr.setTextSize(3); // Setting text size 

  // Drawing border around the display screen
  spr.drawRect(0, 0, TFT_HEIGHT - 1, TFT_WIDTH - 1, TFT_WHITE);

  spr.drawFastVLine(150, 0, 240, TFT_DARKGREEN); // Drawing vertical line
  spr.drawFastHLine(0, 120, 320, TFT_DARKGREEN); // Drawing horizontal line

  // Setting temperature block background color
  if (temperatureThresholdMet) {
    spr.fillRect(0, 0, 150, 120, tempBackgroundColor); // Highlight temperature block
  }

  // Setting humidity block background color
  if (humidityThresholdMet) {
    spr.fillRect(0, 120, 150, 240, humidityBackgroundColor); // Highlight humidity block
  }

  // Setting temperature
  spr.setTextSize(2);
  spr.drawString("Temperature", 10, 30);
  spr.setTextSize(3);
  spr.drawNumber(t, 50, 60); // Display temperature values 
  spr.drawString("C", 90, 60);

  // Setting humidity
  spr.setTextSize(2);
  spr.drawString("Humidity", 25, 160);
  spr.setTextSize(3);
  spr.drawNumber(h, 30, 190); // Display humidity values 
  spr.drawString("%RH", 70, 190);

  // Push sprite to LCD
  spr.pushSprite(0, 0); // Push to LCD

  // If thresholds are met and enough time has passed since the last buzzer alert
  if ((temperatureThresholdMet || humidityThresholdMet) && (millis() - lastBuzzerTime) >= (buzzerInterval * 1000)) {
    // Activate the buzzer for the specified duration
    playTone(1000, buzzerDuration * 1000); // Convert seconds to milliseconds

    // Update the last buzzer alert time
    lastBuzzerTime = millis();
  }

  delay(50); // Delay for stability
}

// Play tone
void playTone(int tone, int duration) {
  unsigned long startTime = millis(); // Get the current time

  while (millis() - startTime < duration) { // Loop until the duration has elapsed
    digitalWrite(BUZZER_PIN, HIGH); // Turn the buzzer on
    delayMicroseconds(tone); // Wait for the specified tone duration
    digitalWrite(BUZZER_PIN, LOW); // Turn the buzzer off
    delayMicroseconds(tone); // Wait for the same duration to complete the cycle
  }
}

// Function to set temperature and humidity thresholds
void setThresholds() {
  // Prompt for low temperature threshold
  Serial.println("Enter low temperature threshold (in Celsius):");
  temperatureThresholdLow = readValue(temperatureThresholdLow);
  Serial.print("Low Temperature Threshold set to: ");
  Serial.println(temperatureThresholdLow);

  // Prompt for high temperature threshold
  Serial.println("Enter high temperature threshold (in Celsius):");
  temperatureThresholdHigh = readValue(temperatureThresholdHigh);
  Serial.print("High Temperature Threshold set to: ");
  Serial.println(temperatureThresholdHigh);

  // Prompt for high humidity threshold
  Serial.println("Enter high humidity threshold (in percentage):");
  humidityThresholdHigh = readValue(humidityThresholdHigh);
  Serial.print("High Humidity Threshold set to: ");
  Serial.println(humidityThresholdHigh);

  // Prompt for low humidity threshold
  Serial.println("Enter low humidity threshold (in percentage):");
  humidityThresholdLow = readValue(humidityThresholdLow);
  Serial.print("Low Humidity Threshold set to: ");
  Serial.println(humidityThresholdLow);
}

// Function to set buzzer settings
void setBuzzerSettings() {
  // Prompt for buzzer duration
  Serial.println("Enter buzzer duration (in seconds):");
  buzzerDuration = readValue(buzzerDuration);
  Serial.print("Buzzer Duration set to: ");
  Serial.println(buzzerDuration);

  // Prompt for buzzer interval
  Serial.println("Enter buzzer interval (in seconds):");
  buzzerInterval = readValue(buzzerInterval);
  Serial.print("Buzzer Interval set to: ");
  Serial.println(buzzerInterval);
}

// Function to read input value from serial with validation
int readValue(int defaultValue) {
  while (!Serial.available()) {}
  String input = Serial.readStringUntil('\n');
  if (input.length() == 0) {
    return defaultValue; // Return default value if no input
  }
  for (size_t i = 0; i < input.length(); i++) {
    if (!isdigit(input.charAt(i))) {
      Serial.println("Invalid input. Please enter a numeric value.");
      return readValue(defaultValue); // Retry input
    }
  }
  return input.toInt(); // Convert input string to integer
}
