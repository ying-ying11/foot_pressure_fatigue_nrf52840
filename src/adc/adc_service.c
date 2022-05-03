#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <stdlib.h>

#include "adc_list.h"
#include "adc_service.h"

static struct bt_uuid_128 adc_uuid = BT_UUID_INIT_128(ADC_SERVICE_UUID_VAL);
static struct bt_uuid_128 adc_raw_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xD16F7A3D, 0x1897, 0x40EA, 0x9629, 0xBDF749AC5991));

K_SEM_DEFINE(sem, 1, 1);

static uint8_t *buffer;

static bool list_switch = false, is_init = false;
static adc_list_t *list0, *list1;

static uint8_t adc_raw_notify_flag;

static void adc_raw_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	adc_raw_notify_flag = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

BT_GATT_SERVICE_DEFINE(adc_service,
    BT_GATT_PRIMARY_SERVICE(&adc_uuid),
    BT_GATT_CHARACTERISTIC(&adc_raw_uuid.uuid, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(adc_raw_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

void adc_data_update(int16_t data) {
    if (!adc_raw_notify_flag) return;

    k_sem_take(&sem, K_FOREVER);

    if (!is_init) {
        is_init = true;
        list0 = malloc(sizeof(adc_list_t));
        list1 = malloc(sizeof(adc_list_t));
    }
    if (list_switch) adc_list_append(list1, data);
    else adc_list_append(list0, data);

    k_sem_give(&sem);
}

uint32_t prev_us_time = 0;

void adc_raw_notify() {
    if (!adc_raw_notify_flag || !is_init) return;

    k_sem_take(&sem, K_FOREVER);
        list_switch = !list_switch;
    k_sem_give(&sem);

    adc_list_t *target_list = list_switch ? list0 : list1;
    
    int16_t len = adc_list_encode_len(target_list);
    buffer = malloc(len * sizeof(uint8_t));
    adc_list_encode(target_list, buffer);
    bt_gatt_notify(NULL, &adc_service.attrs[1], buffer, len);

    printk("data size: %d\n", target_list->size);
    printk("buffer length: %d\n", len);

    free(buffer);
    adc_list_clean(target_list);

    // measure notification time interval
    uint32_t time = k_cyc_to_us_near32(k_cycle_get_32());
    printk("notify interval (us): %d\n", time - prev_us_time);
    prev_us_time = time;
}