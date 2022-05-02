
#include <net/socket.h>
#include <stdio.h>
#include <string.h>
#include <drivers/adc.h>
#include <zephyr.h>
#include <hal/nrf_saadc.h>

#include "adc.h"

#define ADC_GAIN ADC_GAIN_1_4
#define ADC_REFERENCE ADC_REF_VDD_1_4
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)

#define ADC_1ST_CHANNEL_ID 0
#define ADC_1ST_CHANNEL_INPUT NRF_SAADC_INPUT_AIN0

static const struct adc_channel_cfg m_1st_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_1ST_CHANNEL_ID,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive = ADC_1ST_CHANNEL_INPUT,
#endif
};

const struct adc_sequence_options sequence_opts = {
	.interval_us = 0,
	.callback = NULL,
	.user_data = NULL,
	.extra_samplings = 7,
};

int adc_init(const struct device *dev) {
    int ret;

    if (!dev) {
        printk("ADC: Device is not ready.\n");
        return -1;
    }
	ret = adc_channel_setup(dev, &m_1st_channel_cfg);
	if (ret) {
		printk("Error in adc setup: %d\n", ret);
        return ret;
	}

	/* Trigger offset calibration
	 * As this generates a _DONE and _RESULT event
	 * the first result will be incorrect.
	 */
	NRF_SAADC_S->TASKS_CALIBRATEOFFSET = 1;
    printk("adc setup.\n");
    return ret;
}

int adc_sample(const struct device *dev, int16_t *buffer, uint16_t buffer_size) {
	int ret;

	const struct adc_sequence sequence = {
		.options = &sequence_opts,
		.channels = BIT(ADC_1ST_CHANNEL_ID),
		.buffer = buffer,
		.buffer_size = buffer_size * 2,
		.resolution = ADC_RESOLUTION,
	};

	if (!dev) {
		printk("ADC: Device is not ready.\n");
		return -1;
	}

	ret = adc_read(dev, &sequence);

    if (ret) {
        printk("ADC read err: %d\n", ret);
        return ret;
    }

	return ret;
}