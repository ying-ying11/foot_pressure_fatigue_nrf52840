#ifndef FLYN_IMU_SERVICE_H
#define FLYN_IMU_SERVICE_H

#define IMU_SERVICE_UUID_VAL BT_UUID_128_ENCODE(0x58C4FFA1, 0x1548, 0x44D5, 0x9972, 0xF7C25BECB620)

void imu_data_update(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
void accelerometer_notify();
void gyroscope_notify();

#endif