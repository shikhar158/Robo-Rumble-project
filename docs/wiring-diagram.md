# Wiring diagram

Assumed hardware: two BTS7960 modules (one per side), four 12 V motors, a 3S battery, 5 V buck converter, and one 74AHCT125 buffer. Confirm labels on your exact modules.

## Power wiring

```mermaid
flowchart LR
  BAT[3S 11.1 V battery with BMS] --> FUSE[10 A fuse near battery +]
  FUSE --> SPLIT[Motor power split]
  SPLIT --> DL[BTS7960 left B+]
  SPLIT --> DR[BTS7960 right B+]
  SPLIT --> BUCK[5 V buck converter]
  BUCK --> ESP[ESP32 VIN / 5V]
  BUCK --> BUF[74AHCT125 VCC]
  BUCK --> DV[Both driver logic VCC]
  BATN[Battery negative] --> GND[Common ground point]
  GND --> DLN[Both driver B- and GND]
  GND --> DRN[Both driver B- and GND]
  GND --> BUCKG[Buck GND]
  GND --> ESPG[ESP32 GND]
  GND --> BUFG[Buffer GND]
  DL --> ML[Left front + rear motors parallel]
  DR --> MR[Right front + rear motors parallel]
```

## Control signals

```mermaid
flowchart LR
  P25[ESP32 GPIO25] --> A1[74AHCT125 A1]
  P26[ESP32 GPIO26] --> A2[74AHCT125 A2]
  P27[ESP32 GPIO27] --> A3[74AHCT125 A3]
  P14[ESP32 GPIO14] --> A4[74AHCT125 A4]
  A1 --> LRPWM[Left BTS RPWM]
  A2 --> LLPWM[Left BTS LPWM]
  A3 --> RRPWM[Right BTS RPWM]
  A4 --> RLPWM[Right BTS LPWM]
  V5[5 V] --> EN[Both R_EN and L_EN on each driver HIGH]
  LOW[Ground] --> OE[All buffer active-low /OE pins LOW]
```

## Pin table

| ESP32 / supply | Connection |
|---|---|
| GPIO25 | Buffer A1; buffer Y1 to left driver RPWM |
| GPIO26 | Buffer A2; buffer Y2 to left driver LPWM |
| GPIO27 | Buffer A3; buffer Y3 to right driver RPWM |
| GPIO14 | Buffer A4; buffer Y4 to right driver LPWM |
| 5 V buck output | ESP32 VIN/5V, buffer VCC, both driver VCC, each driver's R_EN and L_EN HIGH |
| Common ground | ESP32 GND, buffer GND, driver logic GND and B-, buck GND, battery negative |
| Fused battery positive | Both driver B+ and buck input positive |
| Left driver M+/M- | Left-front and left-rear motors in parallel |
| Right driver M+/M- | Right-front and right-rear motors in parallel |

The 74AHCT125 buffers ESP32 3.3 V PWM to 5 V logic because clone BTS7960 boards may use 5 V logic buffers. Tie all four active-low `/OE` inputs to ground. Do not connect battery voltage to an ESP32 pin. If a wheel spins backward relative to its partner on that side, swap that motor's wires. If forward/reverse is reversed, swap both motors on that side or invert that side in firmware. Keep motors on a side wired in parallel, never series.

