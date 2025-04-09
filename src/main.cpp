#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define BUTTON_PIN 12
#define DEBOUNCE_DELAY 20  // milliseconds
#define RS485_TX     17
#define RS485_RX     16
#define RS485_DIR    4
#define BUZZER_PIN 27
#define LED_PIN     5
#define NUM_LEDS    16
#define RESET_COUNT 1000

void updateLEDs(int count);
void victoryEffect();
void showVictoryMessage();
void rainbowEffect();
void playVictoryTone();
void sendRS485Message(String message);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

HardwareSerial RS485Serial(2);

unsigned long lastDebounceTime = 0;
bool lastButtonState = HIGH;
bool buttonState = HIGH;

int counter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button input with internal pull-up
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(RS485_DIR, OUTPUT);
  RS485Serial.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Count: 0");
  display.display();

  strip.begin();
  strip.show();  // Initialize all LEDs to off

  Serial.println("Setup done!");

}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // reset timer on state change
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) { // Button press detected (LOW due to pull-up)
        counter++;
        Serial.print("Button Pressed! Count = ");
        Serial.println(counter);

        if (counter >= RESET_COUNT) {
          victoryEffect();
          counter = 0;
        }

        // Update OLED
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print("Count: ");
        display.println(counter);
        display.display();

        // Beep
        tone(BUZZER_PIN, 1000, 100);  // 1000 Hz for 100ms

        updateLEDs(counter);

        // Mock
        sendRS485Message("COUNT:" + String(counter) + "\n");

      }
    }
  }

  lastButtonState = reading;
}

void updateLEDs(int count) {
  float percent = (count / (float)RESET_COUNT) * NUM_LEDS;
  int ledsToLight = round(percent);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledsToLight) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));  // Green
    } else {
      strip.setPixelColor(i, 0);  // Off
    }
  }
  strip.show();
}

void victoryEffect() {
  playVictoryTone();
  showVictoryMessage();
  rainbowEffect();
}

void showVictoryMessage() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("🎉 Goal Reached!");
  display.display();
}

void rainbowEffect() {
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.ColorHSV((i * 65536 / strip.numPixels() + j * 256) % 65536));
    }
    strip.show();
    delay(30);
  }
}

void playVictoryTone() {
  tone(BUZZER_PIN, 1200); delay(200);
  tone(BUZZER_PIN, 1500); delay(200);
  tone(BUZZER_PIN, 1800); delay(300);
  noTone(BUZZER_PIN);  // Stop the tone
}

void sendRS485Message(String message) {
  digitalWrite(RS485_DIR, HIGH);  // Enable transmit mode
  delayMicroseconds(10);
  RS485Serial.print(message);
  delayMicroseconds(10);
  digitalWrite(RS485_DIR, LOW);   // Back to receive mode
}
