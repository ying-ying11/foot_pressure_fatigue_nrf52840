#ifndef FLYN_ADC_QUEUE
#define FLYN_ADC_QUEUE

typedef struct adc_node {

    int16_t value;
    struct adc_node *next;

} adc_node_t;

typedef struct adc_queue {

    uint16_t size;
    adc_node_t *head, *tail;
    struct k_sem lock;

} adc_queue_t;

adc_queue_t* adc_queue_init();
void adc_queue_push(adc_queue_t*, int16_t);
void adc_queue_pop_amount(adc_queue_t*, int16_t*, uint16_t);
void adc_queue_clean(adc_queue_t*);

#endif