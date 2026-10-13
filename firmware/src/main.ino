#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Robo Rumble: ESP32 web remote for two BTS7960 motor-driver modules.
// Target: ESP32-WROOM DevKit and Arduino-ESP32 core 3.x.
// Left and right motor pairs are wired in parallel through their own driver.
// GPIO signals must pass through the 74AHCT125 buffer before BTS PWM inputs.

const char* AP_SSID = "RoboRumble";
const char* AP_PASSWORD = "RumbleBot26"; // Change before public demos; 8+ characters.

constexpr uint8_t LEFT_RPWM = 25;
constexpr uint8_t LEFT_LPWM = 26;
constexpr uint8_t RIGHT_RPWM = 27;
constexpr uint8_t RIGHT_LPWM = 14;
constexpr uint32_t PWM_FREQUENCY = 18000;
constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint16_t PWM_MAX = 255;
constexpr int MAX_COMMAND = 100;
constexpr int MAX_DUTY_PERCENT = 60; // Conservative initial speed cap.
constexpr uint32_t COMMAND_TIMEOUT_MS = 450;
constexpr uint32_t RAMP_INTERVAL_MS = 10;
constexpr int RAMP_STEP = 4;

WebServer server(80);
int targetLeft = 0, targetRight = 0;
int currentLeft = 0, currentRight = 0;
uint32_t lastCommandMs = 0, lastRampMs = 0;

const char CONTROL_PAGE[] PROGMEM = R"rawliteral(
<!doctype html><html><head><meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no"><title>Robo Rumble</title><style>
*{box-sizing:border-box}body{font-family:Arial;background:#101827;color:white;margin:0;text-align:center;touch-action:none}main{max-width:520px;margin:auto;padding:18px}h1{font-size:1.5rem}.pad{width:min(82vw,360px);height:min(82vw,360px);border-radius:50%;background:#263449;border:3px solid #61738d;margin:22px auto;position:relative;touch-action:none}#knob{width:92px;height:92px;border-radius:50%;background:#26a269;border:3px solid #baf3d4;position:absolute;left:50%;top:50%;transform:translate(-50%,-50%)}.stop{background:#c62828;color:white;border:0;border-radius:9px;padding:13px 32px;font-size:1.1rem;font-weight:bold}
</style></head><body><main><h1>Robo Rumble</h1><p>Drag the pad: up/down drives, left/right steers. Release to stop.</p><div id="pad" class="pad"><div id="knob"></div></div><button class="stop" id="stop">STOP</button><p id="status">Connecting…</p></main><script>
const pad=document.getElementById('pad'),knob=document.getElementById('knob'),statusText=document.getElementById('status');let active=false,throttle=0,steer=0,lastSend=0;
function update(e){const r=pad.getBoundingClientRect(),cx=r.left+r.width/2,cy=r.top+r.height/2,lim=r.width*.36;let dx=e.clientX-cx,dy=e.clientY-cy,m=Math.hypot(dx,dy);if(m>lim){dx*=lim/m;dy*=lim/m}steer=Math.round(dx/lim*100);throttle=Math.round(-dy/lim*100);knob.style.left=`${50+dx/r.width*100}%`;knob.style.top=`${50+dy/r.height*100}%`}
function center(){active=false;throttle=0;steer=0;knob.style.left='50%';knob.style.top='50%';send(true)}
pad.addEventListener('pointerdown',e=>{active=true;pad.setPointerCapture(e.pointerId);update(e)});pad.addEventListener('pointermove',e=>{if(active)update(e)});pad.addEventListener('pointerup',center);pad.addEventListener('pointercancel',center);document.getElementById('stop').addEventListener('pointerdown',e=>{e.preventDefault();center()});
async function send(force=false){const now=Date.now();if(!force&&now-lastSend<100)return;lastSend=now;try{await fetch(`/drive?t=${throttle}&s=${steer}`,{cache:'no-store'});statusText.textContent='Connected • release or press STOP to stop';}catch(_){statusText.textContent='Link lost • firmware timeout will stop motors';}}
setInterval(()=>send(),120);window.addEventListener('blur',center);document.addEventListener('visibilitychange',()=>{if(document.hidden)center()});
</script></body></html>
)rawliteral";

int clampCommand(int value) { return constrain(value, -MAX_COMMAND, MAX_COMMAND); }

void setSide(uint8_t forwardPin, uint8_t reversePin, int command) {
  command = clampCommand(command);
  const uint32_t duty = (uint32_t)(abs(command) * MAX_DUTY_PERCENT * PWM_MAX / 10000);
  if (command > 0) {
    ledcWrite(forwardPin, duty);
    ledcWrite(reversePin, 0);
  } else if (command < 0) {
    ledcWrite(forwardPin, 0);
    ledcWrite(reversePin, duty);
  } else {
    ledcWrite(forwardPin, 0); // Coast
    ledcWrite(reversePin, 0);
  }
}

void applyMotors() {
  setSide(LEFT_RPWM, LEFT_LPWM, currentLeft);
  setSide(RIGHT_RPWM, RIGHT_LPWM, currentRight);
}

void stopNow() {
  targetLeft = targetRight = currentLeft = currentRight = 0;
  applyMotors();
}

void handleRoot() { server.send_P(200, "text/html", CONTROL_PAGE); }

void handleDrive() {
  if (!server.hasArg("t") || !server.hasArg("s")) {
    server.send(400, "text/plain", "Missing throttle/steering");
    return;
  }
  const int throttle = clampCommand(server.arg("t").toInt());
  const int steering = clampCommand(server.arg("s").toInt());
  int left = throttle + steering;
  int right = throttle - steering;
  const int peak = max(abs(left), abs(right));
  if (peak > MAX_COMMAND) {
    left = left * MAX_COMMAND / peak;
    right = right * MAX_COMMAND / peak;
  }
  targetLeft = clampCommand(left);
  targetRight = clampCommand(right);
  lastCommandMs = millis();
  server.send(200, "text/plain", "OK");
}

void handleStop() {
  stopNow();
  lastCommandMs = 0;
  server.send(200, "text/plain", "STOPPED");
}

void rampToward(int& current, int target) {
  if (current < target) current = min(current + RAMP_STEP, target);
  else if (current > target) current = max(current - RAMP_STEP, target);
}

void setup() {
  Serial.begin(115200);
  pinMode(LEFT_RPWM, OUTPUT); pinMode(LEFT_LPWM, OUTPUT);
  pinMode(RIGHT_RPWM, OUTPUT); pinMode(RIGHT_LPWM, OUTPUT);
  ledcAttach(LEFT_RPWM, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(LEFT_LPWM, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(RIGHT_RPWM, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(RIGHT_LPWM, PWM_FREQUENCY, PWM_RESOLUTION);
  stopNow();

  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    Serial.println("Could not start Wi-Fi access point");
  }
  Serial.print("Wi-Fi: "); Serial.println(AP_SSID);
  Serial.print("Open http://"); Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/drive", HTTP_GET, handleDrive);
  server.on("/stop", HTTP_GET, handleStop);
  server.begin();
}

void loop() {
  server.handleClient();
  const uint32_t now = millis();
  if (lastCommandMs == 0 || now - lastCommandMs > COMMAND_TIMEOUT_MS) {
    targetLeft = targetRight = 0;
  }
  if (now - lastRampMs >= RAMP_INTERVAL_MS) {
    lastRampMs = now;
    rampToward(currentLeft, targetLeft);
    rampToward(currentRight, targetRight);
    applyMotors();
  }
}
