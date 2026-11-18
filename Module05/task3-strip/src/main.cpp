#include <Arduino.h>
#include <NeoPixelBusLg.h>

// Task 3 — 8-pixel WS2812B strip with a serial "index R G B" REPL.
//
// Board: MH-ET LIVE ESP32 MiniKit (`mhetesp32minikit`).
// Wiring: strip VDC -> board 5V, strip GND -> board GND, strip DIN -> GPIO 13.
// GPIO 13 is used because this board does not break out the module's GPIO 16.
//
// Driver choice: NeoEsp32Rmt0Ws2812xMethod puts the 800 kHz NRZ waveform on the
// RMT peripheral in hardware, so Wi-Fi/BLE interrupts cannot corrupt a frame.
// Channel 0 is free here; the I2S methods are the alternative. FastLED is
// deliberately not used.
//
// NeoPixelBusLg (not plain NeoPixelBus) is required for SetLuminance(), and it
// lives in its own header, NeoPixelBusLg.h.
const uint16_t LED_COUNT = 8;
const int DATA_PIN = 13;

// 8 LEDs at full white would be ~480 mA, already the USB 2.0 limit before the
// ESP32's own draw. Luminance 80 keeps the strip inside the USB budget.
const uint8_t LUMINANCE = 80;

NeoPixelBusLg<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod, NeoGammaNullMethod>
    strip(LED_COUNT, DATA_PIN);

namespace {

bool inRange(int v) { return v >= 0 && v <= 255; }

// Accepts exactly `count` (3 or 4) integers separated by whitespace and nothing
// else. %n is only assigned when the whole pattern matched, so the format is
// chosen per count instead of always asking for four numbers.
bool parseInts(const char *line, int *values, int count) {
  int consumed = 0;
  int matched = 0;
  if (count == 3) {
    matched = sscanf(line, "%d %d %d %n", &values[0], &values[1], &values[2],
                     &consumed);
  } else if (count == 4) {
    matched = sscanf(line, "%d %d %d %d %n", &values[0], &values[1],
                     &values[2], &values[3], &consumed);
  } else {
    return false;
  }
  if (matched != count) {
    return false;
  }
  for (const char *rest = line + consumed; *rest != '\0'; ++rest) {
    if (!isspace(static_cast<unsigned char>(*rest))) {
      return false;
    }
  }
  return true;
}

void applyAll(const RgbColor &color) {
  strip.ClearTo(color);
  strip.Show();
}

void handleLine(String &line) {
  line.trim();
  if (line.length() == 0) {
    return;
  }

  if (line.equalsIgnoreCase("clear")) {
    applyAll(RgbColor(0, 0, 0));
    Serial.println("cleared all pixels");
    return;
  }

  // "all R G B" sets every pixel at once; this is what the power measurement
  // needs, since eight separate commands would not be a single steady state.
  if (line.length() >= 3 && (line.length() == 3 || line[3] == ' ') &&
      tolower(line[0]) == 'a' && tolower(line[1]) == 'l' &&
      tolower(line[2]) == 'l') {
    int v[4] = {0, 0, 0, 0};
    if (parseInts(line.c_str() + 3, v, 3) && inRange(v[0]) && inRange(v[1]) &&
        inRange(v[2])) {
      applyAll(RgbColor(v[0], v[1], v[2]));
      Serial.printf("applied all R=%d G=%d B=%d\n", v[0], v[1], v[2]);
      return;
    }
    Serial.printf("error: '%s' needs three values 0..255 after 'all'\n",
                  line.c_str());
    return;
  }

  int v[4] = {0, 0, 0, 0};
  if (parseInts(line.c_str(), v, 4)) {
    if (v[0] < 0 || v[0] >= LED_COUNT) {
      Serial.printf("error: index %d out of range 0..%u\n", v[0], LED_COUNT - 1);
      return;
    }
    if (!inRange(v[1]) || !inRange(v[2]) || !inRange(v[3])) {
      Serial.println("error: values must be 0..255");
      return;
    }
    strip.SetPixelColor(v[0], RgbColor(v[1], v[2], v[3]));
    strip.Show();
    Serial.printf("applied index=%d R=%d G=%d B=%d\n", v[0], v[1], v[2], v[3]);
    return;
  }

  Serial.printf(
      "error: '%s' is not 'index R G B' (index 0..%u, values 0..255), "
      "'all R G B' or 'clear'\n",
      line.c_str(), LED_COUNT - 1);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  strip.Begin();
  strip.SetLuminance(LUMINANCE);
  applyAll(RgbColor(0, 0, 0));

  Serial.println();
  Serial.println("Task 3: 8-pixel WS2812B, serial REPL");
  Serial.printf("Data pin: GPIO%d, pixels: %u, luminance: %u\n", DATA_PIN,
                LED_COUNT, LUMINANCE);
  Serial.println("Driver: NeoPixelBus NeoEsp32Rmt0Ws2812xMethod (RMT channel 0)");
  Serial.println("Power note: 8 x 60 mA = 480 mA at full white; luminance is capped.");
  Serial.println("Commands: 'index R G B', 'all R G B', 'clear'.");
  Serial.println("Example: '0 255 0 0' lights pixel 0 red; 'all 255 255 255' all white.");
  Serial.print("strip> ");
}

void loop() {
  static String line;
  static bool swallowLf = false;

  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());

    // Windows terminals send a bare CR on Enter, so accept CR, LF and CRLF.
    if (c == '\n' && swallowLf) {
      swallowLf = false;
      continue;
    }
    swallowLf = false;

    if (c == '\r' || c == '\n') {
      swallowLf = (c == '\r');
      handleLine(line);
      line = "";
      Serial.print("strip> ");
      continue;
    }

    if (c == '\t') {
      if (line.length() < 64) {
        line += ' ';
      }
      continue;
    }

    // Drop control bytes and line noise: a stray NUL arrives when the port opens.
    if (c < 0x20 || c > 0x7e) {
      continue;
    }
    if (line.length() < 64) {
      line += c;
    }
  }
}
