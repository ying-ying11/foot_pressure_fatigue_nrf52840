#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <stdlib.h>

#include "adc_queue.h"
#include "adc_service.h"

#define MAX_VALUE_AMOUNT 160

static struct bt_uuid_128 adc_uuid = BT_UUID_INIT_128(ADC_SERVICE_UUID_VAL);
static struct bt_uuid_128 adc_raw_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xD16F7A3D, 0x1897, 0x40EA, 0x9629, 0xBDF749AC5991));

static adc_queue_t *adc_values;
static int16_t raw_array[MAX_VALUE_AMOUNT];
static uint8_t buffer[247];

static uint8_t adc_raw_notify_flag;

static void adc_raw_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	adc_raw_notify_flag = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
    if (!adc_raw_notify_flag && adc_values) {
        adc_queue_clean(adc_values);
    }
}

BT_GATT_SERVICE_DEFINE(adc_service,
    BT_GATT_PRIMARY_SERVICE(&adc_uuid),
    BT_GATT_CHARACTERISTIC(&adc_raw_uuid.uuid, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(adc_raw_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

void adc_data_update(int16_t data) {
    if (!adc_raw_notify_flag) return;

    if (!adc_values) adc_values = adc_queue_init();
    if (adc_values->size > 1000) {
        adc_queue_pop_amount(adc_values, raw_array, 100);
        printk("blocked data over 1000, remove 100 data!!");
    }
    adc_queue_push(adc_values, data);

}

uint16_t adc_encode() {
    uint8_t amount = adc_values->size > MAX_VALUE_AMOUNT ? MAX_VALUE_AMOUNT : adc_values->size;
    adc_queue_pop_amount(adc_values, raw_array, amount);

    // calculate buffer size 
    // list_size(2 bytes) | data...(x) | check_sum(1 byte)
    uint16_t buffer_len = 2;
    buffer_len += amount * 12 / 8;
    if (amount % 2) buffer_len++;

    // malloc buffer & add data size to head
    buffer[0] = amount;

    // put data into buffer
    uint16_t loc = 1;
    for (int i = 0; i < amount; i++) {
        uint8_t hi_bit = (raw_array[i] >> 8) & 0x0F;
        uint8_t lo_bit = raw_array[i] & 0xFF;
        if (i % 2) {
            buffer[loc] = buffer[loc] << 4 | hi_bit;
            buffer[loc + 2] = lo_bit;
            loc += 3;
        }
        else {
            buffer[loc] = hi_bit;
            buffer[loc + 1] = lo_bit;
        }
    }

    // put check sum at last
    uint8_t checksum_pos = buffer_len - 1;
    
    for (int i = 0; i < checksum_pos; i++) {
        buffer[checksum_pos] += buffer[i];
    }
    return buffer_len;
}

uint32_t prev_us_time = 0;

void adc_raw_notify() {
    if (!adc_raw_notify_flag || !adc_values) return;
    
    uint16_t len = adc_encode();
    bt_gatt_notify(NULL, &adc_service.attrs[1], buffer, len);
    printk("buffer length: %d\n", len);

    // measure notification time interval
    uint32_t time = k_cyc_to_us_near32(k_cycle_get_32());
    printk("notify interval (us): %d\n", time - prev_us_time);
    prev_us_time = time;
}