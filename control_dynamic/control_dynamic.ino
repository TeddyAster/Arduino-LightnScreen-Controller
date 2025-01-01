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

// 全局变量
unsigned long lastUpdateTime = 0; // 上一次更新的时间
const int updateInterval = 500;  // 更新间隔（500毫秒）
CRGB targetColor = CRGB(0, 150, 255);  // 默认目标颜色（高纯度蓝青色）
CRGB currentColor = CRGB::Black; // 当前颜色
float lastAx = 0, lastAy = 0, lastAz = 0; // 上次的加速度值
float lastRoll = 0, lastPitch = 0; // 上次的角度值

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

// 呼吸效果
void breathingEffect(CRGB color1, int steps) {
  for (int i = 0; i <= steps; i++) {
    uint8_t brightness1 = map(i, 0, steps, 0, BRIGHTNESS);
    FastLED.setBrightness(brightness1);
    fill_solid(lightLeds, LIGHT_NUM_LEDS, color1);
    FastLED.show();
    delay(10);
  }
  for (int i = steps; i >= 0; i--) {
    uint8_t brightness1 = map(i, 0, steps, 0, BRIGHTNESS);
    FastLED.setBrightness(brightness1);
    fill_solid(lightLeds, LIGHT_NUM_LEDS, color1);
    FastLED.show();
    delay(10);
  }
}

// 平滑过渡到目标颜色
void smoothTransition(CRGB newColor) {
  currentColor = blend(currentColor, newColor, 32); // 每次接近目标颜色的1/8
  fill_solid(lightLeds, LIGHT_NUM_LEDS, currentColor);
  FastLED.show();
}

// 获取倾斜角度（Roll 和 Pitch）
void calculateAngles(float ax, float ay, float az, float &roll, float &pitch) {
  roll = atan2(ay, az) * 180 / PI; // 绕X轴的倾斜角度
  pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI; // 绕Y轴的倾斜角度
}

// 获取MPU6050的加速度数据并判断运动情况
void detectMovement() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az); // 获取加速度数据

  // 将加速度值转换为g单位
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // 计算变化幅度
  float diffAx = abs(ax_g - lastAx);
  float diffAy = abs(ay_g - lastAy);
  float diffAz = abs(az_g - lastAz);
  float changeMagnitude = diffAx + diffAy + diffAz;

  // 计算倾斜角度
  float roll, pitch;
  calculateAngles(ax_g, ay_g, az_g, roll, pitch);

  // 输出调试信息
  Serial.print("Accel X: "); Serial.print(ax_g);
  Serial.print(" Y: "); Serial.print(ay_g);
  Serial.print(" Z: "); Serial.println(az_g);
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(" Pitch: "); Serial.println(pitch);
  Serial.print("Change Magnitude: "); Serial.println(changeMagnitude);

  // 更新上次的加速度值和角度值
  lastAx = ax_g;
  lastAy = ay_g;
  lastAz = az_g;
  lastRoll = roll;
  lastPitch = pitch;

  // 判断运动情况并更新目标颜色
  if (abs(roll) > 10 || abs(pitch) > 10) { // 倾斜超过10度
    targetColor = CRGB(200, 0, 0); // 深红
  } else if (changeMagnitude > 1.0) { // 突然被晃动
    targetColor = CRGB(255, 0, 150); // 洋红
  } else if (abs(az_g - 1.0) > 0.5) { // 上下运动
    targetColor = CRGB(255, 200, 0); // 高nn                        亮黄色
  } else { // 静止状态
    targetColor = CRGB(200, 0, 200); // 蓝青色
  }
}

void loop() {
  unsigned long currentTime = millis();

  // 每隔500毫秒检测一次方向并更新目标颜色
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    detectMovement();
  }

  // 呼吸效果（每次调用时逐步过渡到目标颜色）
  breathingEffect(targetColor, 50);
}
