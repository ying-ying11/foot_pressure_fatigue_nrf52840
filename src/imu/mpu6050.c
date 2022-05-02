#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/i2c.h>

#include "mpu6050.h"

#define I2C_SAMPLE_RATE 1000

int read_data(const struct device *dev, uint8_t addr, uint8_t *data, uint8_t num_bytes) {
    int ret;
    for (int8_t i = 0; i < num_bytes; i++) {
        ret = i2c_reg_read_byte(dev, MPU6050_ADDRESS, addr + i, &data[i]);
        if (ret) {
            printk("Error read from MPU6050! error code (%d)\n", ret);
		    return ret;
        }
    }
    return 0;
}

int mpu6050_init(const struct device *dev) {
    int ret;
    if (!device_is_ready(dev)) {
		printk("I2C: Device is not ready.\n");
		return -1;
	}

	ret = i2c_reg_write_byte(dev, MPU6050_ADDRESS, MPU6050_WAKE_UP, MPU6050_CLOCK_INTERNAL);
	if (ret) {
		printk("Error writing to MPU6050! error code (%d)\n", ret);
		return ret;
	}
	printk("mpu6050 wake up.\n");

	ret = i2c_reg_write_byte(dev, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_FS_2);
	if (ret) {
		printk("Error writing to MPU6050! error code (%d)\n", ret);
		return ret;
	}
	printk("mpu6050 set accel.\n");

	ret = i2c_reg_write_byte(dev, MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, MPU6050_GYRO_FS_250);
	if (ret) {
		printk("Error writing to MPU6050! error code (%d)\n", ret);
		return ret;
	}
	printk("mpu6050 set gyro.\n");
    return 0;
}

int mpu6050_sample(const struct device *dev, struct mpu6050_data *data) {

    int ret;
    uint8_t raw[6];

    ret = read_data(dev, MPU6050_ACCEL_XOUT_H, raw, 6);
    if (ret) {
        printk("Error reading accelerometer data! error code (%d)\n", ret);
        return ret;
    }
    data->ax = (raw[0] << 8) | raw[1];
    data->ay = (raw[2] << 8) | raw[3];
    data->az = (raw[4] << 8) | raw[5];

    ret = read_data(dev, MPU6050_GYRO_XOUT_H, raw, 6);
    if (ret) {
        printk("Error reading gyroscope data! error code (%d)\n", ret);
        return ret;
    }
    data->gx = (raw[0] << 8) | raw[1];
    data->gy = (raw[2] << 8) | raw[3];
    data->gz = (raw[4] << 8) | raw[5];

    return ret;
}

// void mpu6050_thread() {
//     const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));
    
//     if (mpu6050_init(i2c_dev)) return;

//     while (1) {
//         struct mpu6050_data data;
//         mpu6050_sample(i2c_dev, &data);

// 		printk("ax: %d, ay: %d, az: %d\n", data.ax, data.ay, data.az);
// 		printk("gx: %d, gy: %d, gz: %d\n", data.gx, data.gy, data.gz);

//         k_msleep(I2C_SAMPLE_RATE);
//     }
// }

// K_THREAD_DEFINE(mpu6050_thread_id, 1024, mpu6050_thread, NULL, NULL, NULL, 7, 0, 0);