#include <Insights.h>
#include <Wifi.h>
#include <HTTP_Method.h>
#include <Uri.h> 
#include <WebServer.h>
#include <Uri.h> 
#include <WebServer.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include <TinyGPS++.h>
#include "MQ135.h"
#include "cretss.h"
#define RXD2 16
#define TXD2 17
#include <HTTP_Method.h> 

HardwareSerial neogps(1);

TinyGPSPlus gps;

#define THINGNAME "esp32"
const char ssid[] = "realme 7 Pro"; //Airtel_comm_6224 realme 7 Pro
const char password[] = "12345678";     //987654321
const char AWS_IOT_ENDPOINT[] = "a27mqcf3cydka7-ats.iot.us-east-1.amazonaws.com";
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure wifi_client;
MQTTClient mqtt_client(256);

Adafruit_MLX90614 mlx;
MAX30105 particleSensor;

MQ135 mq135_sensor(32);

uint32_t t1;

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  wifi_client.setCACert(AWS_CERT_CA);
  wifi_client.setCertificate(AWS_CERT_CRT);
  wifi_client.setPrivateKey(AWS_CERT_PRIVATE);
  mqtt_client.begin(AWS_IOT_ENDPOINT, 8883, wifi_client);
  mqtt_client.onMessage(incomingMessageHandler);
  Serial.print("Connecting to AWS IOT");
  while (!mqtt_client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  if (!mqtt_client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  mqtt_client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

void publishMessage() {
  DynamicJsonDocument doc(512);

  float envTempC = mlx.readAmbientTempC();
  float objTempC = mlx.readObjectTempC();

  // Check if temperature readings are NaN and replace with 0
  if (isnan(envTempC)) {
    envTempC = 0;
  }
  if (isnan(objTempC)) {
    objTempC = 0;
  }
  doc["Environment_temp_C"] = envTempC;
  doc["Object_temp_C"] = objTempC;

  // Convert heart rate to a two-digit format
  int heartRate = particleSensor.getIR();
  if (heartRate < 10) {
    doc["Heart_Rate"] = "0" + String(heartRate);
  } else {
    doc["Heart_Rate"] = String(heartRate);
  }

  float temperature = 25.0;
  float humidity = 50.0;
  doc["Gas_Sensor"] = mq135_sensor.getCorrectedPPM(temperature, humidity);
  //if (gps.location.isValid()) {
  doc["gps_speed"] = gps.speed.knots();
  //doc["gps_altitude"] = gps.altitude.meters();
  //doc["gps_course"] = gps.course.deg();
  doc["gps_hdop"] = gps.hdop.hdop();
  doc["gps_lat: "] = gps.location.lat();
  doc["gps_lng: "] = gps.location.lng();
  //}
  doc["elapsed_time"] = millis() - t1;
  doc["Device_name"] = THINGNAME;
  String jsonString;
  serializeJson(doc, jsonString);
  mqtt_client.publish(AWS_IOT_PUBLISH_TOPIC, jsonString.c_str());
  Serial.println("Sent a message");
}

void incomingMessageHandler(String &topic, String &payload) {
  Serial.println("Message received!");
  Serial.println("Topic: " + topic);
  Serial.println("Payload: " + payload);
}

void setup() {
  Serial.begin(9600);
  t1 = millis();
  mlx.begin();
  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  connectAWS();
}

void loop() {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
      }
    }
  }
}
