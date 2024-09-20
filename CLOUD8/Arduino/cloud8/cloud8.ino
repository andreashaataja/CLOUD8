#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "TFT_eSPI.h" //TFT LCD library 
#include "DHT.h" // DHT library 
#include "Secrets.h"

//Definitions temp/humi
#define DHTPIN 0 //Define signal pin of DHT sensor 
// #define DHTPIN PIN_WIRE_SCL //Use I2C port as Digital Port */
#define DHTTYPE DHT11 //Define DHT sensor type
// Macro Define
#define BUZZER_PIN WIO_BUZZER /* sig pin of the buzzer */
#define LEDPIN 4 // Define pin for led light (Conection schema: black-pin6, red-pin2, yellow-pin22(D4))

//Initializations
DHT dht(DHTPIN, DHTTYPE); //Initializing DHT sensor
TFT_eSPI tft; //Initializing TFT LCD

// Background color for temperature and humidity status
uint16_t tempBackgroundColor = TFT_BLUE;
uint16_t humidityBackgroundColor = TFT_BLUE;

String disabilityMode = ""; // To store incoming disability mode message

// Thresholds for temperature and humidity
int temperatureThresholdHigh = 25; // Default high temperature threshold
int temperatureThresholdLow = 20; // Default low temperature threshold
int humidityThresholdHigh = 70; // Default high humidity threshold
int humidityThresholdLow = 30; // Default low humidity threshold

// Buzzer settings
int buzzerInterval = 60; // Default buzzer interval (1 minute) in seconds
int buzzerDuration = 5; // Default buzzer duration (5 seconds) in seconds
unsigned long lastBuzzerTime = 0; // Variable to store the last time the buzzer sounded

// Flag to track if thresholds are met for temperature and humidity
bool temperatureThresholdMet = false;
bool humidityThresholdMet = false;

const char *ssid = WIFI_NAME;       // These values come from your Secrets.h file.
const char *password = WIFI_PW;     // Make sure that you have the file in Arduino/cloud8/ folder.

const char *ID = CLIENT_ID;     // changed to get info from Secrets.h as to avoid using the same ID when connecting.
const char *TOPIC = "CLOUD8";  // Topic to subcribe to
const char *subTopicOne = "temperature";  // Topic to subcribe to
const char *subTopicTwo = "humidity";
const char *server = CLOUD8_SERVER; // gets it from secrets.h

// Different color schemes and variables to hold information on chosen scheme.
String currentColorScheme; // for holding the string from the broker
uint32_t alarmColor; // saving what color the alert color should be
uint32_t currentColorMode[4]; // will be assigned default if no other scheme is chosen
const uint32_t defaultColorScheme[4] = {TFT_BLUE, TFT_BLACK, TFT_RED, TFT_BLACK}; // In indexing order: BG, TEXT, ALARM, LINES
const uint32_t deuteranopia[4] = {TFT_BLUE, TFT_YELLOW, TFT_MAGENTA, TFT_BLACK};
const uint32_t protanopia[4] = {TFT_BLUE, TFT_CYAN, TFT_ORANGE, TFT_WHITE};
const uint32_t tritanopia[4] {0x8800, 0xFFFF, 0x07E0, 0x0000};
const uint32_t monochromacy[4] = {TFT_BLACK, TFT_WHITE, 0x8410, TFT_WHITE};

String payloadString; // saving incoming payload string
uint8_t fontSize = 3;

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

void updatingScheme() { // method to assign currentColorMode to the chosen scheme in the app.
  for (int i = 0; i < 4; i++) {
    if (currentColorScheme.equals("Default") || currentColorScheme == NULL) {
      currentColorMode[i] = defaultColorScheme[i];

    } else if (currentColorScheme.equals("Deuteranopia")) {
      currentColorMode[i] = deuteranopia[i];

    } else if (currentColorScheme.equals("Protanopia")) {
      currentColorMode[i] = protanopia[i];

    } else if (currentColorScheme.equals("Tritanopia")) {
      currentColorMode[i] = tritanopia[i];

    } else if (currentColorScheme.equals("Monochromacy")) {
      currentColorMode[i] = monochromacy[i];
    }
  } 
}

void callback(char* TOPIC, byte* payload, unsigned int length) { // method for handling sub messages
  
  char tempPayloadHolder[length + 1];                          // for holding charArray of payload.
  payload[length] = '\0';                                  // https://forum.arduino.cc/t/trying-to-find-a-solution-for-converting-byte-to-char/221541
  String payloadMsg = String((char*)payload);              // casting payload to String.
  payloadMsg.toCharArray(tempPayloadHolder, length + 1);   // String to charArray.
  payloadString = payloadMsg;                              // saving incoming payload to a global variable for later use.
                                                           // added print in terminal to make see what message is incoming.
  typeOfSubMessage(payloadString);                         // calling typeOfSubMessage to determine what payload is about.
}

