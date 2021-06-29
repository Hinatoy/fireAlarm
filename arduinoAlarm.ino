#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial fireAlarm(10, 11);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// FLAME
//const int sensorMin = 0;     // sensor minimum
//const int sensorMax = 1024;  // sensor maximum
int flame_sensor = 4;
int flame_undetected;
int count = 0;

void setup() {
  lcd.init();
  Wire.begin();
  lcd.backlight();

  Serial.begin(9600);
  dht.begin(); // initialize the sensor
  fireAlarm.begin(9600);
  Serial.println("I am here - USB");
  pinMode(flame_sensor, INPUT);
}

void loop() {
  // wait a few seconds between measurements.
  delay(2000);

  lcd.setCursor(0,0);

//FLAME SENSOR
flame_undetected = digitalRead(flame_sensor);
if(flame_undetected == 1) {
  Serial.println("no fire");
  count = 0;
} else {
  count++;
  if(count >= 3) {
    //Serial.println("FIRE");
    fireAlarm.println("FIRE");
  }
}
  // read humidity
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  float tempC = dht.readTemperature();

  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%\n");
    lcd.print("Humidity: ");
    lcd.print(humi);

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C\n");
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(tempC);
  }
}
