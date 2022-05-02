#include "adc_list.h"

void adc_list_append(adc_list_t *list, int16_t value) {
    adc_list_node_t *node = malloc(1 * sizeof(adc_list_node_t));
    node->value = value;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
        list->last = node;
    }
    else {
        list->last->next = node;
        list->last = node;
    }
    list->size++;
}

void adc_list_clean(adc_list_t *list) {
    adc_list_node_t *node = list->head;
    while (node) {
        adc_list_node_t *temp = node;
        node = node->next;
        free(temp);
    }
    list->head = NULL;
    list->last = NULL;
    list->size = 0;
}

uint16_t adc_list_encode(adc_list_t *list, uint8_t *buffer) {
    // calculate buffer size 
    // list_size(2 bytes) | data...(x) | check_sum(1 byte)
    uint16_t buffer_len = 3;
    buffer_len += list->size * 12 / 8;
    if (list->size % 2) buffer_len++;
    // create buffer & add data size to head
    buffer = malloc(buffer_len * sizeof(uint8_t));
    buffer[0] = (list->size >> 8) & 0xFF;
    buffer[1] = list->size & 0xFF;
    // put data into buffer
    uint16_t loc = 2;
    adc_list_node_t *node = list->head;
    for (int i = 0; i < list->size; i++) {
        uint8_t hi_bit = (node->value >> 8) & 0x0F;
        uint8_t lo_bit = node->value & 0xFF;
        if (i % 2) {
            buffer[loc] = hi_bit;
            buffer[loc + 1] = lo_bit;
        }
        else {
            buffer[loc] = buffer[loc] << 4 | hi_bit;
            buffer[loc + 2] = lo_bit;
            loc += 3;
        }
        node = node->next;
    }
    // put check sum at last
    uint8_t sum = 0;
    for (int i = 0; i < buffer_len - 1; i++) {
        sum += buffer[i];
    }
    buffer[buffer_len - 1] = sum;
    return buffer_len;
}