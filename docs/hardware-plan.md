# Hardware strategy

## Proposed student build

Build a slow, stable 4WD rover for the fest. Use four 12 V, 100 RPM, 37 mm metal geared motors, one at each wheel, with skid steering: the left pair and right pair are controlled as two sides. Use 65-75 mm rubber-tread wheels with matching 6 mm D-shaft hubs. Target a chassis around 300 x 240 mm and mass below 3 kg.

| Qty | Part | Specification / selection rule |
|---:|---|---|
| 1 | ESP32 DevKit V1 / ESP32-WROOM-32 | Standard ESP32 board with Wi-Fi; firmware hosts a phone control page. |
| 4 | Geared DC motors | 12 V, 100 RPM, 37 mm metal gearbox, 6 mm shaft; require stated stall current about 1.5-2 A each. |
| 4 | Rubber wheels | 65-75 mm, treaded, 6 mm D-bore or matching hubs. |
| 2 | BTS7960 H-bridge modules | One per side; each drives two motors in parallel. Confirm real thermal/current capacity; don't trust peak-current marketing. |
| 1 | 74AHCT125 logic buffer | Powered by 5 V; converts ESP32 3.3 V PWM outputs to 5 V logic. Tie active-low /OE pins low. |
| 1 | 3S Li-ion pack with BMS | 11.1 V nominal, 12.6 V full, about 5 Ah, BMS at least 15 A continuous. Use a matched 12.6 V charger. Buy a protected pack; don't build from loose cells. |
| 1 | 5 V buck converter | Input supports 12.6 V, output 5 V at 2 A or more. |
| 1 | Inline fuse and holder | Install near battery positive. 10 A is a starting point; confirm it survives starts and protects the wire. |
| 4 | Coil-over shocks/springs | Matched 50-70 mm RC shocks, or locally fabricated springs with rubber bump stops; allow 15-25 mm wheel travel. |
| 1 | Chassis plate | 3 mm plywood or 2-3 mm aluminium, about 300 x 240 mm. |
| 4 | Swing arms | 2-3 mm aluminium flat bar or reinforced plywood, 65-75 mm long, M4/M5 pivot bolts and bushes. |
| misc. | Wire and fasteners | 16 AWG battery/motor leads, crimp connectors, heat shrink, M3-M5 hardware, cable ties. |

### Example India pricing

A current Indian listing for a 12 V, 100 RPM, 37 mm metal gearmotor lists the 6 mm shaft and about 1.5-2 A stall current at roughly ₹178 each. Four are about ₹712 before wheels and delivery. An ESP32-WROOM board listing is about ₹448. A 3S 5 Ah pack with BMS is listed near ₹1,590. These are examples, not a quote; stock, delivery and specifications vary. Confirm each part's ratings before purchase.

- [12 V 100 RPM motor listing](https://www.robogears.in/shop/59301266-100-rpm-12v-centre-shaft-dc-geared-motor-high-torque-robotic-motor-2629)
- [ESP32-WROOM board listing](https://stg.robu.in/product/black-diy-kit-electronic-esp32-devkitc-core-boardesp32-development-doard-esp32-wroom-32u-for-arduino%EF%BC%88esp32-wroom-32u%EF%BC%89/)
- [3S 5 Ah battery with BMS listing](https://quartzcomponents.com/collections/latest-products/products/12v-5ah-li-ion-3c-battery-pack-with-1-year-warranty)

Four motors may draw 6-8 A together at stall. The battery, BMS, fuse, connectors, driver modules, and wires must support the measured current. BTS7960 clones vary: use the actual motor stall current to size the modules, provide airflow/heatsinking, and stop if a module or connector gets hot.

## Mechanical layout and suspension

Put the battery low, between the left and right wheels and near the chassis centre. Mount the ESP32 and drivers above it on insulated standoffs. Target a 270 mm wheel track and 260 mm wheelbase; with 70 mm tires, this gives about 300 mm overall width. Keep the 240 mm-wide body inside the tire outer edges. Add a rounded roll hoop around the electronics and battery.

Use one pivoting trailing arm per wheel. Fix the motor to the arm so the wheel moves with it. Fit a coil-over between the arm and chassis at an angle. Add a hard stop so the tire cannot hit the frame or the shock cannot bottom out. Leave a flexible wire loop at each moving motor.

```text
Side view of one corner
 chassis rail
 o-------------------------o upper shock mount
 |                         \\
 | arm pivot o--------------\\ coil-over shock
 |            \\             \\
 |             \\ trailing arm \\____ motor + wheel ( O )
 +------------------------------------------------ ground
```

The suspension reduces tipping risk by letting wheels follow uneven ground instead of losing contact, and by reducing sharp chassis jolts. Keeping more tires planted reduces the chance of a sudden sideways bounce or pivot over an obstacle. Use moderate spring preload and limited travel: a very soft suspension lets the body lean excessively and can make tipping easier. A wide track and low, centred battery further increase the support margin.

Static estimate: `critical tip angle ≈ atan(track / (2 × CG height))`. A 270 mm track and 90 mm CG height gives about 56 degrees in a stationary idealization. Real movement, wheel grip and body lean reduce this margin; treat the formula as a comparison tool, not a safe operating limit. Test slowly on a supervised ramp.

Passive suspension does not make an upside-down robot self-righting. If active recovery is required by the event, add a separate hinged recovery arm with a high-torque servo and its own 5-6 V supply after the driving chassis is stable. The arm must push on the floor to roll the chassis back. Do not describe suspension alone as self-righting.

## Hardware assembly sequence

1. Cut and brace the chassis; mount the four arm pivots and check free movement.
2. Mount motors and wheels. Check clearance at both suspension travel limits.
3. Fit matched shocks/springs, moderate preload and bump stops.
4. Secure the battery low and centre; add insulated electronics mounts and roll hoop.
5. Wire the fuse, buck, ESP32, buffer and drivers using `wiring-diagram.md`.
6. Raise the wheels; test one motor/driver at low PWM and verify every direction.
7. Test on a clear flat floor, then slowly progress to small obstacles. Record results.

## Power and wiring rules

- Use the 3S pack only with 12 V motors and a 12.6 V 3S charger.
- Never power motors from ESP32 5 V or 3.3 V pins.
- Common ground is required between battery negative, drivers, buck, buffer and ESP32.
- Run thick motor wires from a fused battery branch to the drivers; use a separate branch for the buck. Keep motor wiring away from the ESP32 antenna and signal wires.
- Check stall current, pack/BMS rating, connector rating and driver temperature. Remove battery before rewiring; use a stand and reachable battery disconnect for first tests.

