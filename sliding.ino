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

// Scrolling text parameters
const char* scrollingText = "Correct your posture for proper guidance of the yoga workout. Kindly go through the video tutorial from app.";
int textWidth;

void setup() {
  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  
  // Set text size and color for the initial display
  display.setTextSize(3); // Set a larger text size for visibility
  display.setTextColor(SSD1306_WHITE);
  
  // Display "Yogifi"
  String yogifiText = "Yogifi";
  int yogifiWidth = yogifiText.length() * 18; // Estimate width for size 3
  display.setCursor((SCREEN_WIDTH - yogifiWidth) / 2, SCREEN_HEIGHT / 2 - 12); // Adjust for size
  display.println(yogifiText);

  // Display "Connect"
  String connectText = "Connect";
  int connectWidth = connectText.length() * 18; // Estimate width for size 3
  display.setCursor((SCREEN_WIDTH - connectWidth) / 2, SCREEN_HEIGHT / 2 + 8); // Adjust for size
  display.println(connectText);

  // Update the display with the buffer content
  display.display();
  
  delay(2000); // Delay for 2 seconds before scrolling

  // Prepare for scrolling text
  display.setTextSize(2); // Set a larger text size for scrolling
  textWidth = strlen(scrollingText) * 12; // Estimate width for size 2
}

void loop() {
  // Scrolling text
  for (int x = SCREEN_WIDTH; x > -textWidth; x--) {
    display.clearDisplay();
    
    // Set cursor for scrolling text
    display.setCursor(x, SCREEN_HEIGHT - 16); // Adjust y position as needed
    display.print(scrollingText);
    
    // Update the display
    display.display();
    delay(50); // Adjust delay for speed of scrolling
  }

  // Optionally, reset to start again after scrolling
  delay(1000); // Pause before restarting the loop
}
