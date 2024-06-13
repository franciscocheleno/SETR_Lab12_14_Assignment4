/**
 * \file:    uart.h
 * \author: Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Implementation of the command processing and UART communication.
 * 
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
#define MAIN_SLEEP_TIME_MS 1000 /* Time between main() activations */ 

#define FATAL_ERR -1 /* Fatal error return code, app terminates */

#define RXBUF_SIZE 60                   /* RX buffer size */
#define TXBUF_SIZE 60                   /* TX buffer size */
#define MSG_BUF_SIZE 100                /* Buffer for messages sent via UART */
#define RX_TIMEOUT 1000                 /* Inactivity period after the instant when last char was received that triggers an rx event (in us) */

extern uint8_t rx_buf[RXBUF_SIZE];      /* RX buffer, to store received data */
extern uint8_t rx_chars[RXBUF_SIZE];    /* chars actually received  */
extern volatile int uart_rxbuf_nchar;        /* Number of chars currrntly on the rx buffer */

void uartconfig();

void print_interface();

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

void enter_routine(uint8_t rx_chars_aux[RXBUF_SIZE]);            // Executed when "Enter" is pressed on keyboard

#endif