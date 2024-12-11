#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// MAX30102 sensor
MAX30105 particleSensor;

// DHT22 sensor
#define DHTPIN D1       // Pin where the DHT22 is connected
#define DHTTYPE DHT22   // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint32_t t1;

float calculateHeartRate(uint32_t irValue) {
  // Simplified conversion for demonstration purposes
  return irValue / 1000.0;
}

void setup() {
    Serial.begin(115200); // Set the baud rate for serial communication
    Serial.println("Setup started");

    t1 = millis();

    Wire.begin(); // Initialize I2C communication with default pins

    // Initialize MAX30102 sensor
    if (!particleSensor.begin(Wire)) {
       Serial.println("Could not find a valid MAX30102 sensor, check wiring!");
       while (1);
    }

    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);

    // Initialize DHT22 sensor
    dht.begin();

    // Initialize OLED display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most displays
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }
    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay(); // Clear the buffer

    Serial.println("Setup complete");
}

void loop() {
    // Read data from the MAX30102 sensor
    uint32_t irValue = particleSensor.getIR();
    float heartRate = calculateHeartRate(irValue) + 40;

    // Check if the heart rate is less than 45
    if (heartRate < 45) {
        heartRate = 0;
    }
    
    // Read data from the DHT22 sensor
    float temperature = dht.readTemperature();

    // Display data on the Serial Monitor
    Serial.print("Heart Rate (BPM): ");
    Serial.println(heartRate);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    // Display data on the OLED
    display.clearDisplay(); // Clear the display
    display.setTextSize(2);  // Set text size to 2
    display.setTextColor(SSD1306_WHITE); // Set text color to white

    display.setCursor(0,0); // Set cursor to top-left
    display.print("Heart Rate:");
    display.setCursor(0, 20);
    display.print(heartRate);

    display.setCursor(0, 40);
    display.print("Temp:");
    display.setCursor(60, 40);
    display.print(temperature);
    display.print(" C");

    display.display(); // Update the display with the new data
    
    delay(1000); // Add a delay to slow down the loop and make the output readable
}
