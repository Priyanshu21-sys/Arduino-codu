#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define the OLED reset pin
#define OLED_RESET -1  // Use -1 if you don't have a reset pin

// I2C address of the OLED display
#define SCREEN_ADDRESS 0x3C  // Change to 0x3D if your display has that address

// Create an SSD1306 display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define a new small bitmap for the fire symbol
const uint8_t fireBitmap[] PROGMEM = {
  0b00001100, 0b00000000, //    ##
  0b00011110, 0b00000000, //   ####
  0b00111111, 0b00000000, //  ######
  0b01111111, 0b00000000, // #######
  0b01111111, 0b00000000, // #######
  0b11111111, 0b00000000, //#########
  0b11101111, 0b00000000, //### ####
  0b11001111, 0b00000000, //##  ####
  0b10001111, 0b00000000, //#   ###
  0b10000111, 0b00000000, //#   ###
  0b01111100, 0b00000000, // #####
  0b00111100, 0b00000000, //  ####
  0b00011000, 0b00000000, //   ##
};

void setup() {
  // Initialize the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  
  // Set text size and color
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Set cursor position
  display.setCursor(0, 0);

  // Display initial text
  display.println("Hello, World!");
  display.println("Arduino Nano + OLED");

  // Update the display with the buffer content
  display.display();
}

void loop() {
  // Clear the display buffer
  display.clearDisplay();
  
  // Set text size and color for the calorie count
  display.setTextSize(2); // Increase text size here
  display.setTextColor(SSD1306_WHITE);
  
  // Set cursor position for calorie count
  display.setCursor(0, 0);

  // Display calorie count
  display.print("Calories: 106 ");
  
  // Draw the fire symbol after the calorie count
  // The fire symbol is drawn with an x-offset calculated from the length of "Calories: 100 "
  display.drawBitmap(100, 0, fireBitmap, 16, 16, SSD1306_WHITE);

  // Set cursor position for the time text
  display.setCursor(0, 40); // Adjust y position to place it below the calorie count

  // Display time
  display.print("Time:7min");
  
  // Update the display with the buffer content
  display.display();
  
  // Delay for a while before repeating
  delay(1000); // Update every second
}
