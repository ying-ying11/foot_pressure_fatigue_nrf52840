#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "adc_service.h"

static struct bt_uuid_128 adc_uuid = BT_UUID_INIT_128(ADC_SERVICE_UUID_VAL);
static struct bt_uuid_128 adc_raw_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xD16F7A3D, 0x1897, 0x40EA, 0x9629, 0xBDF749AC5991));

static int16_t adc_raw;
static uint8_t adc_raw_notify_flag;

static void adc_raw_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	adc_raw_notify_flag = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

BT_GATT_SERVICE_DEFINE(adc_service,
    BT_GATT_PRIMARY_SERVICE(&adc_uuid),
    BT_GATT_CHARACTERISTIC(&adc_raw_uuid.uuid, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, &adc_raw),
    BT_GATT_CCC(adc_raw_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

void adc_data_update(int16_t data) {
    adc_raw = data;
}

void adc_raw_notify() {
    if (!adc_raw_notify_flag) return;
    printk("notify at: %d\n", k_cyc_to_us_near32(k_cycle_get_32()));
    bt_gatt_notify(NULL, &adc_service.attrs[1], &adc_raw, sizeof(adc_raw));
}