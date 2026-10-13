# ESP32 firmware

`src/main.ino` is a self-contained controller for an ESP32-WROOM DevKit and two BTS7960 H-bridge modules. The ESP32 hosts a Wi-Fi access point and browser joystick, so a phone can control the rover without a router or separate RC transmitter.

## Assumed wiring

- GPIO25 -> left driver RPWM through 74AHCT125
- GPIO26 -> left driver LPWM through 74AHCT125
- GPIO27 -> right driver RPWM through 74AHCT125
- GPIO14 -> right driver LPWM through 74AHCT125
- Both driver's R_EN and L_EN -> 5 V
- ESP32, buffer, driver logic, buck converter and battery negative -> common ground
- Drivers get motor voltage from battery; ESP32 never powers motors

See [`../../docs/wiring-diagram.md`](../../docs/wiring-diagram.md).

## Upload and use

1. Install Arduino IDE and the Espressif Arduino-ESP32 board package; select a standard ESP32 Dev Module. The sketch uses the Arduino-ESP32 3.x LEDC API.
2. Open `src/main.ino`, review pins, and change `AP_PASSWORD` before a public demo.
3. Upload with wheels raised. Open Serial Monitor at 115200 baud for the access point address.
4. Connect a phone to `RoboRumble` using the password in the sketch, then open `http://192.168.4.1`.
5. Drag the on-screen pad to drive; release or press STOP. If commands stop arriving for 450 ms, the firmware returns the target to zero.

Initial speed is capped at 60% PWM and acceleration is ramped. Test on a stand first and keep a reachable physical battery disconnect.

Each driver controls two same-side motors in parallel; choose modules and wiring for the pair's combined stall current. Cheap BTS7960 boards vary, so do not use their peak-current label as a continuous rating. The 74AHCT125 makes the 3.3 V ESP32 outputs 5 V logic. This demo control is not a safety-rated radio link; test slowly and supervised.
