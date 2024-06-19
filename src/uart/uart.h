/**
 * \file uart.h
 * \author Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Header file for command processing and UART communication.
 * 
 * This file contains the declarations for the UART communication, including 
 * initialization, configuration, and command processing functions. It also 
 * defines macros and external variables used in the UART implementation.
 */

#include <zephyr/kernel.h>          /* for k_msleep() */
#include <zephyr/device.h>          /* for device_is_ready() and device structure */
#include <zephyr/devicetree.h>	    /* for DT_NODELABEL() */
#include <zephyr/drivers/uart.h>    /* for ADC API*/
#include <zephyr/sys/printk.h>      /* for printk()*/
#include <stdio.h>                  /* for sprintf() */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#ifndef uart_H
#define uart_H

#define UART_NODE DT_NODELABEL(uart0)   /* UART0 node ID*/
#define MAIN_SLEEP_TIME_MS 1000         /* Time between main() activations */ 

#define FATAL_ERR -1                    /* Fatal error return code, app terminates */

#define RXBUF_SIZE 60                   /* RX buffer size */
#define TXBUF_SIZE 60                   /* TX buffer size */
#define MSG_BUF_SIZE 100                /* Buffer for messages sent via UART */
#define RX_TIMEOUT 1000                 /* Inactivity period after the instant when last char was received that triggers an rx event (in us) */

extern uint8_t RX_buf[RXBUF_SIZE];      /* RX buffer, to store received data */
extern uint8_t RX_chars[RXBUF_SIZE];    /* Chars actually received  */
extern volatile int uart_RXbuf_nchar;   /* Number of chars currently on the rx buffer */

/**
 * \brief Prints the UI.
 *
 * This function prints the user interface, showing the frequencies of various
 * threads, available commands, and the last received string.
 */
void print_UI();

/**
 * \brief Initializes the UART.
 *
 * This function initializes and configures the UART device. It sets up the UART
 * configuration, registers the UART callback, and enables data reception.
 *
 * \return 0 on success, FATAL_ERR on failure.
 */
int uart_init();

/**
 * \brief UART callback implementation.
 *
 * This function handles UART events. Callback functions are executed in the scope
 * of interrupt handlers and run asynchronously after hardware/software interrupts.
 * They have a higher priority than tasks/threads and should be short and simple.
 * Heavier processing should be deferred to a task with suitable priority.
 *
 * \param dev The UART device.
 * \param evt The UART event.
 * \param user_data User data.
 */
void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

/**
 * \brief Reads user input from UART.
 *
 * This function processes user input commands received via UART. It handles
 * commands for setting frequencies, reading button states, setting output
 * states, and reading ADC values.
 *
 * \param RX_chars_aux The received user input characters.
 */
void read_user_inp(uint8_t RX_chars_aux[RXBUF_SIZE]);            // Executed when "Enter" is pressed on keyboard

#endif /* uart_H */