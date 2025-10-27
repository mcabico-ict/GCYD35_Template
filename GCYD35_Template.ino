// =========================================================================
//                             GCYD35_Template.ino
// -------------------------------------------------------------------------
//                The ESP32-3248S035C Template Project (G-Series)
// =========================================================================
//
// Collaborative Creator: Marvin A Cabico (Developer & Systems Integrator) 
// AI Generator: Gemini
//
// -------------------------------------------------------------------------
//                         PROJECT MISSION: NO EXPERTISE GAP
// -------------------------------------------------------------------------
// 
// This template is your first step into embedded systems.
// **The Goal:** To prove that human creativity knows no bounds. This project 
// leverages AI assistance (Gemini) to handle the low-level, technical complexity 
// and drivers, immediately empowering beginners to turn their ideas into hardware 
// reality without the fear of the expertise gap.
//
// =========================================================================
//
//  BOARD MODEL: ESP32-3248S035C (CYD 3.5-inch)
//  HARDWARE:    ST7796 Display Driver / GT911 Capacitive Touch Controller
//
// -------------------------------------------------------------------------
//                         QUICK START GUIDE
// -------------------------------------------------------------------------
//
// 1. INSTALL ARDUINO CORE:
//    - Install the 'esp32' board package by Espressif Systems in the Board Manager.
//
// 2. INSTALL LGFX LIBRARY:
//    - Install the 'LovyanGFX' library via the Arduino Library Manager.
//
// 3. HARDWARE CONFIGURATION:
//    - ALL pin definitions are located in the "1. PIN DEFINITIONS & CONFIGURATION" section below.
//    - To use a different board pinout, ONLY edit these simple #define values.
//    - The core driver logic in 'class LGFX' is guaranteed to work and should NOT be modified.
//
// -------------------------------------------------------------------------
//
//  CURRENT FEATURE: Touch Input Test (5 Buttons + Draw-on-Screen)
//
// =========================================================================

// --- 0. LIBRARIES & CONFIGURATION ---
#include <Arduino.h>
#include <SPI.h> // Required for linking SPIClass methods used by LGFX on ESP32

// LGFX Libraries (used for display and touch)
#define LGFX_USE_V1 
#include <LovyanGFX.hpp>

// --- 1. PIN DEFINITIONS & CONFIGURATION ---
// Display Pins
#define TFT_CS   15  // Chip Select
#define TFT_SCK  14  // SPI Clock
#define TFT_MOSI 13  // MOSI (Data)
#define TFT_MISO 12  // MISO (currently unused in this SPI 3-wire config, but defined for completeness)
#define TFT_DC   2   // Data/Command
#define TFT_BL   27  // Backlight PWM

// Touch (GT911) Pins
#define TOUCH_SDA 33 // I2C Data
#define TOUCH_SCL 32 // I2C Clock
#define TOUCH_INT 36 // Interrupt Pin

// I2C Address for GT911 (Default)
#define TOUCH_I2C_ADDR 0x5D 

// --- 2. HARDWARE CONFIGURATION (CYD 3.5-inch Standard Pinout) ---
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_GT911 _touch_instance;

public:
  LGFX(void)
  {
    // SPI Bus Setup: Uses the defined PIN DEFINITIONS
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = HSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
      cfg.freq_read = 40000000;
      cfg.spi_3wire = true;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = TFT_SCK; 
      cfg.pin_mosi = TFT_MOSI;
      cfg.pin_miso = TFT_MISO;
      cfg.pin_dc = TFT_DC;    
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    // Display Panel Setup (ST7796, 320x480 resolution): Uses the defined PIN DEFINITIONS
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = TFT_CS;   
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.bus_shared = true;
      _panel_instance.config(cfg);
    }
    // Backlight Setup: Uses the defined PIN DEFINITIONS
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = TFT_BL; 
      cfg.invert = false;
      cfg.freq = 1200;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    // Touch Setup (GT911): Uses the defined PIN DEFINITIONS
    {
      auto cfg = _touch_instance.config();
      cfg.pin_int = TOUCH_INT;
      cfg.pin_sda = TOUCH_SDA;
      cfg.pin_scl = TOUCH_SCL;
      cfg.i2c_addr = TOUCH_I2C_ADDR;       
      cfg.i2c_port = 0;          
      cfg.freq = 800000;         
      // Calibration coordinates for 320x480 panel (adjust if needed)
      cfg.x_min = 14;
      cfg.x_max = 310;
      cfg.y_min = 5;
      cfg.y_max = 448;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};
LGFX tft;
// --- END HARDWARE CONFIGURATION ---

// --- BUTTON STRUCTURES AND DEFINITIONS ---

// Define a structure to hold button properties
struct Button {
    int x; // Top-left X coordinate
    int y; // Top-left Y coordinate
    int w; // Width
    int h; // Height
    const char* label;
    uint16_t color;
    uint16_t pressedColor;
    bool isPressed;
};

