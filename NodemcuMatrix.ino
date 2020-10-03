
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>

#include <NTPClient.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h> 
#include <stdio.h>
#include "Streaming.h"
#include <String.h>


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/Picopixel.h>
#include <Fonts/TomThumb.h>

#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif


#define SLEEP_DELAY_IN_SECONDS  30
#define NEOPIN           D6



Adafruit_NeoMatrix *matrix;
Adafruit_NeoMatrix *strip;

uint16_t colors[3];
int x;   
int pass = 0;


const char* clientName;

char mqtt_topictemp[40] = "sensors/NAME/temperature";
char mqtt_topicled[40] = "sensors/NAME/led";
char mqtt_topicstate[40] = "sensors/NAME/state";

char mqtt_subtopicled[40] = "action/NAME/led";
char mqtt_subtopicstate[40] = "action/NAME/state";
char mqtt_subtopiccolor[40] = "action/NAME/color";
char mqtt_subtopictext[40] = "action/NAME/cText";

char ssid[40] = "Freifunk Erfurt";
char password[40] = "";
char devicename[40] ="";

char mqtt_server[40] = "192.168.10.1";
char mqtt_username[40] = "test";
char mqtt_password[40] = "testpwd";

WiFiClient espClient;
PubSubClient client(espClient);
char ledString[16];
char stateString[6];
String sLedString;
String sStateString;
String cText;
int btnVal = 1;
int redVal=128;
int greVal=128;
int bluVal= 128;
int midpix=0;
int mixpixp=0;
int mixpixn=0;
int j=0;
int realpix=0;
char WSPixel[5]="50";
int state=0;
unsigned long mytime;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

void setup() {

  Serial.begin(115200);
  SPIFFS.begin();
  Serial.println("Booting");

 if (SPIFFS.exists("/config.json"))
 {
   readfile("/config.json");
 }
 else
 {

  if(SPIFFS.format())
  {
      DynamicJsonDocument jsonDoc(1024);
      Serial.println("Format successful"); 
      jsonDoc["devicename"] = "DEV";
      jsonDoc["ssid"] = "Freifunk Erfurt";
      jsonDoc["password"] = "";

       File configFile = SPIFFS.open("/config.json", "w");
       if (!configFile)
       {
        Serial.println("Failed to open config file for writing");
       
       }
       else
       {
       serializeJson(jsonDoc, Serial);
        serializeJson(jsonDoc, configFile);
        configFile.close();
       }
  }
  else
  {
      Serial.println("Format failed"); 
  }   
 }


 Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  matrix= new Adafruit_NeoMatrix(32, 8, NEOPIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
  strip = matrix;
  
  colors[0] = matrix->Color(255, 0, 0);
  colors[1] = matrix->Color(0, 255, 0);
  colors[2] = matrix->Color(0, 0, 255);
  x = matrix->width();
  String prefix ="Lolin-";
  cText="Hallo";
  uint8_t mac[6];
  WiFi.macAddress(mac);


 // clientName=strdup(prefix.c_str());
 clientName=strdup(devicename);
 
Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


 // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname(clientName);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  timeClient.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(10);
  matrix->setTextColor(matrix->Color(255,255, 255));
  matrix->setFont(&TomThumb);
  matrix->setCursor(2,7);          
  matrix->print("Start");
  matrix->show();
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
}


void loop() {
  
 
   ArduinoOTA.handle();
 
  if (!client.connected())
  {
    delay(1000);
    reconnect();
    
    if (client.subscribe(mqtt_subtopicled))
    {
      Serial << "Successfully subscribed: " << mqtt_subtopicled << endl;    
    }
    if (client.subscribe(mqtt_subtopiccolor))
    {
      Serial << "Successfully subscribed: " << mqtt_subtopiccolor << endl;    
    }
    if (client.subscribe(mqtt_subtopicstate))
    {
      Serial << "Successfully subscribed: " << mqtt_subtopicstate << endl;    
    }
     if (client.subscribe(mqtt_subtopictext))
    {
      Serial << "Successfully subscribed: " << mqtt_subtopictext << endl;    
    }
    
  }
    
 client.loop();
 timeClient.update();

  
  
  Serial << "State: " << state << endl; 
  switch(state)
         {
            case 1:
              rainbow(100);
              break;
            case 2:
                colorWipe(strip->Color(0, 0, 255), 50); // Blue
                colorWipe(strip->Color(0, 255, 0), 50); // Blue
                colorWipe(strip->Color(255, 0, 0), 50); // Blue
                colorWipe(strip->Color(255, 0, 255), 50); // Blue
                colorWipe(strip->Color(255,255, 0), 50); // Blue
                colorWipe(strip->Color(255,255, 255), 50); // Blue             
              break;
            case 3:
              color(strip->Color(0, 0, 255));
              delay(500);
              color(strip->Color(0, 255, 0));
              delay(500);
              color(strip->Color(255, 0, 0));
              delay(500);
              color(strip->Color(255, 255, 255));
              delay(500);
              color(strip->Color(128, 255, 128));
              delay(500);
              break;
            case 4:
               theaterChaseRainbow(50);
              break;
            case 5:
               rainbowCycle(50);
              break;
            case 6:
                colorWipe(strip->Color(255,255, 255), 0); // Blue  
                delay(500);
                strip->clear();
                strip->show();
                break;
             case 7:
                strip->clear();
                strip->show();
                break;
              case 8:
                matrix->fillScreen(0);
                matrix->drawRect ( 0, 0, 32, 8, matrix->Color(128, 128, 128));
                matrix->setFont(&TomThumb);
                matrix->setCursor(2,7);          
                matrix->print(cText);

                matrix->setTextColor(Wheel((j+timeClient.getSeconds()) & 255));
                matrix->show();
                  break;

              default:
                matrix->fillScreen(0);
                matrix->drawRect ( 0, 0, 32, 8, matrix->Color(64, 255, 64));
                matrix->setFont(&TomThumb);
                matrix->setCursor(2,7);
             
                matrix->print(timeClient.getFormattedTime());
               
                if(++j >= 255) j = 0;
                matrix->setTextColor(Wheel((j+timeClient.getSeconds()) & 255));
                matrix->show();
                break;
          }

    
   if (millis() > (mytime + 30000)) {
        mytime = millis();
         Serial << "mytime: " << mytime << endl; 
         Serial.println(timeClient.getFormattedTime());       
  }
  delay(250);
}
