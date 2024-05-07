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
#define PIN_BUZZ 3
#define FREQ_MAX 5000.0
#define GYRO_MAX 2000
#define GYRO_FS  GYRO_FS_2000
#define GYRO_SEN GYRO_SEN_2000
// #define SERIAL_ON


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
#ifdef SERIAL_ON
  Serial.begin(9600);
#endif

  Wire.begin();
  writeMPU(MPU_PWR_MGMT_1, 0b00001000);
  writeMPU(MPU_GYRO_CONFIG, GYRO_FS);
}

void getGyros(int16_t* gx, int16_t* gy, int16_t* gz) {
  readMPU(MPU_GYRO_XOUT_H);
  Wire.requestFrom(MPU_ADDRESS, 6, true);
  while (Wire.available() < 6);

  *gx = Wire.read() << 8 | Wire.read();
  *gy = Wire.read() << 8 | Wire.read();
  *gz = Wire.read() << 8 | Wire.read();
}

void loop() {
  int16_t gx, gy, gz;
  getGyros(&gx, &gy, &gz);

  float gyro = abs(gx / GYRO_SEN) + abs(gy / GYRO_SEN) + abs(gz / GYRO_SEN);

  float note = FREQ_MAX * gyro / (GYRO_MAX * 3);

#ifdef SERIAL_ON
  Serial.print(gyro); Serial.print(",\t");
  Serial.print(note); Serial.print(",\t");
  Serial.println("");
#endif

  const float threshold = 90;
  if (gyro > threshold) {
    tone(PIN_BUZZ, note);
  } else {
    noTone(PIN_BUZZ);
  }
  delay(10);

}
