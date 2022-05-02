#include <string.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "mpu6050_service.h"

static struct bt_uuid_128 imu_uuid = BT_UUID_INIT_128(IMU_SERVICE_UUID_VAL);
static struct bt_uuid_128 imu_acc_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x58C4FFA1, 0x1548, 0x44D5, 0x9972, 0xF7C25BECB621));
static struct bt_uuid_128 imu_gyr_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x58C4FFA1, 0x1548, 0x44D5, 0x9972, 0xF7C25BECB622));
static struct bt_uuid_128 imu_acc_text_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x58C4FFA1, 0x1548, 0x44D5, 0x9972, 0xF7C25BECB623));
static struct bt_uuid_128 imu_gyr_text_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x58C4FFA1, 0x1548, 0x44D5, 0x9972, 0xF7C25BECB624));

static int16_t imu_acc[3];
static uint8_t imu_acc_notify_flag;
static int16_t imu_gyr[3];
static uint8_t imu_gyr_notify_flag;

#define TEXT_LEN 30
static char imu_acc_text[TEXT_LEN] = "ax:      \nay:      \naz:      ";
static char imu_gyr_text[TEXT_LEN] = "gx:      \ngy:      \ngz:      ";

static void imu_acc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	imu_acc_notify_flag = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

static void imu_gyr_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	imu_gyr_notify_flag = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

void imu_val_to_str(int16_t value, char *buffer, uint8_t offset) {
    int8_t ptr = 5;

    int16_t temp = value > 0 ? value : -value;

    while (temp > 0) {
        buffer[offset + ptr--] = (temp % 10) + 48;
        temp /= 10;
    }

    if (value < 0) buffer[offset + ptr--] = '-';
    while (ptr >= 0) buffer[offset + ptr--] = ' ';
}

static ssize_t read_acc_text(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset) {
    imu_val_to_str(imu_acc[0], imu_acc_text, 3);
    imu_val_to_str(imu_acc[1], imu_acc_text, 13);
    imu_val_to_str(imu_acc[2], imu_acc_text, 23);

    return bt_gatt_attr_read(conn, attr, buf, len, offset, imu_acc_text, sizeof(imu_acc_text));
}

static ssize_t read_gry_text(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset) {
    imu_val_to_str(imu_gyr[0], imu_gyr_text, 3);
    imu_val_to_str(imu_gyr[1], imu_gyr_text, 13);
    imu_val_to_str(imu_gyr[2], imu_gyr_text, 23);

    return bt_gatt_attr_read(conn, attr, buf, len, offset, imu_gyr_text, sizeof(imu_gyr_text));
}

BT_GATT_SERVICE_DEFINE(imu_service,
    BT_GATT_PRIMARY_SERVICE(&imu_uuid),
    BT_GATT_CHARACTERISTIC(&imu_acc_uuid.uuid, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, imu_acc),
    BT_GATT_CCC(imu_acc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&imu_gyr_uuid.uuid, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, imu_gyr),
    BT_GATT_CCC(imu_gyr_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&imu_acc_text_uuid.uuid, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_acc_text, NULL, imu_acc_text),
    BT_GATT_CHARACTERISTIC(&imu_gyr_text_uuid.uuid, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_gry_text, NULL, imu_gyr_text)
);

void imu_data_update(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz) {
    imu_acc[0] = ax;
    imu_acc[1] = ay;
    imu_acc[2] = az;

    imu_gyr[0] = gx;
    imu_gyr[1] = gy;
    imu_gyr[2] = gz;
}

void accelerometer_notify() {
    if (!imu_acc_notify_flag) return;

    bt_gatt_notify(NULL, &imu_service.attrs[1], imu_acc, sizeof(imu_acc));
}

void gyroscope_notify() {
    if (!imu_gyr_notify_flag) return;

    bt_gatt_notify(NULL, &imu_service.attrs[4], imu_gyr, sizeof(imu_gyr));
}
