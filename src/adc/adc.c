/**
 * \file adc.c
 * \author Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt>
 * \date 1, June, 2024
 * \brief Implementation of ADC module that reads an ADC input and prints the corresponding raw and voltage value in the console.
 */

#include "adc.h"

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

static const struct device *adc_dev = NULL;

/**
 * Takes one sample from the ADC.
 * Returns 0 if successful, negative value if there's an error.
 */
int adc_sample(void)
{
	int ret;
	const struct adc_sequence sequence = {
		.channels = BIT(ADC_CHANNEL_ID),
		.buffer = adc_sample_buffer,
		.buffer_size = sizeof(adc_sample_buffer),
		.resolution = ADC_RESOLUTION
	};

	if (adc_dev == NULL) {
		printk("\n\n\nadc_sample(): error, must bind to adc first \n\r");
		return -1;
	}

	ret = adc_read(adc_dev, &sequence);
	if (ret) {
		printk("\n\n\nadc_read() failed with code %d\n", ret);
	}

	return ret;
}
