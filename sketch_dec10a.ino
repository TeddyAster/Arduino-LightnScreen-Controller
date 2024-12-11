#include <FastLED.h>

#define LIGHT_DATA_PIN 3
#define EMOJI_DATA_PIN 4
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LIGHT_NUM_LEDS 60
#define EMOJI_NUM_LEDS 256
#define BRIGHTNESS 72
#define BRIGHTNESS2 72

CRGB lightLeds[LIGHT_NUM_LEDS];
CRGB emojiLeds[EMOJI_NUM_LEDS];

// 32x8 格式的颜文字模式（1 表示亮，0 表示灭）
const char emoji1[] PROGMEM = 
"00000000"
"00000000"
"00000000"
"00111000"
"01000100"
"01000100"
"01000100"
"00000000"
"00011000"
"00100100"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000"
"00000000";


const char emoji2[] PROGMEM = 
  "11111111"
  "11111111"
  "11111111"
  "11111111"
  "11111111"
  "11111111"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000";

const char emoji3[] PROGMEM = 
  "11111111"
  "11111111"
  "11111111"
  "11111111"
  "11111111"
  "11111111"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000"
  "00000000";

// 将 (x, y) 转换为蛇形排列的实际 LED 索引
int xyToIndex(int x, int y) {
  if (x % 2 == 0) {
    // 偶数列：从上到下
    return x * 8 + (7 - y);
  } else {
    // 奇数列：从下到上
    return x * 8 + y;
  }
}

// 初始化
void setup() {
  delay(3000); // 启动延时

  FastLED.addLeds<LED_TYPE, LIGHT_DATA_PIN, COLOR_ORDER>(lightLeds, LIGHT_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, EMOJI_DATA_PIN, COLOR_ORDER>(emojiLeds, EMOJI_NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
}

// 运行单个情景
void runSituation(CRGB color1, CRGB color2, const char* emoji, int steps, int delayTime, int duration) {
  displayEmoji(emoji); // 显示颜文字，保持常亮
  for (int i = 0; i < duration; i++) {
    breathingEffect(color1, color2, steps, delayTime);
  }
}

// LED灯带呼吸效果（仅影响呼吸灯）
void breathingEffect(CRGB color1, CRGB color2, int steps, int delayTime) {
  for (int i = 0; i <= steps; i++) {
    uint8_t brightness1 = map(i, 0, steps, 0, BRIGHTNESS);
    FastLED.setBrightness(brightness1);
    CRGB blendColor = blend(color1, color2, i * (255 / steps));
    fill_solid(lightLeds, LIGHT_NUM_LEDS, blendColor);
    FastLED.show();
    delay(delayTime);
  }
  for (int i = steps; i >= 0; i--) {
    uint8_t brightness1 = map(i, 0, steps, 0, BRIGHTNESS);
    FastLED.setBrightness(brightness1);
    CRGB blendColor = blend(color1, color2, i * (255 / steps));
    fill_solid(lightLeds, LIGHT_NUM_LEDS, blendColor);
    FastLED.show();
    delay(delayTime);
  }
}

// 点阵屏幕显示颜文字
void displayEmoji(const char* emojiPattern) {
  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 8; y++) {
      int i = xyToIndex(x, y);
      char pixel = pgm_read_byte_near(emojiPattern + x * 8 + y);
      emojiLeds[i] = (pixel == '1') ? CRGB::White : CRGB::Black;
    }
  }
  FastLED.setBrightness(BRIGHTNESS2); // 确保常亮的亮度
  FastLED.show();
}

void loop() {
  runSituation(CRGB::White, CRGB(180, 180, 180), emoji1, 50, 10, 10); // 情景1
  runSituation(CRGB::Red, CRGB(150, 0, 0), emoji2, 50, 10, 10);       // 情景2
  runSituation(CRGB::Blue, CRGB(0, 0, 150), emoji3, 50, 10, 10);     // 情景3
  runSituation(CRGB::Orange, CRGB(200, 100, 0), emoji1, 50, 10, 10); // 情景4
}
