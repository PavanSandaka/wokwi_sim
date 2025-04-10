## ESP32-Based Person Counter System

This project implements a smart person counter using an ESP32 microcontroller. The counter system is triggered via a button press, and incorporates various feedback mechanisms including:

- OLED display to show the live count

- Buzzer feedback for every press and celebration tone on goal

- Addressable RGB LED strip to show visual progress toward a configurable goal (default: 1000 people)

- RS485 communication to report the counter to a master system (simulated)

💡 The project was developed and simulated using Wokwi (an online simulator) along with Visual Studio Code and the PlatformIO extension for writing, building, and testing the firmware — no physical hardware was required.



## Firmware Features

1. Software debounce to prevent multiple triggers on a single press

2. OLED screen updates the count in real-time

3. Buzzer beep on every valid press and multi-tone celebration at goal

4. RGB LED percentage display lights up progressively based on total count

5. RS485 communication sends a message back to master controller on every update

6. Victory effect with special tone and rainbow LED animation once 1000 is reached, followed by reset



## Flow Chart To Control Logic

┌────────────┐
│ Start      │
└────┬───────┘
     ↓
┌──────────────┐
│ Initialize   │
│ Peripherals  │
└────┬─────────┘
     ↓
┌──────────────┐
│ Read Button  │◄────┐
└────┬─────────┘     │
     ↓               │
┌──────────────────────┐
│ Is Button Pressed?   │─No─┐
└────┬─────────────────┘    │
     ↓ Yes                  │
┌──────────────────────────────┐
│ Debounce: Valid Press?       │
└────┬─────────────────────────┘
     ↓ Yes
┌────────────────────┐
│ Increment Counter  │
└────┬───────────────┘
     ↓
┌────────────────────┐
│ Update OLED Display│
├────────────────────┤
│ Beep Buzzer        │
├────────────────────┤
│ Update LEDs        │
├────────────────────┤
│ Send RS485 Msg     │
└────┬───────────────┘
     ↓
┌──────────────────────────────┐
│ Counter == 1000?             │
└────┬──────────────┬──────────┘
     │Yes           │No
     ↓              ↓
┌──────────────┐    │
│ Play Victory │    │
│ Sound/LED    │    │
│ Reset Counter│    │
└──────────────┘    │
     ↑              │
     └──────────────┘ (Back to loop)


## Pseudo Code

BEGIN

Initialize OLED, Button, Buzzer, RGB LED Strip, RS485 Serial

SET counter = 0
SET lastButtonState = HIGH
SET debounceDelay = 20 ms

LOOP:

    READ current button state

    IF current state != lastButtonState THEN
        SET lastDebounceTime = current time

    IF (current time - lastDebounceTime) > debounceDelay THEN
        IF current state != buttonState THEN
            UPDATE buttonState

            IF button is pressed THEN
                INCREMENT counter

                IF counter >= 1000 THEN
                    Play victory buzzer tones
                    Show victory message on OLED
                    Rainbow LED effect
                    RESET counter to 0

                ELSE
                    Update OLED with current count
                    Beep buzzer
                    Update LED strip to show percentage
                    Send message over RS485

    UPDATE lastButtonState = current state

END LOOP

## Technical Specifications
### Hardware Components (Simulated)

- Microcontroller: ESP32 Dev Module (Wokwi-compatible)

- Button: Push-button with internal pull-up configuration

- OLED Display: SSD1306 128x64 pixels (I2C interface)

- Buzzer: Active buzzer connected via GPIO

- LED Strip: Addressable RGB LEDs (WS2812, 16 LEDs, driven via a single data pin)

- RS485 Communication: Simulated using UART2 pins with DE/RE control pin

### Communication Protocols

- I2C — Used for communication with the SSD1306 OLED display

- GPIO (Digital I/O) — Used for button input and buzzer output

- Single-wire (NeoPixel protocol) — Used for WS2812 addressable RGB LED strip

- UART (RS485 simulation) — Used for serial communication with a master device (TX, RX, and DE pins)

### Required Libraries

- Adafruit_SSD1306 — For OLED display

- Adafruit_GFX — Graphics library for OLED

- Adafruit_NeoPixel — For WS2812 RGB LED control


*Make sure these libraries are installed and declared in your platformio.ini if using PlatformIO.


## Alternative Detection Methods

To make the person counting system more robust and automatic, alternative sensing methods can replace or complement the manual button press:

1. IR Proximity Sensor: Detects nearby objects or people using reflected infrared light. Suitable for basic entry/exit counters.

2. Ultrasonic Sensor: Uses sound waves to detect presence and distance changes. Can detect a person walking past a doorway.

3. ESP32-CAM + Computer Vision: Uses machine learning to detect people using object detection models. Great for intelligent automation.

And soon...These methods offer automation, reduce human error, and enable scalability.
