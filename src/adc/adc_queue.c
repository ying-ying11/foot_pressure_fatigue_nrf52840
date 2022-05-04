#include <stdlib.h>
#include <stdint.h>
#include <kernel.h>

#include "adc_queue.h"

adc_queue_t* adc_queue_init() {
    adc_queue_t *queue = malloc(sizeof(adc_queue_t));
    k_sem_init(&queue->lock, 1, 1);
    return queue;
}

void adc_queue_push(adc_queue_t *queue, int16_t value) {
    k_sem_take(&queue->lock, K_FOREVER);

    adc_node_t *node = malloc(sizeof(adc_node_t));
    node->value = value;
    node->next = NULL;

    if (!queue->head) queue->head = node;
    else queue->tail->next = node;

    queue->tail = node;
    queue->size++;

    k_sem_give(&queue->lock);
}

void adc_queue_pop_amount(adc_queue_t *queue, int16_t *array, uint16_t amount) {
    if (amount > queue->size) return;

    k_sem_take(&queue->lock, K_FOREVER);
    queue->size -= amount;
    k_sem_give(&queue->lock);

    adc_node_t *node = queue->head, *temp;
    for (int i = 0; i < amount; i++) {
        temp = node;
        node = node->next;
        array[i] = temp->value;
        free(temp);
    }
    queue->head = node;
}

void adc_queue_clean(adc_queue_t *queue) {
    adc_node_t *node = queue->head, *temp;
    while (node) {
        temp = node;
        node = node->next;
        free(temp);
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}