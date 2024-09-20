#include "TFT_eSPI.h" // TFT LCD library
#include "DHT.h" // DHT sensor library

#define DHTPIN 0 // Signal pin for DHT sensor
#define DHTTYPE DHT11 // Type of DHT sensor

TFT_eSPI tft; // Initialize TFT LCD
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// Temperature and humidity thresholds
int temperatureThresholdHigh = 30;
int temperatureThresholdLow = 20;
int humidityThresholdHigh = 70;
int humidityThresholdLow = 30;

void setup() {
    tft.begin(); // Start TFT LCD
    dht.begin(); // Start DHT sensor
    tft.setRotation(3); // Set TFT LCD orientation
}

void loop() {
    int temperature = dht.readTemperature(); // Read temperature
    int humidity = dht.readHumidity(); // Read humidity

    uint16_t backgroundColor = TFT_WHITE; // Default background color

    // Change background color based on temperature thresholds
    if (temperature > temperatureThresholdHigh) {
        backgroundColor = TFT_RED; // High temperature
    } else if (temperature < temperatureThresholdLow) {
        backgroundColor = TFT_BLUE; // Low temperature
    }

    // Change background color based on humidity thresholds
    if (humidity > humidityThresholdHigh) {
        backgroundColor = TFT_YELLOW; // High humidity
    } else if (humidity < humidityThresholdLow) {
        backgroundColor = TFT_GREEN; // Low humidity
    }

    // Apply the background color to the TFT screen
    tft.fillScreen(backgroundColor); // Fill screen with the selected color

    delay(1000); // Delay for a short interval
}
