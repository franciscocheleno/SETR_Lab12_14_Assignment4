/**
 * \file:   threads.h
 * \author: Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Implementation of the command processing and UART communication.
 * 
 */

#include <zephyr/kernel.h>          /* for kernel functions*/
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/timing/timing.h>   /* for timing services */
#include <stdio.h>
#include <string.h>

#ifndef threads_H
#define threads_H

/**
 * \struct DATABASE
 * \brief Data structure to store button and output states along with ADC value.
 */
struct DATABASE
{
    int8_t BUTTON1;       /**< State of Button 1 */
    int8_t BUTTON2;       /**< State of Button 2 */
    int8_t BUTTON3;       /**< State of Button 3 */
    int8_t BUTTON4;       /**< State of Button 4 */
    int8_t OUTPUT1;       /**< State of Output 1 */
    int8_t OUTPUT2;       /**< State of Output 2 */
    int8_t OUTPUT3;       /**< State of Output 3 */
    int8_t OUTPUT4;       /**< State of Output 4 */
    int8_t Pot_Voltage;   /**< Potentiometer Voltage */
};

extern struct DATABASE DB;                      /**< Global database instance */
extern struct k_sem sem_adc;                    /**< Semaphore for ADC */
extern struct k_sem sem_inputs;                 /**< Semaphore for Inputs */
extern struct k_sem sem_outputs;                /**< Semaphore for Outputs */
extern struct k_sem sem_Led_1_update;           /**< Semaphore for LED 1 update */
extern struct k_sem sem_Led_2_update;           /**< Semaphore for LED 2 update */
extern struct k_sem sem_Led_3_update;           /**< Semaphore for LED 3 update */
extern struct k_sem sem_Led_4_update;           /**< Semaphore for LED 4 update */

extern struct gpio_dt_spec led0_dev;            /**< GPIO specification for LED 0 */
extern struct gpio_dt_spec led1_dev;            /**< GPIO specification for LED 1 */
extern struct gpio_dt_spec led2_dev;            /**< GPIO specification for LED 2 */
extern struct gpio_dt_spec led3_dev;            /**< GPIO specification for LED 3 */

extern float thread_UART_period;                /**< Periodicity of UART thread (in ms) */
extern float thread_INPUTS_period;              /**< Periodicity of Inputs thread (in ms) */
extern float thread_OUTPUTS_period;             /**< Periodicity of Outputs thread (in ms) */
extern float thread_ADC_period;                 /**< Periodicity of ADC thread (in ms) */

/**
 * \brief Configures the threads.
 *
 * This function sets up and initializes the threads for UART, inputs, outputs, ADC, and LEDs.
 */
void configure_threads();

/**
 * \brief UART thread function.
 *
 * This function contains the code that runs in the UART thread. It handles UART communication.
 *
 * \param argA Argument A (unused).
 * \param argB Argument B (unused).
 * \param argC Argument C (unused).
 */
void thread_UART_code(void *argA, void *argB, void *argC); 

/**
 * \brief Inputs thread function.
 *
 * This function contains the code that runs in the Inputs thread. It handles input processing.
 */
void thread_INPUTS_code();

/**
 * \brief Outputs thread function.
 *
 * This function contains the code that runs in the Outputs thread. It handles output processing.
 */
void thread_OUTPUTS_code();

/**
 * \brief ADC read thread function.
 *
 * This function contains the code that runs in the ADC thread. It handles ADC value reading.
 */
void thread_ADC_read();

/**
 * \brief LED 1 thread function.
 *
 * This function contains the code that runs in the LED 1 thread. It handles updating the state of LED 1.
 */
void thread_Led_1_code();

/**
 * \brief LED 2 thread function.
 *
 * This function contains the code that runs in the LED 2 thread. It handles updating the state of LED 2.
 */
void thread_Led_2_code();

/**
 * \brief LED 3 thread function.
 *
 * This function contains the code that runs in the LED 3 thread. It handles updating the state of LED 3.
 */
void thread_Led_3_code();

/**
 * \brief LED 4 thread function.
 *
 * This function contains the code that runs in the LED 4 thread. It handles updating the state of LED 4.
 */
void thread_Led_4_code();

#endif /* threads_H */