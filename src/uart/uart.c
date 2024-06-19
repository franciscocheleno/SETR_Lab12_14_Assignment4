/**
 * \file:   uart.c
 * \author: Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Implementation of the command processing and UART communication.
 * 
 * This file implements the UART communication, including initialization,
 * configuration, and command processing. It includes the UART callback
 * function and helper functions for reading user input and printing the UI.
 */

#include "uart.h"
#include "threads.h"

/* UART related variables */
const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);   /**< UART device instance */
uint8_t RX_buf[RXBUF_SIZE];                                 /**< RX buffer, to store received data */
uint8_t RX_chars[RXBUF_SIZE];                               /**< chars actually received  */
volatile int uart_RXbuf_nchar = 0;                          /**< Number of chars currrntly on the rx buffer */
uint8_t command_state[RXBUF_SIZE];                          /**< Output sent to the interface depending on user commands */
uint8_t Led_1_newState;                                     /**< New state for LED 1 */
uint8_t Led_2_newState;                                     /**< New state for LED 2 */
uint8_t Led_3_newState;                                     /**< New state for LED 3 */
uint8_t Led_4_newState;                                     /**< New state for LED 4 */                


/* Struct for UART configuration (if using default values is not needed) */
const struct uart_config uart_cfg = 
{
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

void print_UI()
{
    printf("\033[2J\033[H");
    printf("\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("\n UART frequency: %fHz", (int)1/(thread_UART_period*0.001));
    printf("\n Buttons frequency: %.0fHz", (int)1/(thread_INPUTS_period * 0.001));
    printf("\n ADC frequency: %.0fHz", (int)1/(thread_ADC_period * 0.001));
    printf("\n Outputs(LEDs) frequency: %.0fHz", (int)1/(thread_OUTPUTS_period * 0.001));
    printf("\n");
    printf("\n %s",command_state);
    printf("\n");
    printf("#---------------------------------------------------------------------------------------------------------------------#\n");
    printf(" Available commands:");
    printf("\n  \033[0;32m/fuxxx /fbxxx /faxxx /foxxx \033[0;37m- (Change frequency of UART, buttons, ADC and outputs(LEDs), xxx is desired frequency)");
    printf("\n  \033[0;32m/bx \033[0;37m- (Check Button State)");
    printf("\n  \033[0;32m/ox_y \033[0;37m- (Active (y=1) or Disable (y=0) Led x)"); 
    printf("\n  \033[0;32m/a \033[0;37m- (See ADC value)");
    printf("\n#---------------------------------------------------------------------------------------------------------------------#\n");
    printf("\n String sent: %s",RX_chars);
}

int uart_init()
{
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
    err =  uart_rx_enable(uart_dev ,RX_buf,sizeof(RX_buf),RX_TIMEOUT);
    if (err) {
        printk("uart_rx_enable() error. Error code:%d\n\r",err);
        return FATAL_ERR;
    }
}

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    int err;

    switch (evt->type) {
	
        case UART_TX_DONE:
		    printk("\nUART_TX_DONE event \n\r");
            break;

    	case UART_TX_ABORTED:
	    	printk("\nUART_TX_ABORTED event \n\r");
		    break;
		
	    case UART_RX_RDY:
		    printk("\nUART_RX_RDY event \n\r");
            memcpy(&RX_chars[uart_RXbuf_nchar],&(RX_buf[evt->data.rx.offset]),evt->data.rx.len); 
            if(RX_chars[uart_RXbuf_nchar] == '\r')
            {
                RX_chars[uart_RXbuf_nchar + 1] = '\0'; 
                read_user_inp(RX_chars);
                RX_chars[0] = '\0';
                uart_RXbuf_nchar = 0;
            }
            else if(RX_chars[uart_RXbuf_nchar] == 127)
            {
                if(uart_RXbuf_nchar - 1 >= 0)
                {
                    RX_chars[uart_RXbuf_nchar - 1] = '\0';
                    uart_RXbuf_nchar = uart_RXbuf_nchar - 1;
                }
            }
            else
            {
                RX_chars[uart_RXbuf_nchar + 1] = '\0';
                uart_RXbuf_nchar++;   
            }           
		    break;

	    case UART_RX_BUF_REQUEST:
		    printk("\nUART_RX_BUF_REQUEST event \n\r");
 		    break;

	    case UART_RX_BUF_RELEASED:
		    printk("\nUART_RX_BUF_RELEASED event \n\r");
		    break;
		
	    case UART_RX_DISABLED:
            printk("\nUART_RX_DISABLED event \n\r");
		    err =  uart_rx_enable(uart_dev ,RX_buf,sizeof(RX_buf),RX_TIMEOUT);
            if (err) {
                printk("\nuart_rx_enable() error. Error code:%d\n\r",err);
                exit(FATAL_ERR);                
            }
		    break;

	    case UART_RX_STOPPED:
		    printk("\nUART_RX_STOPPED event \n\r");
		    break;
		
	    default:
            printk("\nUART: unknown event \n\r");
		    break;
    }

}

void read_user_inp(uint8_t RX_chars_user[RXBUF_SIZE])
{
    strcpy(RX_chars,RX_chars_user);

    /* SET Frequency COMMAND 
    * For frequency set 20Hz to adc, buttons and outputs
    * /fa20
    * /fb20
    * /fo20
    */
    if(RX_chars[0] == '/' && RX_chars[1] == 'f' && (RX_chars[2] == 'b' || RX_chars[2] == 'a' || RX_chars[2] == 'o' || RX_chars[2] == 'u') )   
    {
        char *init = strchr(RX_chars, 'f');
        char *end = strchr(RX_chars, '\r');
        int len = end - init -2;
        char number_aux[len];
        strncpy(number_aux, init+2, len);
        number_aux[len+1] = '\0';
        if(RX_chars[2] == 'u')
        {
            thread_UART_period = 1/(atoi(number_aux) * 0.001);
            printf("\nUART frequency: %f",thread_UART_period);
        }
        if(RX_chars[2] == 'b')
        {
            thread_INPUTS_period = 1/(atoi(number_aux) * 0.001);
            printf("\nButtons frequency: %f",thread_INPUTS_period);
        }
        if(RX_chars[2] == 'a')
        {
            thread_ADC_period = 1/(atoi(number_aux) * 0.001);
            printf("\nADC frequency: %f",thread_ADC_period);
        }
        if(RX_chars[2] == 'o')
        {
            thread_OUTPUTS_period = 1/(atoi(number_aux) * 0.001);
            printf("\nLEDs frequency: %f",thread_OUTPUTS_period);
        }
    }

    /* Read button state COMMAND
    *   /bx
    *   x - button to be read, available buttons 1-4.
    */ 
    else if(RX_chars[0] == '/' && RX_chars[1] == 'b' && (isdigit(RX_chars[2]) == 1 ))            
    {
        if(RX_chars[2] == '1')
        {
            uint8_t str_aux[RXBUF_SIZE];
            uint8_t str_message[] = "Button 1 state: "; 
            sprintf(str_aux,"%i",DB.BUTTON1);
            strcat(str_message,str_aux);
            strcpy(command_state,str_message);
        }
        else if(RX_chars[2] == '2')
        {
            uint8_t str_aux[RXBUF_SIZE];
            uint8_t str_message[] = "Button 2 state: "; 
            sprintf(str_aux,"%i",DB.BUTTON2);
            strcat(str_message,str_aux);
            strcpy(command_state,str_message);
        }
        else if(RX_chars[2] == '3')
        {
            uint8_t str_aux[RXBUF_SIZE];
            uint8_t str_message[] = "Button 3 state: "; 
            sprintf(str_aux,"%i",DB.BUTTON3);
            strcat(str_message,str_aux);
            strcpy(command_state,str_message);
        }
        else if(RX_chars[2] == '4')
        {
            uint8_t str_aux[RXBUF_SIZE];
            uint8_t str_message[] = "Button 4 state: "; 
            sprintf(str_aux,"%i",DB.BUTTON4);
            strcat(str_message,str_aux);
            strcpy(command_state,str_message);
        }
        else
        {
            printf("\nInvalid command");
            return;
        }
    }

    /* Set output state COMMAND
    *   /ox_y
    *   x - output to be set, outputs available 1-4.
    *   y - state of the output
    */
    else if(RX_chars[0] == '/' && RX_chars[1] == 'o' && (isdigit(RX_chars[2]) == 1) && RX_chars[3] == '_' && (RX_chars[4] == '1' || RX_chars[4] == '0') )      
    {
        if(RX_chars[2] == '1')
        {
            k_sem_take(&sem_outputs,  K_FOREVER);   
            Led_1_newState = RX_chars[4] - '0';
            k_sem_give(&sem_Led_1_update);
            k_sem_give(&sem_outputs);
        }
        else if(RX_chars[2] == '2')
        {
            k_sem_take(&sem_outputs,  K_FOREVER);   
            Led_2_newState = RX_chars[4] - '0';
            k_sem_give(&sem_Led_2_update);
            k_sem_give(&sem_outputs);
        }
        else if(RX_chars[2] == '3')
        {
            k_sem_take(&sem_outputs,  K_FOREVER);   
            Led_3_newState = RX_chars[4] - '0';
            k_sem_give(&sem_Led_3_update);
            k_sem_give(&sem_outputs);
        }
        else if(RX_chars[2] == '4')
        {
            k_sem_take(&sem_outputs,  K_FOREVER);   
            Led_4_newState = RX_chars[4] - '0';
            k_sem_give(&sem_Led_4_update);
            k_sem_give(&sem_outputs);
        }
        else
        {
            printf("\nInvalid command");
            return;
        }

    }

    /* Read ADC state COMMAND 
    * /a
    */
    else if(RX_chars[0] == '/' && RX_chars[1] == 'a')            
    {
        uint8_t str_aux[RXBUF_SIZE];
        uint8_t str_message[] = "ADC value is: "; 
        k_sem_take(&sem_adc,  K_FOREVER);
        sprintf(str_aux,"%d",DB.Pot_Voltage);
        k_sem_give(&sem_adc); 
        strcat(str_message,str_aux);
        strcpy(command_state,str_message);
    }

    else
    {
        printf("\nInvalid Command");
        return;
    }
}