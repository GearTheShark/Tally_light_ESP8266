#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

#define LED_BUILTINN D4
#define DATA_PIN D2
#define NUM_LEDS 1
//#define FASTLED_ESP8266_RAW_PIN_ORDER
CRGB leds[NUM_LEDS];

const char* ssid = "Your_SSID";
const char* pass = "Your_PASS";
String HexValue = "000000";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  leds[0] = CRGB::Black;
  FastLED.show();
  leds[0] = CRGB::Blue;
  FastLED.show();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { //Wait for connection
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Blue;
    FastLED.show();
    Serial.println("Waiting to connect…");
  }\
  //  digitalWrite(LED_BUILTINN, HIGH);
  leds[0] = CRGB::Black;
  FastLED.show();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/set", HTTP_GET, recieveColor);
  server.begin();
  Serial.println("Server listening");
}

void recieveColor() {
  String HexValue = server.arg("color");
  long number = (long) strtol( &HexValue[0], NULL, 16);
  Serial.println(number);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  leds[0].setRGB(r, g, b);
  FastLED.show();
  server.send(200, "text/plain", "OK");
  Serial.println(HexValue);
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
}

void loop() {
  if (WiFi.status() == WL_DISCONNECTED) {
    Serial.print(WiFi.status());
    Serial.println("Disconnected");
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
  } else if (WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.print(WiFi.status());
    Serial.println("No SSID Available");
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(500);
  } else {
    server.handleClient();
  }
}
