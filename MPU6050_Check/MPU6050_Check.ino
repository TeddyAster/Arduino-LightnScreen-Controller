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

// 打印MPU6050传感器数据
void printMPU6050Data() {
  // 读取加速度数据
  float ax = mpu.getAccelerationX();
  float ay = mpu.getAccelerationY();
  float az = mpu.getAccelerationZ();
  // 读取陀螺仪数据
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  float speedSquared = ax * ax + ay * ay + az * az;
  // 打印数据到串行监视器
  //Serial.print("Accel X: "); Serial.print(ax);
  //Serial.print(", Accel Y: "); Serial.print(ay);
  //Serial.print(", Accel Z: "); Serial.println(az);
  Serial.print("SpeedSquared:");Serial.println(speedSquared);
  //Serial.print("Gyro X: "); Serial.print(gx);
  //Serial.print(", Gyro Y: "); Serial.print(gy);
  //Serial.print(", Gyro Z: "); Serial.println(gz);
}

void loop() {
  printMPU6050Data(); // 打印MPU6050传感器数据
  delay(100); // 延迟，避免过多读取
}
