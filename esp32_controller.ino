#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "your ssid";
const char* pass = "your password";
const char* server = "your server ip address";

#define NUM_LED 145
#define LED 2

Adafruit_NeoPixel pixels(NUM_LED, LED, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Timing variables
unsigned long lastFetch = 0;
unsigned long lastEffectUpdate = 0;
unsigned long blinkInterval = 800;
unsigned long rainbowInterval = 30;
unsigned long colorWipeInterval = 40;

// LED control variables
int r = 0, g = 0, b = 0, brightness = 50;
String displayMode = "normal";
String colorName = "none";
bool ledState = false;
int rainbowPos = 0;
int wipeIndex = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  lcd.init();
  lcd.backlight();
  pixels.begin();
  pixels.show();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(2,0);  lcd.print("Neo-Pixel Status");

  lcd.setCursor(3, 1);  lcd.print("Mode: ");  lcd.print(displayMode);  
  
  lcd.setCursor(3, 2);  lcd.print("Color: ");  lcd.print(colorName);

  lcd.setCursor(2, 3); lcd.printf("Brightness: "); lcd.print(brightness);
}

void displayNormal() {
  pixels.setBrightness(brightness);
  for (int i = 0; i < NUM_LED; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void displayBlink() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastEffectUpdate >= blinkInterval) {
    lastEffectUpdate = currentMillis;
    ledState = !ledState;

    if (ledState) {
      pixels.setBrightness(brightness);
      for (int i = 0; i < NUM_LED; i++) {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
      }
    } else {
      pixels.clear();
    }
    pixels.show();
  }
}

void displayColorWipe() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastEffectUpdate >= 20) {
    lastEffectUpdate = currentMillis;

    // Forward
    if (wipeIndex < NUM_LED) {
      pixels.setPixelColor(wipeIndex, pixels.Color(r, g, b));
      pixels.show();
      wipeIndex++;
    } 
    // Backward
    else {
      wipeIndex =0;
      pixels.clear();
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void displayRainbow() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastEffectUpdate >= rainbowInterval) {
    lastEffectUpdate = currentMillis;
    for (int i = 0; i < NUM_LED; i++) {
      pixels.setPixelColor(i, Wheel((i + rainbowPos) & 255));
    }
    pixels.show();
    rainbowPos++;
    if (rainbowPos >= 256) rainbowPos = 0;
  }
}

void fetchServerData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    int httpRes = http.GET();

    if (httpRes > 0) {
      String dataHolder = http.getString();
      StaticJsonDocument<300> doc;
      DeserializationError error = deserializeJson(doc, dataHolder);

      if (!error) {
        r = doc[0]["r"];
        g = doc[0]["g"];
        b = doc[0]["b"];
        brightness = doc[0]["brightness"];
        colorName = doc[0]["color"].as<String>();
        displayMode = doc[0]["display"].as<String>();
        Serial.printf("Mode: %s, Color: %s\n", displayMode.c_str(), colorName.c_str());
        updateLCD();
      } else {
        Serial.printf("JSON Error: %s\n", error.c_str());
      }
    } else {
      Serial.printf("GET failed: %d\n", httpRes);
    }
    http.end();
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Fetch new data every 2 seconds
  if (currentMillis - lastFetch >= 2000) {
    lastFetch = currentMillis;
    fetchServerData();
  }

  // Handle LED effects asynchronously
  if (displayMode == "normal") {
    displayNormal();
  } 
  else if (displayMode == "blink") {
    displayBlink();
  } 
  else if (displayMode == "colorwipe") {
    displayColorWipe();
  } 
  else if (displayMode == "rainbow") {
    displayRainbow();
  }
}
