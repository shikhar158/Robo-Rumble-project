# ESP32 firmware (C++ / PlatformIO)

The source is `src/main.cpp`. Build it with PlatformIO for an ESP32-WROOM DevKit / generic ESP32 Dev Module. The project uses Arduino-ESP32 core 3.x APIs, so `platformio.ini` selects the PioArduino PlatformIO platform that provides the current Arduino core.

## Build and upload

1. Open the repository root in VS Code with the PlatformIO IDE extension installed, or install PlatformIO Core CLI.
2. Connect the ESP32 by USB and select the detected serial port if prompted.
3. Build from the repo root with `pio run -e esp32dev`.
4. Upload with `pio run -e esp32dev -t upload`.
5. Open serial monitor with `pio device monitor -b 115200`.
6. Connect your phone to Wi-Fi `RoboRumble` using the password in `src/main.cpp`, then open `http://192.168.4.1`.

Change `AP_PASSWORD` before a public demo. Test with wheels raised first.

## Assumed wiring

- GPIO25 -> left driver RPWM through 74AHCT125
- GPIO26 -> left driver LPWM through 74AHCT125
- GPIO27 -> right driver RPWM through 74AHCT125
- GPIO14 -> right driver LPWM through 74AHCT125
- Both driver's R_EN and L_EN -> 5 V
- ESP32, buffer, driver logic, buck converter and battery negative -> common ground
- Drivers receive motor voltage from the fused battery branch; ESP32 never powers motors

See [`../../docs/wiring-diagram.md`](../../docs/wiring-diagram.md).

The code provides browser joystick input, differential steering, a 60% initial PWM cap, an acceleration ramp, and a 450 ms command timeout. Each driver controls two same-side motors in parallel; size the driver and wiring for their combined stall current. Low-cost BTS7960 clone boards vary, so do not use a peak-current label as a continuous rating. The 74AHCT125 buffers ESP32 3.3 V signals to 5 V logic.
