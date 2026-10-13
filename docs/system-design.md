# System design

```text
Phone browser --Wi-Fi--> ESP32 access point / control page
                               |
                      four 3.3 V PWM signals
                               v
                         74AHCT125 buffer
                               |
                      5 V PWM signals
                               v
                  left BTS7960   right BTS7960
                  /          \   /          \
        left-front/rear motors     right-front/rear motors
                    \                 /
                    fused 3S battery
                           |
                      5 V buck
                           |
                     ESP32 + logic
```

- **Mobility:** four 12 V geared motors, 4WD skid steering and rubber-tread wheels.
- **Suspension:** four spring/damper trailing arms with limited travel and bump stops.
- **Control:** ESP32 Wi-Fi access point hosts a phone browser joystick; no router or separate transmitter is needed for the basic demo.
- **Power:** fused 3S battery goes to motor drivers; a separate buck supplies 5 V logic.
- **Stability:** low centred battery, wide wheel track, moderate speed and roll hoop.

See [hardware plan](hardware-plan.md) and [wiring diagram](wiring-diagram.md) for dimensions and pin connections.
