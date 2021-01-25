/*
   Código fuente desarrollado por iberotecno. Año 2020.
   Versión 1.6

   MIT License

Copyright (c) 2021 iberotecno.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

Contact with info@iberotecno.com to ask for permissions.
Have a written contract from iberotecno.

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 * */

//Download and copy the libraries to Arduino libraries folder (documents > arduino > libraries) and restart IDE. Use the provided URLS.
#include <WiFi.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>       //https://github.com/me-no-dev/ESPAsyncWebServer and download this library too: https://github.com/me-no-dev/AsyncTCP
#include <ESPAsyncWiFiManager.h>     //https://github.com/alanswx/ESPAsyncWiFiManager
#include <ArduinoJson.h>             //https://github.com/bblanchon/ArduinoJson/tree/5.x
#include <SimpleTimer.h>             //https://github.com/jfturcot/SimpleTimer
#include <FastLED.h>                 //https://github.com/FastLED/FastLED
#include "ThingSpeak.h"              //https://github.com/mathworks/thingspeak-arduino/
#include <FS.h>                      //library to access the filesystem

#define LED_PIN     14
#define NUM_LEDS    1
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
float t = 0;
CRGB leds[NUM_LEDS];

//define custom fields
char limite_malo[100];
char limite_bueno[100];
char brillo_led[100];
char api_key[100];
char numero_canal[100];
char offset_calibracion[100];

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

AsyncWebServer server(80);
DNSServer dns;
SimpleTimer timer;

//Paleta de color
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {

  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  //reset pins
  pinMode(26, INPUT);           // set pin to input
  digitalWrite(26, LOW);
  pinMode(27, INPUT);           // set pin to input
  digitalWrite(27, LOW);


  //FS
  Serial.println("\n Starting");
  //
  //***clean FS, for testing if pin 26 high on boot
  if (digitalRead(26) == HIGH) {
    if (SPIFFS.format()) {
      Serial.println("SPIFFS Formatted ok. Starting.");
      delay(1000);
    }
    else {
      Serial.println("SPIFFS Format error. Restarting.");
      ESP.restart();
      delay(1000);
    }
  }
  //read configuration from FS json
  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(limite_malo, json["limite_malo"]);
          strcpy(limite_bueno, json["limite_bueno"]);
          strcpy(brillo_led, json["brillo_led"]);
          strcpy(api_key, json["api_key"]);
          strcpy(numero_canal, json["numero_canal"]);
          strcpy(offset_calibracion, json["offset_calibracion"]);
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }

  //end read
  AsyncWiFiManagerParameter custom_limite_bueno("limite_bueno", "Valor aire bueno", limite_bueno, 100);
  AsyncWiFiManagerParameter custom_limite_malo("limite_malo", "Valor aire malo", limite_malo, 100);
  AsyncWiFiManagerParameter custom_brillo_led("brillo_led", "Brillo del LED", brillo_led, 100);
  AsyncWiFiManagerParameter custom_api_key("api_key", "Write API Key", api_key, 100);
  AsyncWiFiManagerParameter custom_numero_canal("numero_canal", "Channel ID", numero_canal, 100);
  AsyncWiFiManagerParameter custom_offset_calibracion("offset_calibracion", "Offset", offset_calibracion, 100);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  AsyncWiFiManager wifiManager(&server, &dns);

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&custom_limite_bueno);
  wifiManager.addParameter(&custom_limite_malo);
  wifiManager.addParameter(&custom_brillo_led);
  wifiManager.addParameter(&custom_api_key);
  wifiManager.addParameter(&custom_numero_canal);
  wifiManager.addParameter(&custom_offset_calibracion);

  //scheduled reset
  int horas = 1; // hours for scheduled reset
  timer.setInterval(horas * 3600000, reinicio);

  //WiFiManager
  AsyncWiFiManagerParameter custom_text("<p>Seleccione la red WiFi para conectarse.</p>");
  wifiManager.addParameter(&custom_text);
  //reset saved settings if pin 27 high on boot
  if (digitalRead(27) == HIGH) {
    wifiManager.resetSettings();
    WiFi.disconnect(true);   // still not erasing the ssid/pw. Will happily reconnect on next start
    WiFi.begin("0", "0");      // adding this effectively seems to erase the previous stored SSID/PW
    Serial.println("Wifi credentials reset. Restarting.");
    delay(1000);
    ESP.restart();
    delay(1000);
  }
  //set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Iberotecno AP");
  //or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  //read updated parameters
  strcpy(limite_malo, custom_limite_malo.getValue());
  strcpy(limite_bueno, custom_limite_bueno.getValue());
  strcpy(brillo_led, custom_brillo_led.getValue());
  strcpy(api_key, custom_api_key.getValue());
  strcpy(numero_canal, custom_numero_canal.getValue());
  strcpy(offset_calibracion, custom_offset_calibracion.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["limite_malo"] = limite_malo;
    json["limite_bueno"] = limite_bueno;
    json["brillo_led"] = brillo_led;
    json["api_key"] = api_key;
    json["numero_canal"] = numero_canal;
    json["offset_calibracion"] = offset_calibracion;


    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  //WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); // Initialize ThingSpeak

  timer.run();
  int brillo_led_convertido = atoi(brillo_led);
  int limite_malo_convertido = atoi(limite_malo);
  int limite_bueno_convertido = atoi(limite_bueno);
  int offset_calibracion_convertido = atoi(offset_calibracion);
  int offstet_mq_135 = 400;

  unsigned long numero_canal_convertido = strtoul(numero_canal, NULL, 10);

  FastLED.setBrightness(brillo_led_convertido);

int medicion = analogRead(A0);  // Read sensor value and stores in a variable medicion
  int t = medicion + offstet_mq_135 + offset_calibracion_convertido;

  Serial.print("Medicion raw = ");
  Serial.println(medicion);
  Serial.print("Offset = ");
  Serial.println(offset_calibracion_convertido);
  Serial.print("Co2 ppm = ");
  Serial.println(t);

  if (t <= limite_bueno_convertido)
  {
    FastLED.setBrightness(brillo_led_convertido);
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    Serial.println("Aire puro");
  }
  else if ( t >= limite_bueno_convertido && t <= limite_malo_convertido )
  {
    FastLED.setBrightness(brillo_led_convertido);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    FastLED.show();
    Serial.println("Aire cargandose");
  }
  else if (t >= limite_malo_convertido )
  {
    FastLED.setBrightness(brillo_led_convertido);
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    Serial.println("Aire viciado");
  }

  ThingSpeak.setField(1, t); //co2, ppm

  //debug
  //Serial.print("Numero canal char: ");
  //Serial.println(numero_canal);
  //Serial.print("Numero canal convertido: ");
  //Serial.println(numero_canal_convertido);
  //Serial.print("API Key: ");
  //Serial.println(api_key);
  //Serial.print("Limites bueno y malo : ");
  //Serial.println(limite_bueno);
  //Serial.println(limite_bueno_convertido);


  // write to the ThingSpeak channel
  //int x = ThingSpeak.writeFields(numero_canal_convertido, api_key);
  int x = ThingSpeak.writeFields(numero_canal_convertido, api_key);

  delay(15000); // Wait 20 seconds to update the channel again

  if (x == 200) {
    Serial.println("Channel update successful.");
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(500);
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));

  }


}

// restart function definition
void reinicio() {
  ESP.restart();
}
