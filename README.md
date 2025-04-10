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
