// Robo Rumble conceptual component placement model (OpenSCAD).
// Units: millimetres. Simplified blocks are layout envelopes, not part geometry.
// Open this file in OpenSCAD, press F5 to preview, then F6 / Export STL if desired.

$fn = 48;
chassis_length = 300;
chassis_width = 240;
chassis_thickness = 3;
wheel_diameter = 70;
wheel_width = 30;
wheelbase = 260;
track = 270;
wheel_z = wheel_diameter / 2;
chassis_z = 50;

module block_at(pos, size, col) {
    color(col) translate([pos[0]-size[0]/2, pos[1]-size[1]/2, pos[2]-size[2]/2]) cube(size);
}

module wheel_at(x, y, z) {
    color("#374151") translate([x, y, z]) rotate([90,0,0]) cylinder(h=wheel_width, d=wheel_diameter, center=true);
}

// Chassis is centered at the origin in X/Y.
color("#93c5fd") translate([-chassis_length/2,-chassis_width/2,chassis_z])
    cube([chassis_length,chassis_width,chassis_thickness]);

// Four wheels and motor envelopes, arranged symmetrically.
for (x = [-wheelbase/2, wheelbase/2]) {
    for (y = [-track/2, track/2]) {
        wheel_at(x,y,wheel_z);
        // Simple 37 mm motor/gearbox envelope inside each wheel position.
        block_at([x + (x < 0 ? 18 : -18), y + (y < 0 ? 20 : -20), chassis_z-8], [46,37,37], "#fca5a5");
        // Swing arm from chassis pivot toward the wheel.
        color("#b45309") hull() {
            translate([x*0.68,y*0.72,chassis_z-13]) sphere(d=9);
            translate([x,y,wheel_z]) sphere(d=9);
        }
    }
}

// Main battery is the largest single mass; centered and low on the chassis.
block_at([0,0,chassis_z+chassis_thickness+20], [110,70,40], "#fbbf24");

// Light electronics sit above the battery on a small upper plate.
color("#c4b5fd") translate([-48,-42,chassis_z+chassis_thickness+42]) cube([96,84,3]);
block_at([-20,0,chassis_z+chassis_thickness+50], [38,28,12], "#a78bfa"); // ESP32
block_at([26,18,chassis_z+chassis_thickness+50], [40,28,12], "#86efac"); // driver/logic envelope
block_at([24,-25,chassis_z+chassis_thickness+50], [30,18,10], "#67e8f9"); // buck/fuse

// Simple roll hoop envelope: two light side bars above the component deck.
color("#64748b") for (y = [-chassis_width/2+12, chassis_width/2-12]) {
    translate([-chassis_length/2+20,y,chassis_z+chassis_thickness+48]) cube([4,4,80]);
    translate([chassis_length/2-24,y,chassis_z+chassis_thickness+48]) cube([4,4,80]);
    translate([-chassis_length/2+20,y-2,chassis_z+chassis_thickness+124]) cube([chassis_length-40,4,4]);
}


