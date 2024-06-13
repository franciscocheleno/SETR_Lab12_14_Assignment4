/**
 * \file:   uart.c
 * \author: Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Implementation of the command processing and UART communication.
 * 
 */

#include "uart.h"
#include "threads.h"

/* UART related variables */
const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);   /**< UART device instance */
uint8_t rx_buf[RXBUF_SIZE];             /**< RX buffer, to store received data */
uint8_t rx_chars[RXBUF_SIZE];           /**< chars actually received  */
volatile int uart_rxbuf_nchar=0;        /**< Number of chars currrntly on the rx buffer */
uint8_t comand_state[RXBUF_SIZE];       /**< Output sent to the interface depending on user commands */
uint8_t Led_1_new;
uint8_t Led_2_new;                    


/* Struct for UART configuration (if using default values is not needed) */
const struct uart_config uart_cfg = 
{
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

void uart_init(){

	/* Local vars */    
    int err=0; /* Generic error variable */

    /* Check if uart device is open */
    if (!device_is_ready(uart_dev)) {
        printk("device_is_ready(uart) returned error! Aborting! \n\r");
        return FATAL_ERR;
    }

    /* Configure UART */
    err = uart_configure(uart_dev, &uart_cfg);
    if (err == -ENOSYS) { /* If invalid configuration */
        printk("uart_configure() error. Invalid configuration\n\r");
        return FATAL_ERR; 
    }
        
    /* Register callback */
    err = uart_callback_set(uart_dev, uart_cb, NULL);
    if (err) {
        printk("uart_callback_set() error. Error code:%d\n\r",err);
        return FATAL_ERR;
    }
		
    /* Enable data reception */
    err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
    if (err) {
        printk("uart_rx_enable() error. Error code:%d\n\r",err);
        return FATAL_ERR;
    }
}

/** UART callback implementation 
 * Note that callback functions are executed in the scope of interrupt handlers. 
 * They run asynchronously after hardware/software interrupts and have a higher priority than tasks/threads 
 * Should be short and simple. Heavier processing should be deferred to a task with suitable priority
 * \param dev The UART device.
 * \param evt The UART event.
 * \param user_data User data.
 */

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    int err;

    switch (evt->type) {
	
        case UART_TX_DONE:
		    printk("UART_TX_DONE event \n\r");
            break;

    	case UART_TX_ABORTED:
	    	printk("UART_TX_ABORTED event \n\r");
		    break;
		
	    case UART_RX_RDY:
		    printk("UART_RX_RDY event \n\r");
            /* Just copy data to a buffer. */
            /* Simple approach, just for illustration. In most cases it is necessary to use */
            /*    e.g. a FIFO or a circular buffer to communicate with a task that shall process the messages*/
            memcpy(&rx_chars[uart_rxbuf_nchar],&(rx_buf[evt->data.rx.offset]),evt->data.rx.len); 
            uart_rxbuf_nchar++;           
		    break;

	    case UART_RX_BUF_REQUEST:
		    printk("UART_RX_BUF_REQUEST event \n\r");
 		    break;

	    case UART_RX_BUF_RELEASED:
		    printk("UART_RX_BUF_RELEASED event \n\r");
		    break;
		
	    case UART_RX_DISABLED: 
            /* When the RX_BUFF becomes full RX is disabled automaticaly.  */
            /* It must be re-enabled manually for continuous reception */
            printk("UART_RX_DISABLED event \n\r");
		    err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
            if (err) {
                printk("uart_rx_enable() error. Error code:%d\n\r",err);
                exit(FATAL_ERR);                
            }
		    break;

	    case UART_RX_STOPPED:
		    printk("UART_RX_STOPPED event \n\r");
		    break;
		
	    default:
            printk("UART: unknown event \n\r");
		    break;
    }

}

void read_user_inp(uint8_t rx_chars_user[RXBUF_SIZE]){



}
