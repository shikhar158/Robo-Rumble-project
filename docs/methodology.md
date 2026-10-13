# Project methodology

## Goal

Build a remote-controlled 4WD rover for our college Technomanagement Fest. It navigates uneven surfaces using rubber-tread wheels and suspension, while a low battery position, wide stance, and controlled speed improve stability.

## Workflow

1. **Set targets:** confirm fest rules, size, obstacle, battery, and budget limits. Plan roughly 300 x 240 mm chassis, 70 mm wheels, mass below 3 kg.
2. **Design:** draw the chassis and four trailing arms. Place the battery low and centred. Plan four matched spring dampers and travel stops. Keep heavy parts low and symmetric; balance-check left/right and front/rear using two scales.
3. **Select hardware:** use four 12 V geared motors, two drivers (one per side), ESP32, logic buffer, and fused 3S battery. Check current ratings in `hardware-plan.md` and `../electronics/components-list.csv`.
4. **Build:** mount motors/wheels on pivoting arms. Check suspension movement, tire clearance, and wire slack before installing electronics.
5. **Wire:** follow `wiring-diagram.md`; separate motor power from logic power, with common ground. Check polarity and continuity before connecting the battery.
6. **Program:** upload `../firmware/src/main.cpp`. ESP32 hosts a Wi-Fi control page; throttle and steering mix into left/right motor speeds. A command timeout stops motors if commands stop.
7. **Test progressively:** wheels raised first; then clear flat floor; then wheel articulation and small obstacles; finally gentle slopes. Keep speed low and a physical battery disconnect reachable.
8. **Improve and document:** record test conditions, failures, and changes in `test-results.md`; add CAD exports, photos, and a demo link.

## Why the suspension helps reduce tipping

Trailing arms let wheels follow terrain and keep more tires in contact. Springs and damping soften impacts that can bounce the chassis sideways. A wide wheel track and low, centred battery increase the side-to-side support margin. Use moderate preload and limited suspension travel; overly soft suspension lets the body lean and can make rollover easier. Suspension cannot guarantee zero rollover, and it does not right a fully inverted robot. A roll hoop protects electronics; active self-righting requires a separate recovery device.

## Suggested team split

- Mechanical: chassis, swing arms, shocks, wheel alignment and roll hoop.
- Electrical: battery, fuse, buck converter, drivers, wiring and current checks.
- Firmware: ESP32 controls, speed ramp, command timeout and direction setup.
- Testing/documentation: test log, photos, component list and demo.

