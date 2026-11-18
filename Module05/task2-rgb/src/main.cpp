#include <Arduino.h>

// Task 2 — RGB LED with a serial "R G B" REPL.
//
// Board: MH-ET LIVE ESP32 MiniKit (`mhetesp32minikit`).
// This board does not break out GPIO 16/17/18, so the course wiring is mapped to
// GPIO 13 (red), 14 (green) and 27 (blue). Each channel has its own 220 ohm
// resistor; the common pin goes to GND for a common-cathode part.
//
// For a common-anode part: wire the common pin to 3V3 and set COMMON_ANODE to
// true. The PWM duty is then inverted, so "255" still means "full colour".
const bool COMMON_ANODE = false;

const int PIN_R = 13;
const int PIN_G = 14;
const int PIN_B = 27;

const int DUTY_MAX = 255;

namespace {

void applyChannel(int pin, int value) {
  analogWrite(pin, COMMON_ANODE ? (DUTY_MAX - value) : value);
}

void applyRgb(int r, int g, int b) {
  applyChannel(PIN_R, r);
  applyChannel(PIN_G, g);
  applyChannel(PIN_B, b);
}

bool inRange(int v) { return v >= 0 && v <= DUTY_MAX; }

// Accepts exactly three integers separated by whitespace and nothing else.
bool parseRgb(const char *line, int &r, int &g, int &b) {
  int values[3] = {0, 0, 0};
  int consumed = 0;
  const int matched = sscanf(line, "%d %d %d %n", &values[0], &values[1],
                             &values[2], &consumed);
  if (matched != 3) {
    return false;
  }
  // Anything after the third number must be whitespace only.
  for (const char *rest = line + consumed; *rest != '\0'; ++rest) {
    if (!isspace(static_cast<unsigned char>(*rest))) {
      return false;
    }
  }
  if (!inRange(values[0]) || !inRange(values[1]) || !inRange(values[2])) {
    return false;
  }
  r = values[0];
  g = values[1];
  b = values[2];
  return true;
}

void handleLine(String &line) {
  line.trim();
  if (line.length() == 0) {
    return;
  }
  int r = 0;
  int g = 0;
  int b = 0;
  if (line.equalsIgnoreCase("off")) {
    applyRgb(0, 0, 0);
    Serial.println("applied R=0 G=0 B=0");
  } else if (parseRgb(line.c_str(), r, g, b)) {
    applyRgb(r, g, b);
    Serial.printf("applied R=%d G=%d B=%d\n", r, g, b);
  } else {
    Serial.printf("error: '%s' is not three integers in 0..255 (got %s)\n",
                  line.c_str(), line.c_str());
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  applyRgb(0, 0, 0);

  Serial.println();
  Serial.println("Task 2: RGB LED, serial 'R G B' REPL");
  Serial.printf("Pins: R=GPIO%d G=GPIO%d B=GPIO%d, %s\n", PIN_R, PIN_G, PIN_B,
                COMMON_ANODE ? "common anode" : "common cathode");
  Serial.println("Send three integers 0..255, e.g. '255 0 0'. Commands are case-insensitive.");
  Serial.println("Enter 'off' to turn every channel off.");
  Serial.print("rgb> ");
}

void loop() {
  static String line;
  static bool swallowLf = false;

  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());

    // Windows terminals send a bare CR on Enter, so accept CR, LF and CRLF.
    if (c == '\n' && swallowLf) {
      swallowLf = false;  // second half of a CRLF pair
      continue;
    }
    swallowLf = false;

    if (c == '\r' || c == '\n') {
      swallowLf = (c == '\r');
      handleLine(line);
      line = "";
      Serial.print("rgb> ");
      continue;
    }

    if (c == '\t') {
      if (line.length() < 64) {
        line += ' ';
      }
      continue;
    }

    // Drop control bytes and line noise: a stray NUL (0x00) arrives when the
    // port opens, and String::length()/c_str() disagree about it.
    if (c < 0x20 || c > 0x7e) {
      continue;
    }
    if (line.length() < 64) {
      line += c;
    }
  }
}
