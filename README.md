# Robo Rumble

Robo Rumble is a remote-controlled, four-wheel-drive all-terrain rover developed for our college's Technomanagement Fest. It uses rubber-tread wheels and spring suspension to travel over uneven ground. The design keeps the battery low and centred to improve stability and protects the electronics with a roll hoop.

## Design features

- ESP32 Wi-Fi browser controller
- Four-wheel drive with differential/skid steering
- Independent trailing-arm spring suspension
- Low-centred battery and wide wheel stance to reduce rollover risk
- Motor command timeout and gradual acceleration in firmware

Suspension helps keep wheels in contact and soften impacts; it does not by itself right an upside-down robot. See the [hardware plan](docs/hardware-plan.md) and [system design](docs/system-design.md) for the stability and weight-distribution strategy.

## Build workflow

1. Set size and obstacle targets based on fest rules.
2. Build a compact chassis with four pivoting wheel arms and matched springs/dampers.
3. Install four 12 V geared motors, high-traction wheels, two motor drivers, and a fused battery.
4. Wire the ESP32 and drivers using the [wiring diagram](docs/wiring-diagram.md).
5. Build and upload `firmware/src/main.cpp` with PlatformIO, then test with wheels raised.
6. Test on flat ground, then gradually add uneven terrain and record results.

## Repository map

- `docs/hardware-plan.md` — hardware choices, stability strategy, assembly sequence.
- `docs/methodology.md` — project workflow.
- `docs/system-design.md` — system overview.
- `docs/wiring-diagram.md` — Mermaid diagrams and pin connections.
- `cad/component-placement.svg` — top and side component placement diagram.
- `cad/component-layout.scad` — editable 3D layout model.
- `docs/test-results.md` — test log.
- `electronics/components-list.csv` — proposed India-oriented parts list.
- `firmware/src/main.cpp` — ESP32 C++ controller.
- `platformio.ini` — PlatformIO board and build configuration.
- `cad/` — chassis, suspension, and component placement design files.

## Controller

The ESP32 creates Wi-Fi named `RoboRumble`. Connect a phone and open `http://192.168.4.1`. The password is set in `firmware/src/main.cpp`; change it before a public demo. If commands stop arriving, firmware stops the motors after a timeout.

## Safety

Use a 3S pack with an integrated BMS and its specified charger. Fit a fuse near battery positive. Check motor stall current and driver temperature; low-cost BTS7960 clone boards may not meet the headline peak current printed on a listing. Disconnect the battery before rewiring. Raise wheels for initial motor tests and keep a reachable battery disconnect.

## Team and event

- Team members: _Add names_
- Event: College Technomanagement Fest
- Demo video: _Add link after recording_

Example prices and availability change. See the component list and [hardware plan](docs/hardware-plan.md) for India-oriented purchase references.


