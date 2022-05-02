#ifndef FLYN_ADC_LIST
#define FLYN_ADC_LIST

#include <stdio.h>
#include <stdint.h>

typedef struct adc_list_node {

    int16_t value;
    struct adc_list_node *next;

} adc_list_node_t;

typedef struct adc_list {

    uint16_t size;
    struct adc_list_node *head, *last;

} adc_list_t ;


void adc_list_append(adc_list_t *, int16_t);
void adc_list_clean(adc_list_t *);
uint16_t adc_list_encode(adc_list_t *, uint8_t *);

#endif