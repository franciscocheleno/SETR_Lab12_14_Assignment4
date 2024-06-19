/**
 * \file adc.h
 * \author Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt>
 * \date 1, June, 2024
 * \brief Implementation of ADC module that reads an ADC input and prints the corresponding raw and voltage value in the console.
 */

#ifndef ADC_H
#define ADC_H

#include <zephyr/kernel.h>          /* for k_msleep() */
#include <zephyr/device.h>          /* for device_is_ready() and device structure */
#include <zephyr/devicetree.h>	    /* for DT_NODELABEL() */
#include <zephyr/drivers/gpio.h>    /* for GPIO API*/
#include <zephyr/drivers/adc.h>     /* for ADC API*/
#include <zephyr/sys/printk.h>      /* for printk()*/
#include <string.h>

#include <hal/nrf_saadc.h>              /* ADC definitions and includes */

#define ADC_RESOLUTION 10               /* ADC resolution in bits */
#define ADC_GAIN ADC_GAIN_1_4           /* ADC gain setting */
#define ADC_REFERENCE ADC_REF_VDD_1_4   /* ADC reference voltage */
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40) /**< ADC acquisition time */
#define ADC_CHANNEL_ID 1                /* ADC channel ID */

#define ADC_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1 /*nRF ANx input to use*/

#define BUFFER_SIZE 1                   /* Buffer size for ADC samples */

#define ADC_NODE DT_NODELABEL(adc)      /* Device tree node label for ADC */

/* Error codes */
#define ERR_OK 0        /* All fine */
#define ERR_CONFIG -1   /* Configuration failure */

/* Global vars */
static uint16_t adc_sample_buffer[BUFFER_SIZE];

/**
 * \brief Initializes ADC configuration and starts sampling.
 * \return ERR_OK if successful, ERR_CONFIG if configuration failed.
 */
int adc_sample(void);

#endif /* ADC_H */