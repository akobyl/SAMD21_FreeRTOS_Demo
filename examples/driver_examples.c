/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static uint16_t example_DAC_0[10] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900};

/**
 * Example task of using DAC_0 to generate waveform.
 */
void DAC_0_example_task(void *p)
{
	(void)p;

	dac_os_enable_channel(&DAC_0, 0);

	while (1) {
		if (dac_os_write(&DAC_0, 0, example_DAC_0, 10) == 10) {
			/* convert OK */;
		} else {
			/* error. */;
		}
	}
}
