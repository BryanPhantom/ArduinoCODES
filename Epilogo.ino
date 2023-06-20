#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "Secrets.h"
#include "time.h"

#define DATOS 25
#define LEDGREENPIN 15
#define LEDREDPIN 2
OneWire oneWire(DATOS);
DallasTemperature sensors(&oneWire);

unsigned long epochTime; 
unsigned long dataMillis = 0;

const char* ntpServer = "pool.ntp.org";
const char* serverName = "https://us-east-1.aws.data.mongodb-api.com/app/plant_monitor-epekp/endpoint/plantdata";

//const int contador_rfid = 23;<-------------------MOVER 

StaticJsonDocument<500> doc;


float led =27;
float buzz = 26;
//int contador = 0;<------------------------------------MOVER 
//int estado_rfid =0; <-----------------------------------MOVER
//int lastButtonState_asc = 1; <-------------------------------MOVER

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Dallas Temperature IC Control Library Demo");
    sensors.begin();
    pinMode(LEDGREENPIN, OUTPUT);
    pinMode(LEDREDPIN, OUTPUT);

    //pinMode(contador, INPUT); <------------------------MOVER

    digitalWrite(LEDGREENPIN, LOW);
    digitalWrite(LEDREDPIN, LOW);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Cargando...");
    delay(2000);
    lcd.clear();
    
    pinMode(led, OUTPUT);
    pinMode(buzz, OUTPUT);

   
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    configTime(0, 0, ntpServer);
}

void loop(void)
{
 Serial.print("Requesting temperatures...");
 sensors.requestTemperatures();
 Serial.println("DONE");
 float tempC = sensors.getTempCByIndex(0);

 if (tempC != DEVICE_DISCONNECTED_C)
  {
    /*
     *  MEDIDOR DE TEMPERATURA
     */
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
    lcd.setCursor(0, 0);
    lcd.print("Temperatura:");
    lcd.setCursor(0, 1);
    lcd.print(tempC);
    lcd.print((char)223);
    lcd.print("C");
    lcd.print(" | ");
    lcd.print(DallasTemperature::toFahrenheit(tempC));
    lcd.print(" F"); 

    /*
     * SENSOR RFID
     */

    /*
   estado_rfid = digitalRead(contador_rfid);

    if (estado_rfid != lastButtonState_asc){
      if (estado_rfid == HIGH) {
        contador ++;
        Serial.println("+");
        Serial.println("Se registro");
        Serial.println(contador);
        delay(100);
        
      }
    }
    */

    doc["sensors"]["temperature"] = tempC;
    //doc["sensors"]["constant"] = contador;  <-----------------------------------MOVER


    Serial.println("Uploading data... "); 
    POSTData();
    
  }
  else
  {
    Serial.println("Error: No se detecta temperatura");
  }
  if (tempC>30){
    digitalWrite(led, HIGH);
    digitalWrite(buzz, HIGH);
  }else{
    digitalWrite(led, LOW);
    digitalWrite(buzz, LOW);
  }
  if (tempC<25){
    digitalWrite(led, HIGH);
    digitalWrite(buzz, HIGH);
  }
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return(0);
  }
  time(&now);
  return now;
}

void POSTData()
{
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String json;
      serializeJson(doc, json);

      Serial.println(json);
      
      int httpResponseCode = http.POST(json);
      Serial.println(httpResponseCode);
      
     // Serial.println(http.getString());
      
      if (httpResponseCode == 204) {
        Serial.println("Data uploaded.");
        digitalWrite(LEDGREENPIN, HIGH);
        delay(200);
        digitalWrite(LEDGREENPIN, LOW);
      } else {
        Serial.println("ERROR: Couldn't upload data.");
        digitalWrite(LEDREDPIN, HIGH);
        delay(200);
        digitalWrite(LEDREDPIN, LOW);
      }

    }
}
