#ifndef FLYN_ADC_QUEUE
#define FLYN_ADC_QUEUE

typedef struct adc_node {

    int16_t value;

} adc_node_t;

void adc_queue_push(struct k_queue*, int16_t);
uint16_t adc_queue_pop_amount(struct k_queue*, int16_t*, uint16_t);
void adc_queue_clean(struct k_queue*);

#endif