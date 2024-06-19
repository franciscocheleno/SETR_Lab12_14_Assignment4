/*
 * File:   IO.c
 * Authors: Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt>
 * Based on sample code given by Prof. Paulo Pedreiras
 *
 * Date: 1, June, 2024
 * Brief: Implementation of the command processing and UART communication.
 */

#include <zephyr/kernel.h>          // for k_msleep()
#include <zephyr/device.h>          // for device_is_ready() and device structure
#include <zephyr/devicetree.h>      // for DT_NODELABEL()
#include <zephyr/drivers/gpio.h>    // for GPIO api
#include <zephyr/sys/printk.h>      // for printk()
#include "IO.h"

const uint8_t buttons_pins[] = {11,12,24,25};             // Vector with pins where buttons are connected
static const struct device * gpio0_dev = DEVICE_DT_GET(GPIO0_NODE); // Now get the device pointer for GPIO0

/* Define a variable of type static struct gpio_callback, which will latter be used to install the callback
*  It defines e.g. which pin triggers the callback and the address of the function */
static struct gpio_callback button_cb_data;

uint8_t button_state[4];    // Array to track all buttons state

/*
 * Callback function for button presses.
 * It updates the button_state array based on which buttons are pressed.
 */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    for(int i=0; i<sizeof(buttons_pins); i++)
    {
        button_state[i] = !gpio_pin_get(gpio0_dev, buttons_pins[i]);
    }
}

/*
 * Function that configures the GPIO pins as inputs and initializes the interrupt routine.
 * It sets up the GPIO pins, configures them for interrupt handling, and initializes the callback function.
 */
void button_config()
{
    int ret;
    int i;
    uint32_t pinmask = 0;   // Mask for setting the pins that shall generate interrupts
	
    /* Welcome message */
    printk("Digital IO accessing IO pins not set via DT (external buttons in the case) \n\r");
    printk("Hit buttons 1-8 (1...4 internal, 5-8 external connected to A0...A3). Led toggles and button ID printed at console \n\r");

    /* Check if gpio0 device is ready */
    if (!device_is_ready(gpio0_dev)) 
    {
        printk("Error: gpio0 device is not ready\n");
        return;
    } 
    else 
    {
        printk("Success: gpio0 device is ready\n");
    }

    /* Configure GPIO pins as inputs with pull-up resistors */
    for(i=0; i<sizeof(buttons_pins); i++) 
    {
        ret = gpio_pin_configure(gpio0_dev, buttons_pins[i], GPIO_INPUT | GPIO_PULL_UP);
        if (ret < 0) 
        {
            printk("Error: gpio_pin_configure failed for button %d/pin %d, error:%d\n\r", i+1, buttons_pins[i], ret);
            return;
        } 
        else 
        {
            printk("Success: gpio_pin_configure for button %d/pin %d\n\r", i+1, buttons_pins[i]);
        }
    }

    /* Configure interrupt on the button's pin */
    for(i=0; i<sizeof(buttons_pins); i++) 
    {
        ret = gpio_pin_interrupt_configure(gpio0_dev, buttons_pins[i], GPIO_INT_EDGE_BOTH);
        if (ret < 0) 
        {
            printk("Error: gpio_pin_interrupt_configure failed for button %d / pin %d, error:%d", i+1, buttons_pins[i], ret);
            return;
        }
    }

    /* HW init done!*/
    printk("All devices initialized successfully!\n\r");

    /* Initialize the static struct gpio_callback variable   */
    pinmask=0;
    for(i=0; i<sizeof(buttons_pins); i++) 
    {
        pinmask |= BIT(buttons_pins[i]);
    }
    gpio_init_callback(&button_cb_data, button_pressed, pinmask); 
    
    /* Add the callback function by calling gpio_add_callback()   */
    gpio_add_callback(gpio0_dev, &button_cb_data);
}
