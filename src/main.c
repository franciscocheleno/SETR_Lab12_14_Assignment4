/**
 * @file main.c
 * \author: Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * @brief Main application file
 *
 * This file contains the main application code. It initializes the
 * database, configures the output pins for LEDs, sets up UART, configures
 * buttons, and initializes threads.
 *
 * @details
 * The main function resets the database values, configures the GPIO pins
 * for the LEDs, initializes UART communication, configures button inputs,
 * and sets up the necessary threads for the application. The outputs_config
 * function ensures the GPIO pins for the LEDs are ready and configures them
 * to be in an output state.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "threads.h"
#include "uart.h"
#include "IO.h"
#include "ADC.h"

/* Struct variable DB */
struct DATABASE DB;

/* The devicetree node identifier for the "ledx" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
struct gpio_dt_spec led0_dev = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
struct gpio_dt_spec led1_dev = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
struct gpio_dt_spec led2_dev = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
struct gpio_dt_spec led3_dev = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

/**
 * @brief Configures the output for the LEDs.
 *
 * This function initializes and configures the GPIO pins
 * for the LEDs. It sets the GPIO pins to be in an output state
 * and ensures the LEDs are initially turned off.
 */
void outputs_config()
{
    int ret = 0;

    /* Check if led0 device is ready */
    if (!device_is_ready(led0_dev.port))  
	{
        printk("Fatal error: led0 device not ready!");
		return;
	}

    /* Configure led0 GPIO pin */
    ret = gpio_pin_configure_dt(&led0_dev, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) 
    {
        printk("Failed to configure led0 \n\r");
	    return;
    }

    /* Check if led1 device is ready */
	if (!device_is_ready(led1_dev.port))  
	{
        printk("Fatal error: led1 device not ready!");
		return;
	}
    /* Configure led1 GPIO pin */
    ret = gpio_pin_configure_dt(&led1_dev, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) 
    {
        printk("Failed to configure led1 \n\r");
	    return;
    }

    /* Check if led2 device is ready */
	if (!device_is_ready(led2_dev.port))  
	{
        printk("Fatal error: led2 device not ready!");
		return;
	}
    /* Configure led2 GPIO pin */
    ret = gpio_pin_configure_dt(&led2_dev, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) 
    {
        printk("Failed to configure led0 \n\r");
	    return;
    }

    /* Check if led3 device is ready */
	if (!device_is_ready(led3_dev.port))  
	{
        printk("Fatal error: led2 device not ready!");
		return;
	}
    /* Configure led3 GPIO pin */
    ret = gpio_pin_configure_dt(&led3_dev, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) 
    {
        printk("Failed to configure led0 \n\r");
	    return;
    }

    /* Turn off all LEDs initially */
    gpio_pin_set_dt(&led0_dev,0);
    gpio_pin_set_dt(&led1_dev,0);
	gpio_pin_set_dt(&led2_dev,0);
	gpio_pin_set_dt(&led3_dev,0);
}

/**
 * @brief Initialize threads, pins, and UART.
 *
 * This is the main entry point for the program. It resets the database
 * variables, configures the outputs, initializes the UART, configures
 * the buttons, and sets up the threads.
 *
 * @return Always returns 0.
 */
int main(void)
{
	/* Reset buttons and adc variables of database */
    DB.BUTTON1 = 0;
    DB.BUTTON2 = 0;
    DB.BUTTON3 = 0;
    DB.BUTTON4 = 0;
    DB.OUTPUT1 = 0;
    DB.OUTPUT2 = 0;
    DB.Pot_Voltage = 0;

	/* Initialize setups of outputs, uart and threads */
    outputs_config();
    uart_init();
    button_config();
    configure_threads();

	return 0;
}
