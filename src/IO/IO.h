/**
 * \file IO.h
 * \author Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Implementation of the command processing and UART communication.
 */

#ifndef IO_H
#define IO_H

#include <zephyr/kernel.h>          /**< Kernel functions */
#include <zephyr/device.h>          /**< Device management */
#include <zephyr/devicetree.h>      /**< Device tree definitions */
#include <zephyr/drivers/gpio.h>    /**< GPIO access */
#include <zephyr/sys/printk.h>      /**< Kernel logging */
#include <zephyr/timing/timing.h>   /**< Timing services */
#include <stdio.h>                  /**< Standard I/O */
#include <string.h>                 /**< String manipulation */

#define SLEEP_TIME_MS   60*1000

/**
 * \brief Pin definitions for LEDs and buttons.
 *
 * LEDs 1-4 and buttons 1-4 are the ones on board.
 */
#define LED1_PIN 13
#define LED2_PIN 14
#define LED3_PIN 15
#define LED4_PIN 16

#define GPIO0_NODE DT_NODELABEL(gpio0)  /**< Device tree node for GPIO */

extern uint8_t button_state[4];         /**< Array to store button states */

/**
 * \brief Callback function for button presses.
 *
 * \param dev Pointer to the GPIO device structure.
 * \param cb Pointer to the GPIO callback structure.
 * \param pins Bitmask of pins for which the callback was triggered.
 */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * \brief Configure buttons for interrupt-driven input.
 */
void button_config();

#endif /* IO_H */