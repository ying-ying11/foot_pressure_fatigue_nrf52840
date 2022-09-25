/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <sys/printk.h>

#include "flyn_bluetooth.h"
#include "adc.h"
#include "adc_service.h"


#define IMU_SAMPLE_TIME 100
#define DT_N_S_soc_S_adc_40007000_P_label "ADC_0"
#define ADC_DEVICE_NAME DT_LABEL(DT_INST(0, DT_N_S_soc_S_adc_40007000_P_compatible_IDX_0_TOKEN))

#define THREAD_STACK_SIZE 500
#define THREAD_PRIORITY 5

const struct device *adc_dev;




void main(void) {
	printk("Hello World! %s\n", CONFIG_BOARD);

	// ADC setup
    adc_dev = device_get_binding(ADC_DEVICE_NAME);
	if (adc_init(&adc_dev)) return;


	if (bt_init()) return;

	adc_sample(adc_dev);
}

//K_THREAD_DEFINE(imu_sample_thread, THREAD_STACK_SIZE, imu_sample_event, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);