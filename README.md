# Robo Rumble

A student-built project concept for a compact, Wi-Fi-controlled, four-wheel-drive rover designed for the uneven-terrain challenges at our college Technomanagement Fest.

## Contents

- [At a glance](#at-a-glance)
- [Project goals](#project-goals)
- [How it works](#how-it-works)
- [Mechanical and stability approach](#mechanical-and-stability-approach)
- [Hardware overview](#hardware-overview)
- [Firmware setup and control](#firmware-build-and-control)
- [Build and test workflow](#build-and-test-workflow)
- [Safety notes](#safety-notes)
- [Repository guide](#repository-guide)

## At a glance

- **Drive:** Four 12 V geared motors with skid steering
- **Controller:** ESP32 DevKit with a phone-friendly web joystick
- **Suspension:** Four independent trailing arms with spring/damper units
- **Stability approach:** Wide wheel stance, low and centered battery, controlled speed, and limited suspension travel
- **Firmware safeguards:** Gradual speed ramp and a 450 ms command-loss timeout
- **Build target:** Approximately 300 × 240 mm chassis, 65–75 mm wheels, and under 3 kg (confirm against event rules and final parts)

## Project goals

Robo Rumble is planned as an affordable and repairable student rover. The project aims to:

- Navigate small obstacles and uneven surfaces using four driven wheels.
- Let a driver control the rover wirelessly from a phone browser.
- Reduce abrupt impacts and wheel lift with independent suspension.
- Improve static stability by keeping heavy parts low, balancing the chassis, and using a wide wheel stance.
- Document the design, wiring, firmware, and measured test results so another student team can reproduce and improve the build.

The dimensions and parts in this repository are starting targets. Confirm the final design against the event rules, parts available, and your team budget before fabrication.

## How it works

1. A phone connects to the Wi-Fi access point hosted by the ESP32.
2. The phone browser sends throttle and steering commands to the rover.
3. The ESP32 mixes those commands into left- and right-side motor speeds.
4. Two motor drivers power the motor pairs. A 74AHCT125 buffer raises the ESP32 PWM signal level for the driver inputs.
5. If command updates stop for 450 ms, the firmware ramps the motor commands back to zero.

The battery is planned low and near the chassis center. Trailing-arm suspension helps the wheels follow uneven ground and softens impacts; it cannot guarantee that the rover will never tip and does not right an overturned rover. See [hardware plan](docs/hardware-plan.md) for the stability strategy and the limits of the critical tip-angle estimate.

## Mechanical and stability approach

The proposed chassis uses one pivoting trailing arm at each wheel. A spring/damper unit and a travel stop at each corner help the wheels follow the ground while limiting chassis jolts and preventing the tire or shock from hitting its mechanical limit. Leave flexible wire loops at the moving motors and check that wiring cannot snag through the full suspension travel.

Keep the battery near the chassis centre and as low as practical. Place the left and right components symmetrically where possible, then check front-to-rear and side-to-side balance with the fully equipped rover. A lower centre of gravity and wider track increase the static support margin; moderate spring preload and limited travel help avoid excessive body lean. These measures reduce rollover risk but do not eliminate it. Passive suspension is not a self-righting mechanism.

For design assumptions, the static tip-angle estimate, and assembly details, see the [system design](docs/system-design.md) and [hardware plan](docs/hardware-plan.md).

## Repository guide

| Path | What it contains |
|---|---|
| [`docs/methodology.md`](docs/methodology.md) | Project workflow, team roles, and testing progression |
| [`docs/hardware-plan.md`](docs/hardware-plan.md) | Proposed components, assembly sequence, suspension, and safety notes |
| [`docs/system-design.md`](docs/system-design.md) | System overview, weight optimization, CG calculations, and stability |
| [`docs/wiring-diagram.md`](docs/wiring-diagram.md) | Power and signal connections |
| [`docs/test-results.md`](docs/test-results.md) | Test log; enter actual results here |
| [`electronics/components-list.csv`](electronics/components-list.csv) | Proposed parts list with India-oriented sourcing notes |
| [`firmware/src/main.cpp`](firmware/src/main.cpp) | ESP32 C++ firmware and browser controller |
| [`platformio.ini`](platformio.ini) | PlatformIO target and build configuration |

## Hardware overview

The hardware plan proposes four 12 V, 100 RPM, 37 mm geared motors; two BTS7960 driver modules (one per side); a 3S battery with BMS; a 5 V buck converter; and a 74AHCT125 logic buffer. It also describes the chassis, trailing arms, shocks, fuse, and wiring.

These are proposed selections, not confirmed purchases or validated ratings. Check motor stall current, battery/BMS limits, driver thermal capacity, wire and connector ratings, and local availability against the actual parts you choose. The [hardware plan](docs/hardware-plan.md) and [components list](electronics/components-list.csv) include more detail.

## Firmware build and control

You need [VS Code](https://code.visualstudio.com/) with the PlatformIO IDE extension, or PlatformIO Core CLI, plus a compatible ESP32 DevKit and USB data cable.

From the repository root, build and upload with:

```sh
pio run -e esp32dev
pio run -e esp32dev -t upload
pio device monitor -b 115200
```

After upload:

1. Connect your phone to the ESP32 Wi-Fi network named `RoboRumble`.
2. Open `http://192.168.4.1` in a browser.
3. Drag the joystick to drive and steer; release it or press **STOP** to stop.

Change the access-point password in `firmware/src/main.cpp` before a public demo. The assumed GPIO connections and driver wiring are documented in [`firmware/README.md`](firmware/README.md) and [`docs/wiring-diagram.md`](docs/wiring-diagram.md).

### Controller behavior

The browser joystick sends throttle and steering values. Firmware combines them into left and right drive commands for skid steering, limits the initial PWM duty to 60%, and ramps speed changes instead of applying them instantly. If the phone disconnects or updates stop, a 450 ms timeout sets the target motor commands to zero. These are software safeguards; keep the rover within sight and use a physical battery disconnect during testing.

## Build and test workflow

1. Confirm the fest's size, obstacle, battery, and safety rules.
2. Finalize dimensions and verify the selected motors, drivers, battery, and wiring ratings.
3. Build the chassis and suspension; check wheel clearance and arm travel before installing electronics.
4. Place the battery low and centrally; secure all electronics and route flexible motor wires clear of moving arms.
5. Wire the system with the battery disconnected and verify polarity and continuity.
6. Test one motor at a time with the rover raised and a low speed limit.
7. Test command-loss stopping, then drive on a clear flat surface before attempting small obstacles.
8. Record conditions, outcomes, and follow-up actions in [`docs/test-results.md`](docs/test-results.md).

## Safety notes

- Raise and secure the rover for initial motor tests; keep hands and loose items clear of wheels.
- Disconnect the battery before changing wiring. Fit a correctly sized fuse close to battery positive and keep a reachable disconnect.
- Use a protected battery pack with its matched charger. Do not power motors from the ESP32.
- Make all grounds common as shown in the wiring diagram.
- Stop if a driver, wire, connector, or battery becomes unusually hot. BTS7960 clone modules can have lower continuous capacity than their advertised peak rating.
- Test slowly with supervision. The tip-angle calculation is an idealized comparison, not a safe operating limit.

## Project information

- **Event:** College Technomanagement Fest
- **Team:** _Add team members_
- **Demo video:** _Add a link after recording_

## Contributions

Contributions and build notes are welcome. If you adapt the design, document the parts you used and add measured test results so others can reproduce your build.