void typeOfSubMessage(String payload) {               // Can be built upon to continue handling incoming broker messages.
  if (payload.startsWith("ColorScheme")) {            // Messages from the app will have different identifiers
    payload.remove(0, 11);                            // such as #, % and & to identify what the message is about.
    currentColorScheme = payload;                     // This method will be built upon when we have more 
                                                      // data sent from the app to the terminal.
  } else if (payload.startsWith("tempThresholdLow")) {   
    payload.remove(0, 16);
    temperatureThresholdLow = payload.toInt();
    Serial.print("Low temperature limit received: ");
    Serial.print(temperatureThresholdLow);
    Serial.println();

  } else if (payload.startsWith("tempThresholdHigh")) {
    payload.remove(0, 17);
    temperatureThresholdHigh = payload.toInt();
    Serial.print("High temperature limit received: ");
    Serial.print(temperatureThresholdHigh);
    Serial.println();

  } else if (payload.startsWith("humThresholdLow")) {
    payload.remove(0, 15);
    humidityThresholdLow = payload.toInt();
    Serial.print("Low humidity limit received: ");
    Serial.print(humidityThresholdLow);
    Serial.println();

  } else if (payload.startsWith("humThresholdHigh")) {
    payload.remove(0, 16);
    humidityThresholdHigh = payload.toInt();
    Serial.print("High humidity limit received: ");
    Serial.print(humidityThresholdHigh);
    Serial.println();

  } else if (payload.startsWith("buzzerInterval")) {
    payload.remove(0, 14);
    buzzerInterval = payload.toInt();
    Serial.print("Buzzer interval received: ");
    Serial.print(buzzerInterval);
    Serial.println();

  } else if (payload.startsWith("buzzerDuration")) {
    payload.remove(0, 14);
    buzzerDuration = payload.toInt();
    Serial.print("Buzzer duration received: ");
    Serial.println(buzzerDuration);
    Serial.println();
  } else if (payload.startsWith("fontSize")) {
      if (payload.equals("fontSizeIncrease")) {
        if (fontSize != 4) {
        fontSize = fontSize + 1;
        }
      }
    else if (payload.equals("fontSizeDecrease")) {
        if(fontSize != 1) {
        fontSize = fontSize - 1;
      }
    } 
  } else if (payload.equals("defaultMode")) {
    disabilityMode = "defaultMode";

  } else if (payload.equals("colorBlindMode")) {
    disabilityMode = "defaultMode";
    
  } else if (payload.equals("hearingImpMode")) {
    disabilityMode = "hearingImp";
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID, CLOUD8_USER, CLOUD8_PW)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(TOPIC, "{\"message\": \"Wio Terminal is connected!\"}");
      Serial.println("Published connection message successfully!");
      // ... and resubscribe
      client.subscribe(TOPIC);
      Serial.print("Subcribed to ");
      Serial.println(TOPIC);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}

void updateDisplay(int temperature, int humidity) {
  updatingScheme();

  //Setting the title header 
  tft.fillScreen(currentColorMode[0]); 
  tft.setTextColor(currentColorMode[1]);  //Setting text color
  tft.setTextSize(3);                     //Setting text size 

  tft.drawFastVLine(160,0,240,currentColorMode[3]); //Drawing verticle line
  tft.drawFastHLine(0,120,320,currentColorMode[3]); //Drawing horizontal line

  // Setting temperature block background color
  if (temperatureThresholdMet) {
    tft.fillRect(0, 0, 160, 120, currentColorMode[2]); // Highlight temperature block
  }

  // Setting humidity block background color
  if (humidityThresholdMet) {
    tft.fillRect(0, 120, 160, 240, currentColorMode[2]); // Highlight humidity block
  }

  //Setting temperature
  tft.setTextSize(2);
  tft.drawString("Temperature",10,30);
  tft.setTextSize(fontSize);
  tft.drawNumber(temperature,35,60); //Display temperature values 
  tft.drawString("C",90,60);

  //Setting humidity
  tft.setTextSize(2);
  tft.drawString("Humidity",25,160);
  tft.setTextSize(fontSize);
  tft.drawNumber(humidity,15,190); //Display humidity values 
  tft.drawString("%RH",70,190);
}

boolean isTempThresholdMet (int temperature) {
  // Check if temperature is above high or below low thresholds
  if (temperature > temperatureThresholdHigh) {
    temperatureThresholdMet = true;
  } else if (temperature < temperatureThresholdLow) {
    temperatureThresholdMet = true;
  } else {
    temperatureThresholdMet = false;
  }
  return temperatureThresholdMet;
}

