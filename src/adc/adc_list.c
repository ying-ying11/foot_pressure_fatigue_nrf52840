#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "adc_list.h"

void adc_list_append(adc_list_t *list, int16_t value) {
    adc_list_node_t *node = malloc(1 * sizeof(adc_list_node_t));
    node->value = value;
    node->next = NULL;

    if (!list->head) list->head = node;
    else list->last->next = node;

    list->last = node;
    list->size++;
    // printk("%d data append at %d\n", node->value, list->size);
}

void adc_list_clean(adc_list_t *list) {
    adc_list_node_t *node = list->head, *temp;
    while (node) {
        temp = node;
        node = node->next;
        free(temp);
    }
    list->head = NULL;
    list->last = NULL;
    list->size = 0;
}

uint16_t adc_list_encode_len(adc_list_t *list) {
    // calculate buffer size 
    // list_size(2 bytes) | data...(x) | check_sum(1 byte)
    uint16_t buffer_len = 3;
    buffer_len += list->size * 12 / 8;
    if (list->size % 2) buffer_len++;
    return buffer_len;
}

void adc_list_encode(adc_list_t *list, uint8_t *buffer) {
    // add data size to head
    buffer[0] = (list->size >> 8) & 0xFF;
    buffer[1] = list->size & 0xFF;
    // put data into buffer
    uint16_t loc = 2;
    adc_list_node_t *node = list->head;
    for (int i = 0; i < list->size; i++) {
        uint8_t hi_bit = (node->value >> 8) & 0x0F;
        uint8_t lo_bit = node->value & 0xFF;
        if (i % 2) {
            buffer[loc] = buffer[loc] << 4 | hi_bit;
            buffer[loc + 2] = lo_bit;
            loc += 3;
        }
        else {
            buffer[loc] = hi_bit;
            buffer[loc + 1] = lo_bit;
        }
        node = node->next;
    }
    // put check sum at last
    uint8_t checksum_pos = adc_list_encode_len(list) - 1;
    
    for (int i = 0; i < checksum_pos; i++) {
        buffer[checksum_pos] += buffer[i];
    }
}