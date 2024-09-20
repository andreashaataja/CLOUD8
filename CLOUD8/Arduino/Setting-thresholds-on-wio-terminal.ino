#include "DHT.h"
#include "Seeed_LCD.h"
#include "Seeed_touchScreen.h"

#define DHTPIN 5   // Pin for the DHT sensor
#define DHTTYPE DHT11 // DHT 11 sensor type

// Create an instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Define threshold limits
float tempHigh = 30.0; // Temperature threshold in Celsius
float tempLow = 15.0;  // Temperature threshold in Celsius
float humidityHigh = 70.0; // Humidity threshold in percentage
float humidityLow = 30.0;  // Humidity threshold in percentage

void setup() {
  Serial.begin(9600);  // Start the serial communication
  dht.begin();         // Initialize the DHT sensor
  tft.begin();         // Initialize the Wio Terminal display
  pinMode(BUZZER, OUTPUT);  // Set the buzzer pin as output
  tft.setTextSize(2);  // Set text size for the display
}

void loop() {
  // Read the temperature and humidity from the DHT sensor
  float temperature = dht.readTemperature(); // Reading temperature in Celsius
  float humidity = dht.readHumidity();       // Reading humidity in percentage

  // Display temperature and humidity on the Wio Terminal
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 10);
  tft.print("Temp: ");
  tft.print(temperature);
  tft.print(" C");

  tft.setCursor(10, 40);
  tft.print("Humidity: ");
  tft.print(humidity);
  tft.print(" %");

  // Check if temperature exceeds thresholds
  if (temperature > tempHigh || temperature < tempLow) {
    buzzAlert(); // Trigger the alert if temperature is out of range
    tft.setTextColor(TFT_RED);
    tft.setCursor(10, 70);
    tft.print("Temperature Alert!");
  }

  // Check if humidity exceeds thresholds
  if (humidity > humidityHigh || humidity < humidityLow) {
    buzzAlert(); // Trigger the alert if humidity is out of range
    tft.setTextColor(TFT_RED);
    tft.setCursor(10, 100);
    tft.print("Humidity Alert!");
  }

  delay(2000); // Delay for 2 seconds before re-checking
}

// Function to trigger the buzzer for alerts
void buzzAlert() {
  digitalWrite(BUZZER, HIGH); // Turn on the buzzer
  delay(500);                 // Buzz for 0.5 seconds
  digitalWrite(BUZZER, LOW);  // Turn off the buzzer
}

