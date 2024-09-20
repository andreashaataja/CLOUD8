//The following code is from https://github.com/lakshanthad/Wio_Terminal_Classroom_Arduino/blob/main/Classroom%2012/Smart_Garden/Smart_Garden.ino
// and was slightly altered to match our needs.
//Libraries 
#include "TFT_eSPI.h" //TFT LCD library 
#include "DHT.h" // DHT library 

//Definitions
#define DHTPIN 0 //Define signal pin of DHT sensor 
// #define DHTPIN PIN_WIRE_SCL //Use I2C port as Digital Port */
#define DHTTYPE DHT11 //Define DHT sensor type 

//Initializations
DHT dht(DHTPIN, DHTTYPE); //Initializing DHT sensor
TFT_eSPI tft; //Initializing TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing buffer

void setup() {
  Serial.begin(115200); //Start serial communication
  
  dht.begin(); //Start DHT sensor 
  tft.begin(); //Start TFT LCD
  tft.setRotation(3); //Set TFT LCD rotation
  spr.createSprite(TFT_HEIGHT,TFT_WIDTH); //Create buffer
    
}

void loop() {
  int temp = dht.readTemperature(); //Assign variable to store temperature 
  int humi = dht.readHumidity(); //Assign variable to store humidity 

  //Setting the title header 
  spr.fillSprite(TFT_BLUE); //Fill background with white color
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.setTextSize(3); //Setting text size 

  spr.drawFastVLine(160,0,240,TFT_BLACK); //Drawing verticle line
  spr.drawFastHLine(0,120,160,TFT_BLACK); //Drawing horizontal line

  //Setting temperature
  spr.setTextColor(TFT_BLACK);
  spr.setTextSize(2);
  spr.drawString("Temperature",10,30);
  spr.setTextSize(3);
  spr.drawNumber(temp,50,60); //Display temperature values 
  spr.drawString("C",90,60);

  //Setting humidity
  spr.setTextSize(2);
  spr.drawString("Humidity",25,160);
  spr.setTextSize(3);
  spr.drawNumber(humi,30,190); //Display humidity values 
  spr.drawString("%RH",70,190);

  spr.pushSprite(0,0); //Push to LCD
  delay(50);


}