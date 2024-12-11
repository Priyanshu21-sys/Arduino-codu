#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin (if necessary)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MLX90614 sensor object
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// MAX30105 sensor object
MAX30105 particleSensor;

// Define the pin for the LED
const int ledPin = D1;

uint32_t t1;

// Heart symbol bitmap (16x16 pixels)
const unsigned char heart_bmp[] PROGMEM = {
  0b00001100, 0b00110000,
  0b00011110, 0b01111000,
  0b00111111, 0b11111100,
  0b01111111, 0b11111110,
  0b01111111, 0b11111110,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b01111111, 0b11111110,
  0b01111111, 0b11111110,
  0b00111111, 0b11111100,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000011, 0b11000000,
  0b00000001, 0b10000000
};

float calculateHeartRate(uint32_t irValue) {
  // Simplified conversion for demonstration purposes
  return irValue / 1000.0 + 40;
}

void setup() {
    Serial.begin(115200); // Set the baud rate for serial communication

    // Initialize the LED pin as an output
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Ensure the LED is off initially

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }
    display.clearDisplay();
    display.setTextSize(2); // Set text size for better visibility
    display.setTextColor(SSD1306_WHITE);

    // Initialize the MLX90614 sensor
    if (!mlx.begin()) {
        Serial.println("Error connecting to MLX90614 sensor. Check wiring.");
        while (1);
    }

    // Initialize the MAX30105 sensor
    Wire.begin(); // Initialize I2C communication

    if (!particleSensor.begin(Wire)) {
        Serial.println("Could not find a valid MAX30105 sensor, check wiring!");
        while (1);
    }

    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);

    Serial.println("Setup complete");

    display.clearDisplay();
    display.setCursor(10, 25); // Center the text
    display.println("Setup Complete");
    display.display();
    delay(1000); // Show the setup complete message for 1 second
}

void loop() {
    display.clearDisplay(); // Clear the display buffer

    // MLX90614 temperature readings
    int objectTemp = mlx.readObjectTempC();
    display.setCursor(10, 10); // Center the text
    display.print("Temp: ");
    display.print(objectTemp);
    display.println(" C");
    display.display();

    // Print to the Serial Monitor as well
    Serial.print("Object = "); 
    Serial.print(objectTemp); 
    Serial.println("*C");

    delay(3000); // Show temperature reading for 3 seconds

    display.clearDisplay(); // Clear the display buffer

    // MAX30105 heart rate readings
    uint32_t irValue = particleSensor.getIR();
    int heartRate = calculateHeartRate(irValue);
    
    // Check if heart rate is below 45 BPM
    if (heartRate < 45) {
        heartRate = 0;
        digitalWrite(ledPin, HIGH); // Turn on the LED
    } else {
        digitalWrite(ledPin, LOW); // Turn off the LED
    }
    
    display.setCursor(10, 10); // Center the text
    display.print("HeartRate:");
    display.setCursor(25, 30); // Move text down for separation
    display.print(heartRate);
    display.println(" BPM");

    // Draw a heart symbol that blinks
    for (int i = 0; i < 3; i++) { // Blink 3 times during the 3-second display time
        display.drawBitmap(112, 48, heart_bmp, 16, 16, SSD1306_WHITE);
        display.display();
        delay(250);
        display.drawBitmap(112, 48, heart_bmp, 16, 16, SSD1306_BLACK);
        display.display();
        delay(250);
    }

    // Update the display with the new values
    display.display();

    // Print to the Serial Monitor as well
    Serial.print("Heart Rate (BPM): ");
    Serial.println(heartRate);

    delay(3000); // Show heart rate reading for 3 seconds
}
