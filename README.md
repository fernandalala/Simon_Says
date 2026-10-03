# Simon Says Embedded Game

Hardware memory game implemented in C++ for Arduino / ESP32.

  Overview
- Microcontroller-based memory game with incremental sequence difficulty.
- Synchronized visual (LEDs) and audio (buzzer frequencies) feedback.
- Software button debouncing using system timers (`millis()`).
- UART Serial interface for telemetry and debugging.
  
  Components
- 1x ESP32 / Arduino compatible microcontroller
- 4x LEDs
- 4x Push buttons
- 4x Resistors (220Ω - 330Ω)
- 1x Passive buzzer

  Pin Configuration
Outputs (LEDs):
- LED 0: GPIO 26 
- LED 1: GPIO 27 
- LED 2: GPIO 14 
- LED 3: GPIO 12 
Inputs (Buttons):
- Button 0: GPIO 13 
- Button 1: GPIO 32
- Button 2: GPIO 33 
- Button 3: GPIO 25 
Audio:
- Passive Buzzer: GPIO 15 
 
  Audio Frequencies
- Tone 0: 262 Hz
- Tone 1: 330 Hz
- Tone 2: 392 Hz
- Tone 3: 440 Hz

 Setup & Execution
1. Wire components per the pinout table.
2. Open code in VS Code (PlatformIO) or Arduino IDE.
3. Flash firmware to the microcontroller.
4. Open Serial Monitor at 460800 baud.
5. Press any button to initialize the game loop.
