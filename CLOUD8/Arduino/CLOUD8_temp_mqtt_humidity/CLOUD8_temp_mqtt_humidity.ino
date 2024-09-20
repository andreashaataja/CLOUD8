#include "rpcWiFi.h"
#include <PubSubClient.h>
#include "TFT_eSPI.h" //TFT LCD library 
#include "DHT.h" // DHT library 

//Definitions temp/humi
#define DHTPIN 0 //Define signal pin of DHT sensor 
// #define DHTPIN PIN_WIRE_SCL //Use I2C port as Digital Port */
#define DHTTYPE DHT11 //Define DHT sensor type

//Initializations
DHT dht(DHTPIN, DHTTYPE); //Initializing DHT sensor
TFT_eSPI tft; //Initializing TFT LCD
// TFT_etftite tft = TFT_etftite(&tft); //Initializing buffer

// Update these with values suitable for your network.
const char *ssid = "#Telia-DA3108";      // your network SSID
const char *password = "349KTcA3KKcf4Tfe"; // your network password

const char *ID = "Wio-Terminal-Client";  // Name of our device, must be unique
const char *TOPIC = "CLOUD8";  // Topic to subcribe to
const char *subTopicOne = "temperature";  // Topic to subcribe to
const char *subTopicTwo = "humidity";
const char *server = "192.168.1.124"; // Server URL


WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0; i<length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(TOPIC, "{\"message\": \"Wio Terminal is connected!\"}");
      Serial.println("Published connection message successfully!");
      // ... and resubscribe
      client.subscribe(subTopicOne);
      client.subscribe(subTopicTwo);
      Serial.print("Subcribed to ");
      Serial.print(subTopicOne);
      Serial.print(" and ");
      Serial.println(subTopicTwo);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  }
}
void updateDisplay(int temperature, int humidity) {
  //Setting the title header 
  tft.fillScreen(TFT_BLUE); 
  tft.setTextColor(TFT_WHITE);  //Setting text color
  tft.setTextSize(3);                         //Setting text size 

  tft.drawFastVLine(150,0,240,TFT_DARKGREEN); //Drawing verticle line
  tft.drawFastHLine(0,120,320,TFT_DARKGREEN); //Drawing horizontal line

  //Setting temperature
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Temperature",10,30);
  tft.setTextSize(3);
  tft.drawNumber(temperature,50,60); //Display temperature values 
  tft.drawString("C",90,60);

  //Setting humidity
  tft.setTextSize(2);
  tft.drawString("Humidity",25,160);
  tft.setTextSize(3);
  tft.drawNumber(humidity,30,190); //Display humidity values 
  tft.drawString("%RH",70,190);

  //tft.pushSprite(0,0); //Push to LCD
  //delay(50);

}

void setup()
{
  // Wio studio code
  //Serial.begin(9600); //Start serial communication
  
  Serial.begin(115200);

  while (!Serial); // Wait for Serial to be ready
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    WiFi.begin(ssid, password);
    // wait 1 second for re-trying
    //delay(1000);
  }
  
  Serial.print("Connected to ");
  Serial.println(ssid);
  //delay(500);

  client.setServer(server, 1883);
  client.setCallback(callback);

  tft.begin(); //Start TFT LCD
  tft.setRotation(3); //Set TFT LCD rotation
 // tft.createtftite(240,240); //Create buffer

  dht.begin(); //Start DHT sensor 
}

void loop()
{
  
  int t = dht.readTemperature(); //Assign variable to store temperature 
  int h = dht.readHumidity(); //Assign variable to store humidity

  int temperature = t;  // Example integer to publish
  char temperatureString[10];   // Buffer to hold the formatted string

  int humidity = h;
  char humidityString[10];

  snprintf(temperatureString, sizeof(temperatureString), "%d", temperature); // Format integer as string
  snprintf(humidityString, sizeof(humidityString), "%d", humidity);
  client.publish("CLOUD8/temperature", temperatureString);  // Publish the integer
  client.publish("CLOUD8/humidity", humidityString);
  updateDisplay(t, h);
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(2500);
}