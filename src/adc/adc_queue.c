#include <stdlib.h>
#include <stdint.h>
#include <kernel.h>

#include "adc_queue.h"

#define QUEUE_BLOCK_SIZE 8
#define QUEUE_BLOCK_COUNT 1024

K_MEM_SLAB_DEFINE(queue_slab, QUEUE_BLOCK_SIZE, QUEUE_BLOCK_COUNT, sizeof(void *));

void adc_queue_push(struct k_queue *queue, int16_t value) {
    adc_node_t *node;
    if (!k_mem_slab_alloc(&queue_slab, &node, K_NO_WAIT)) {
        node->value = value;
    }
    else {
        printk("Memory allocation fail");
        return;
    }

    k_queue_append(queue, (void *) node);
}

uint16_t adc_queue_pop_amount(struct k_queue *queue, int16_t *array, uint16_t amount) {
    uint16_t pop_amount = 0;
    adc_node_t *node;
    for (int i = 0; i < amount; i++) {
        if (k_queue_is_empty(queue)) break;
        node = k_queue_get(queue, K_NO_WAIT);
        array[i] = node->value;
        k_mem_slab_free(&queue_slab, &node);
        pop_amount++;
    }
    return pop_amount;
}

void adc_queue_clean(struct k_queue *queue) {
    adc_node_t *node;
    while (!k_queue_is_empty(queue)) {
        node = k_queue_get(queue, K_NO_WAIT);
        k_mem_slab_free(&queue_slab, &node);
    }
}