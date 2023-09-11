#include <math.h>
#include <Wire.h>

// Constants
#define MPU_ADDRESS 0x68

#define MPU_PWR_MGMT_1   0x6B
#define MPU_GYRO_CONFIG  0x1B
#define MPU_ACCEL_CONFIG 0x1C
#define MPU_ACCEL_XOUT_H 0x3B
#define MPU_GYRO_XOUT_H  0x43

#define GYRO_FS_250  0b00000000
#define GYRO_FS_500  0b00001000
#define GYRO_FS_1000 0b00010000
#define GYRO_FS_2000 0b00011000

#define GYRO_SEN_250  131.0
#define GYRO_SEN_500  65.5
#define GYRO_SEN_1000 32.8
#define GYRO_SEN_2000 16.4

// Config
#define PIN_BUZZ 0
#define GYRO_FS  GYRO_FS_2000
#define GYRO_SEN GYRO_SEN_2000

void writeMPU(uint8_t addr, uint8_t data) {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
}

void readMPU(uint8_t addr) {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(addr);
  Wire.endTransmission(false);
}

void setup() {
  Wire.begin();

  writeMPU(MPU_PWR_MGMT_1, 0b00001000);
  writeMPU(MPU_GYRO_CONFIG, GYRO_FS);
}

float sqr(float x) {
  return x * x;
}

void getGyros(int16_t* gx, int16_t* gy, int16_t* gz) {
  readMPU(MPU_GYRO_XOUT_H);
  Wire.requestFrom(MPU_ADDRESS, 6, true);
  while (Wire.available() < 6);

  *gx = Wire.read() << 8 | Wire.read();
  *gy = Wire.read() << 8 | Wire.read();
  *gz = Wire.read() << 8 | Wire.read();
}

float toNote(float value, float valueMax, float noteMin, float noteMax) {
  float scale = (value < valueMax) ? value / valueMax : 1.0;

  return scale * (noteMax - noteMin) + noteMin;
}

void loop() {
  int16_t gx, gy, gz;
  getGyros(&gx, &gy, &gz);

  float gSqr = sqr(gx / GYRO_SEN) + sqr(gy / GYRO_SEN) + sqr(gz / GYRO_SEN);
  float gSqrt = sqrt(gSqr);
  float note = toNote(gSqrt, 3464.1 , 130.81, 3951.07);

  tone(PIN_BUZZ, note);
  delay(10);
}
