// here we include the needed libarys for the program
#include <WiFi.h>
#include <MQTT.h>
#include <FastLED.h>
#include <DHT.h>
// Wifi
const char* ssid = "RUC-IOT";
const char* password = "GiHa5934La";
WiFiClient net;
MQTTClient client;
//MQTT
const char* mqttServer = "public.cloud.shiftr.io";
const char* mqttUser = "public"; 
const char* mqttPassword = "public"; 

// Here we define the used variables 
#define TEMP_LED_PIN 17 
#define HUMID_LED_PIN 22
#define NUM_LED 8
#define DHTPIN 16
#define DHTTYPE DHT11

// array to hold values for temp and humid 
CRGB tempLEDs[NUM_LED];
CRGB humidLEDs[NUM_LED];

// we initiate the DHT11 sensor as a object
DHT dht(DHTPIN, DHTTYPE);

// global variables for temp and humid
float dmiTemp  = NAN;
float dmiHumid = NAN;
float lastTemp = 100;     
float lastHumid = 100;

// callback function: we use it to manage the data from dmi and parse it into two values
void dmiInManager(String &topic, String &payload) {
  if (topic == "esp32IDS/DMI") {
    int parsedDataComma = payload.indexOf(',');
    // Only proceed if a comma was actually found!
    if (parsedDataComma != -1) {
      dmiTemp = payload.substring(0, parsedDataComma).toFloat();
      dmiHumid = payload.substring(parsedDataComma + 1).toFloat();
    }
  }
}

//LED logic for temp: Returns how many LEDs should be colored based on temperature
int tempToLEDCount(float temp) {
  if (temp < -5)  return 1;
  if (temp < 5)   return 2;
  if (temp < 10)  return 3;
  if (temp < 15)  return 4;
  if (temp < 20)  return 5;
  if (temp < 25)  return 6;
  if (temp < 30)  return 7;
  if (temp < 35)  return 8;
}
//LED logic for humid: Returns how many LEDs should be colored based on humidity
int humidToLEDCount(float humid) {
  if (humid < 20)  return 1;
  if (humid < 23)  return 2;
  if (humid < 26)  return 3;
  if (humid < 29)  return 4;
  if (humid < 31)  return 5;
  if (humid < 35)  return 6;
  if (humid < 38)  return 7;
  if (humid < 41)  return 8;
}
// Function that defines LED color based update on temp.
void updateTempStrip(float temp) {
  int count = tempToLEDCount(temp);
  for (int i = 0; i < NUM_LED; i++) {
    if (i < count) {
      tempLEDs[i] = CRGB::Red;   // colored LED
    } else {
      tempLEDs[i] = CRGB::White;  // remaining LEDs white
    }
  }
}
// Function that defines LED color based update on humid.
void updateHumidStrip(float humid) {
  int count = humidToLEDCount(humid);
  for (int i = 0; i < NUM_LED; i++) {
    if (i < count) {
      humidLEDs[i] = CRGB::Blue; // colored LED
    } else {
      humidLEDs[i] = CRGB::White; // remaining LEDs white
    }
  }
}

// checks connection to broker and subscribes vand validates connection
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n MQTT is not connected");

  Serial.print("\nConnecting to MQTT...");
  // You MUST call client.connect to actually talk to Shiftr.io
  while (!client.connect("ESP32_Client", mqttUser, mqttPassword)) {
    Serial.print(".");
    delay(1000);
  }
  // subscribe to the broker in not connected 
  Serial.println("Connected!");
  client.subscribe("esp32IDS/DMI");
}

// setup of funtaments for IoT system to be able to run. 
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  dht.begin();
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(dmiInManager);
  connect();

// Initize the FastLED strips
FastLED.addLeds<WS2812B, TEMP_LED_PIN, GRB>(tempLEDs, NUM_LED);
FastLED.addLeds<WS2812B, HUMID_LED_PIN, GRB>(humidLEDs, NUM_LED);
FastLED.setBrightness(20);

// Starts by showing the values "White" on all LED's
fill_solid(tempLEDs, NUM_LED, CRGB::White);
fill_solid(humidLEDs, NUM_LED, CRGB::White);
  // Send the color data to the physical strip
  FastLED.show();
  Serial.println("DHT11 sensor is responding");
}

void loop() {
  // sampling rate (every 3 sec)
  delay(3000);
  // makes sure that MQTT is connected in all runs
  client.loop();
  // read DHT11 input and define it. 
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();

  // client connection validation
  if (!client.connected()) {
    connect();
  }
  // make sure that values from DHT11 is valid/readable
  if (isnan(temp) || isnan(humid)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // gives parimiters for measurements 
  if (temp < -30 || temp > 40 || humid < 0 || humid > 100) {
    Serial.println("error");
    return;
  }
  // make sure that values from DMI is valid/readable
  // gives parimiters for measurements 
if (!isnan(dmiTemp) && !isnan(dmiHumid)) {
   if (dmiTemp < -30 || dmiTemp > 40 || dmiHumid < 0 || dmiHumid > 100) {
    Serial.println("error");
    return;
  }
  // defines the difference between regional and local readings
  float tempDiff = abs(temp - dmiTemp);
  float humidDiff = abs(humid - dmiHumid);
  // if the gap in readings is within 5 reading 'units', the reading is accepted and passed onto the hardware. 
  if (tempDiff <= 5.0 && humidDiff <= 5){
   if (temp != lastTemp || humid != lastHumid) {
    lastTemp = temp;
    lastHumid = humid;
    Serial.println("UPDATING LEDs");
    updateTempStrip(temp);
    updateHumidStrip(humid);
    FastLED.show();
  } else {
    Serial.println("STOPPED: values unchanged");
      }
    }
}
}