boolean isHumThresholdMet (int humidity) {
  // Check if humidity is above high or below low thresholds
  if (humidity > humidityThresholdHigh) {
    humidityThresholdMet = true;
  } else if (humidity < humidityThresholdLow) {
    humidityThresholdMet = true;
  } else {
    humidityThresholdMet = false;
  }
  return humidityThresholdMet;
}

// Method to check threshold limits and change color accordingly for deafult mode
void checkThresholdsDefault (int temperature, int humidity) {
  if (isTempThresholdMet(temperature) && isHumThresholdMet(humidity)) {
    checkBuzzerInterval();
    tempBackgroundColor = currentColorMode[2];
  } else if (isTempThresholdMet(temperature)) {
    checkBuzzerInterval();
    tempBackgroundColor = currentColorMode[2];
  } else if (isHumThresholdMet(humidity)){
    checkBuzzerInterval();
    tempBackgroundColor = currentColorMode[2];
  }
}

// Method to check threshold limits and change color accordingly for hearing deficiency mode
void checkThresholdsHearing (int temperature, int humidity) {
  if (isTempThresholdMet(temperature) && isHumThresholdMet(humidity)) {
    tempBackgroundColor = currentColorMode[2];
    digitalWrite(LEDPIN, HIGH);
  } else if (isTempThresholdMet(temperature)) {
    tempBackgroundColor = currentColorMode[2];
    digitalWrite(LEDPIN, HIGH);
  } else if (isHumThresholdMet(humidity)){
      digitalWrite(LEDPIN, HIGH);
    tempBackgroundColor = currentColorMode[2];
  } else {
    digitalWrite(LEDPIN, LOW);
  }
}

// Method to check buzzer interval and update the value when the buzzer were deactivated
void checkBuzzerInterval () {
 // If thresholds are met and enough time has passed since the last buzzer alert
  if ((temperatureThresholdMet || humidityThresholdMet) && (millis() - lastBuzzerTime) >= (buzzerInterval * 1000)) {
    // Activate the buzzer for the specified duration
    playTone(1000, buzzerDuration * 1000); // Convert seconds to milliseconds

    // Update the last buzzer alert time
    lastBuzzerTime = millis();
  }
}

// Method to adjust the alarm
void playTone(int tone, int duration) {
  unsigned long startTime = millis(); // Get the current time

  while (millis() - startTime < duration) { // Loop until the duration has elapsed
    digitalWrite(BUZZER_PIN, HIGH); // Turn the buzzer on
    delayMicroseconds(tone); // Wait for the specified tone duration
    digitalWrite(BUZZER_PIN, LOW); // Turn the buzzer off
    delayMicroseconds(tone); // Wait for the same duration to complete the cycle
  }
}

void setup() {

  digitalWrite(LEDPIN, LOW);

  wifiClient.setCACert(CA_CERTIFICATION);
  Serial.begin(115200);

  while (!Serial); // Wait for Serial to be ready
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    WiFi.begin(ssid, password);
  }
  Serial.println();
  Serial.println("Successfully connected to Wi-Fi!");

  client.setServer(server, 8883);
  client.setCallback(callback);

  tft.begin(); //Start TFT LCD
  tft.setRotation(3); //Set TFT LCD rotation

  dht.begin(); //Start DHT sensor

  pinMode(WIO_BUZZER, OUTPUT); // Set buzzer pin as output
}

void loop() {
  int temperature = dht.readTemperature(); //Assign variable to store temperature 
  int humidity = dht.readHumidity(); //Assign variable to store humidity

  char temperatureString[10];
  char humidityString[10];
  
  snprintf(temperatureString, sizeof(temperatureString), "%d", temperature); // Format integer as string
  snprintf(humidityString, sizeof(humidityString), "%d", humidity);
  client.publish("CLOUD8/temperature", temperatureString);  // Publish the integer
  client.publish("CLOUD8/humidity", humidityString);

  // Update xxThresholdMet variables to be up to date on the current sensor data
  isTempThresholdMet(temperature); // Method call to check if thresholds has been reached
  isHumThresholdMet(humidity); // Method call to check if thresholds has been reached

  // Check what is held in the disabilityMode variable and call appropriate threshold methods
  if (disabilityMode.equals("defaultMode")) {
    checkThresholdsDefault(temperature, humidity);

  } else if (disabilityMode.equals("hearingImp")) {
    checkThresholdsHearing(temperature, humidity);
    
  } else { // Error handling: If the string is empty or a faulty message is sent, deafult mode is the standard mode
    checkThresholdsDefault(temperature, humidity);
  }

  updateDisplay(temperature, humidity); // Update the display
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(2500);
}
