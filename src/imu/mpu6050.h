#ifndef FLYN_MPU6050_H
#define FLYN_MPU6050_H

#define MPU6050_ADDRESS       0x68

#define MPU6050_GYRO_CONFIG   0x1B
#define MPU6050_ACCEL_CONFIG  0x1C

#define MPU6050_WAKE_UP       0x6B
#define MPU6050_CLOCK_INTERNAL      0x00

#define MPU6050_ACCEL_XOUT_H  0x3B
#define MPU6050_ACCEL_XOUT_L  0x3C
#define MPU6050_ACCEL_YOUT_H  0x3D
#define MPU6050_ACCEL_YOUT_L  0x3E
#define MPU6050_ACCEL_ZOUT_H  0x3F
#define MPU6050_ACCEL_ZOUT_L  0x40

#define MPU6050_GYRO_XOUT_H   0x43
#define MPU6050_GYRO_XOUT_L   0x44
#define MPU6050_GYRO_YOUT_H   0x45
#define MPU6050_GYRO_YOUT_L   0x46
#define MPU6050_GYRO_ZOUT_H   0x47
#define MPU6050_GYRO_ZOUT_L   0x48

// GYRO_CONFIG - scale gyroscope range
#define MPU6050_GYRO_FS_250   0x00
#define MPU6050_GYRO_FS_500   0x01
#define MPU6050_GYRO_FS_1000  0x02
#define MPU6050_GYRO_FS_2000  0x03

// ACCEL_CONFIG - scale accelerometer range
#define MPU6050_ACCEL_FS_2    0x00
#define MPU6050_ACCEL_FS_4    0x01
#define MPU6050_ACCEL_FS_8    0x02
#define MPU6050_ACCEL_FS_16   0x03

struct mpu6050_data {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
};

#include <device.h>

int read_data(const struct device *, uint8_t, uint8_t *, uint8_t);
int mpu6050_init(const struct device *);
int mpu6050_sample(const struct device *, struct mpu6050_data *);

#endif
