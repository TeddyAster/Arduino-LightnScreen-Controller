#include <Wire.h>
#include <FastLED.h>
#include "MPU6050.h" // 包含MPU6050.h头文件

#define LIGHT_DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LIGHT_NUM_LEDS 60
#define BRIGHTNESS 90

CRGB lightLeds[LIGHT_NUM_LEDS];

// 实例化MPU6050对象
MPU6050 mpu;

// 初始化
void setup() {
  Serial.begin(9600);
  delay(3000); // 启动延时
  FastLED.addLeds<LED_TYPE, LIGHT_DATA_PIN, COLOR_ORDER>(lightLeds, LIGHT_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // 设置MPU6050
  Wire.begin();
  mpu.initialize(); // 使用MPU6050的initialize函数
}

// LED灯带呼吸效果
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

// 获取MPU6050的加速度数据并判断运动情况
void detectMovement() {
  float ax = mpu.getAccelerationX(); // 获取加速度数据
  float ay = mpu.getAccelerationY();
  float az = mpu.getAccelerationZ();
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  float speedSquared = ax * ax + ay * ay + az * az;

  // 判断不同的运动情况
  if (450000000 < speedSquared < 510000000) { // 不移动，情景1
    breathingEffect(CRGB::White, CRGB(180, 180, 180), 50, 10);
  } else if (abs(ax) > 1.0 && abs(ay) < 0.5) { // 平行快速移动，情景2
    if (speedSquared > 700000000 or speedSquared < 350000000) {
      breathingEffect(CRGB::Red, CRGB(150, 0, 0), 50, 10);
    } else {
      breathingEffect(CRGB::Blue, CRGB(0, 0, 150), 50, 10); // 平行慢速移动，情景3
    }
  } else if (abs(az) > 1.0) { // 上下移动，情景4
    breathingEffect(CRGB::Orange, CRGB(200, 100, 0), 50, 10);
  }
}

void loop() {
  detectMovement(); // 检测运动并控制LED情景
  delay(100); // 延迟，避免过多读取
}