// Array of Button definitions for a 480x320 landscape screen
Button buttons[] = {
    // x, y, w, h, label, color (Light Gray), pressedColor (Black), isPressed
    {30, 120, 80, 80, "LEFT", TFT_LIGHTGREY, TFT_BLACK, false},     
    {370, 120, 80, 80, "RIGHT", TFT_LIGHTGREY, TFT_BLACK, false},   
    {180, 30, 120, 60, "TOP", TFT_LIGHTGREY, TFT_BLACK, false},   
    {180, 230, 120, 60, "BOTTOM", TFT_LIGHTGREY, TFT_BLACK, false}, 
    {180, 120, 120, 80, "CLEAR", TFT_LIGHTGREY, TFT_BLACK, false}    // Label changed to CLEAR
};
const int NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

// --- FUNCTION PROTOTYPES ---
void drawButton(const Button& btn, bool pressed);
bool isTouchInside(int touchX, int touchY, const Button& btn);


// --- DRAW BUTTON FUNCTION ---
void drawButton(const Button& btn, bool pressed) {
    uint16_t fillColor = pressed ? btn.pressedColor : btn.color;
    uint16_t borderColor = TFT_WHITE;
    uint16_t textColor = TFT_WHITE;
    
    // Draw rounded rectangle button
    tft.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 8, fillColor);
    tft.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 8, borderColor);
    
    // Draw label
    tft.setFont(&fonts::Font2);
    tft.setTextColor(textColor, fillColor);
    tft.setTextDatum(middle_center);
    int centerX = btn.x + (btn.w / 2);
    int centerY = btn.y + (btn.h / 2);
    tft.drawString(btn.label, centerX, centerY);
}

// --- CHECK TOUCH BOUNDS ---
bool isTouchInside(int touchX, int touchY, const Button& btn) {
    return (touchX >= btn.x && touchX <= (btn.x + btn.w) &&
            touchY >= btn.y && touchY <= (btn.y + btn.h));
}


// --- SETUP FUNCTION ---
void setup() {
  Serial.begin(115200); 
  Serial.println("\n--- GCYD35 Gemini Template Start ---");
  
  // 1. Initialize LGFX
  tft.begin();
  tft.setRotation(1); // Set to Landscape (480x320)
  tft.setBrightness(255);
  tft.fillScreen(TFT_BLACK);
  
  // 2. Draw Title
  tft.setFont(&fonts::Font4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(top_center);
  tft.drawString("GCYD35 Touch Test Template", tft.width() / 2, 5);
  
  // 3. Draw all buttons initially (unpressed state)
  for (int i = 0; i < NUM_BUTTONS; i++) {
    drawButton(buttons[i], false);
  }
}

// --- MAIN LOOP ---
void loop() {
  uint16_t x, y;
  
  // Check for touch input
  bool touched = tft.getTouch(&x, &y);
  bool buttonWasHit = false;
  
  // Iterate through all buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool inside = isTouchInside(x, y, buttons[i]);
    
    // Logic: 
    // If touched AND inside AND button was NOT already pressed -> change state to pressed
    if (touched && inside && !buttons[i].isPressed) {
      buttons[i].isPressed = true;
      drawButton(buttons[i], true);
      Serial.printf("Button '%s' Pressed at (%d, %d)\n", buttons[i].label, x, y);

      // --- CLEAR ACTION LOGIC ---
      if (strcmp(buttons[i].label, "CLEAR") == 0) {
        // Clear the entire screen area below the title (approx y=40)
        tft.fillRect(0, 40, tft.width(), tft.height() - 40, TFT_BLACK); 
        
        // Redraw all buttons immediately
        for (int j = 0; j < NUM_BUTTONS; j++) {
             buttons[j].isPressed = false; // Ensure state is reset
             drawButton(buttons[j], false);
        }
      }
      // --- END CLEAR ACTION LOGIC ---
      
      buttonWasHit = true; // Button was hit (either the press or a hold)
    } 
    // If NOT touched OR NOT inside AND button WAS pressed -> change state to released
    else if ((!touched || !inside) && buttons[i].isPressed) {
      buttons[i].isPressed = false;
      drawButton(buttons[i], false);
      Serial.printf("Button '%s' Released\n", buttons[i].label);
    }
    
    // If touched and inside (even if held), we stop the drawing function
    if (touched && inside) {
        buttonWasHit = true;
    }
  }
  
  // --- DRAW DOT LOGIC ---
  // If touched AND no button was hit, draw a green circle
  if (touched && !buttonWasHit) {
    // Draw a small green circle/point at the touch coordinates
    tft.fillCircle(x, y, 2, TFT_GREEN); // Using standard TFT_GREEN
  }
  
  // A small delay to debounce and limit loop rate
  delay(10); 
}
