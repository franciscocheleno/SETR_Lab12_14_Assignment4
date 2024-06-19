/**
 * \file threads.c
 * \author Francisco Heleno <francisco.c.heleno@ua.pt> and Hugo Carola <hugocarola@ua.pt> based on sample
 * code given by Prof. Paulo Pedreiras
 *
 * \date 1, June, 2024
 * \brief Implementation of thread functions for UART, Inputs, Outputs, ADC, and LEDs.
 */

#include "threads.h"
#include "uart.h"
#include "IO.h"
#include "adc.h"

#define STACK_SIZE 1024                     /**< Size of stack area used by each thread (can be thread specific, if necessary) */

/**< Thread scheduling priority */
#define thread_UART_prio 1
#define thread_INPUTS_prio 1
#define thread_OUTPUTS_prio 1
#define thread_Led_1_prio 1
#define thread_Led_2_prio 1
#define thread_Led_3_prio 1
#define thread_Led_4_prio 1 
#define thread_ADC_prio 1

/* Thread periodicity (in ms)*/
float thread_UART_period = 1000;
float thread_INPUTS_period = 200;
float thread_OUTPUTS_period = 200;
float thread_ADC_period = 200;

/**< Create thread stack space */
K_THREAD_STACK_DEFINE(thread_UART_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_INPUTS_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_OUTPUTS_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_Led_1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_Led_2_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_Led_3_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_Led_4_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_ADC_stack, STACK_SIZE);

/**< Create variables for thread data */
struct k_thread thread_UART_data;
struct k_thread thread_INPUTS_data;
struct k_thread thread_OUTPUTS_data;
struct k_thread thread_Led_1_data;
struct k_thread thread_Led_2_data;
struct k_thread thread_Led_3_data;
struct k_thread thread_Led_4_data;
struct k_thread thread_ADC_data;

/**< Create task IDs */
k_tid_t thread_UART_tid;                              
k_tid_t thread_INPUTS_tid;                             
k_tid_t thread_OUTPUTS_tid;
k_tid_t thread_Led_1_tid;                              
k_tid_t thread_Led_2_tid;                              
k_tid_t thread_Led_3_tid;
k_tid_t thread_Led_4_tid;
k_tid_t thread_ADC_tid;

/**< Semaphore for Task access synchronization */
struct k_sem sem_adc;                   
struct k_sem sem_outputs;                   
struct k_sem sem_Led_1_update;              
struct k_sem sem_Led_2_update; 
struct k_sem sem_Led_3_update; 
struct k_sem sem_Led_4_update;              

/* Global vars (shared memory between tasks) */
extern uint8_t Led_1_newState;
extern uint8_t Led_2_newState;
extern uint8_t Led_3_newState;
extern uint8_t Led_4_newState;

void configure_threads()
{

    /* Create and init semaphores */
    k_sem_init(&sem_adc, 1, 1);
    k_sem_init(&sem_outputs, 0, 1);
    k_sem_init(&sem_Led_1_update, 0, 1);
    k_sem_init(&sem_Led_2_update, 0, 1);
    k_sem_init(&sem_Led_3_update, 0, 1);
    k_sem_init(&sem_Led_4_update, 0, 1);


    thread_UART_tid = k_thread_create(&thread_UART_data, thread_UART_stack,
        K_THREAD_STACK_SIZEOF(thread_UART_stack), thread_UART_code,
        NULL, NULL, NULL, thread_UART_prio, 0, K_NO_WAIT);

    thread_INPUTS_tid = k_thread_create(&thread_INPUTS_data, thread_INPUTS_stack,
        K_THREAD_STACK_SIZEOF(thread_INPUTS_stack), thread_INPUTS_code,
        NULL, NULL, NULL, thread_INPUTS_prio, 0, K_NO_WAIT);

    thread_OUTPUTS_tid = k_thread_create(&thread_OUTPUTS_data, thread_OUTPUTS_stack,
        K_THREAD_STACK_SIZEOF(thread_OUTPUTS_stack), thread_OUTPUTS_code,
        NULL, NULL, NULL, thread_OUTPUTS_prio, 0, K_NO_WAIT);

    thread_ADC_tid = k_thread_create(&thread_ADC_data, thread_ADC_stack,
        K_THREAD_STACK_SIZEOF(thread_ADC_stack), thread_ADC_read,
        NULL, NULL, NULL, thread_ADC_prio, 0, K_NO_WAIT);

    thread_Led_1_tid = k_thread_create(&thread_Led_1_data, thread_Led_1_stack,
        K_THREAD_STACK_SIZEOF(thread_Led_1_stack), thread_Led_1_code,
        NULL, NULL, NULL, thread_Led_1_prio, 0, K_NO_WAIT);

    thread_Led_2_tid = k_thread_create(&thread_Led_2_data, thread_Led_2_stack,
        K_THREAD_STACK_SIZEOF(thread_Led_2_stack), thread_Led_2_code,
        NULL, NULL, NULL, thread_Led_2_prio, 0, K_NO_WAIT);
    
    thread_Led_3_tid = k_thread_create(&thread_Led_3_data, thread_Led_3_stack,
        K_THREAD_STACK_SIZEOF(thread_Led_3_stack), thread_Led_3_code,
        NULL, NULL, NULL, thread_Led_3_prio, 0, K_NO_WAIT);

    thread_Led_4_tid = k_thread_create(&thread_Led_4_data, thread_Led_4_stack,
        K_THREAD_STACK_SIZEOF(thread_Led_4_stack), thread_Led_4_code,
        NULL, NULL, NULL, thread_Led_4_prio, 0, K_NO_WAIT);
}

void thread_Led_1_code(void *argA , void *argB, void *argC)
{
    /* Thread loop */
    while(1) 
    {   
        k_sem_take(&sem_Led_1_update,  K_FOREVER);
        DB.OUTPUT1 = Led_1_newState;
    }
}

void thread_Led_2_code(void *argA , void *argB, void *argC)
{
    /* Thread loop */
    while(1) 
    {   
        k_sem_take(&sem_Led_2_update,  K_FOREVER);
        DB.OUTPUT2 = Led_2_newState;
    }
}

void thread_Led_3_code(void *argA , void *argB, void *argC)
{
    /* Thread loop */
    while(1) 
    {   
        k_sem_take(&sem_Led_3_update,  K_FOREVER);
        DB.OUTPUT3 = Led_3_newState;
    }
}

void thread_Led_4_code(void *argA , void *argB, void *argC)
{
    /* Thread loop */
    while(1) 
    {   
        k_sem_take(&sem_Led_4_update,  K_FOREVER);
        DB.OUTPUT4 = Led_4_newState;
    }
}

void thread_UART_code(void *argA , void *argB, void *argC)
{
    /* Local vars */
    int64_t fin_time = 0;
    int64_t release_time = 0;     /* Timing variables to control task periodicity */

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_UART_period;

    /* Thread loop */
    while(1) 
    {   
        print_UI();
        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) 
        {
            k_msleep(release_time - fin_time);
            release_time += thread_UART_period;
        }
    }
}

void thread_INPUTS_code()
{

    /* Local vars */
    int64_t fin_time = 0;
    int64_t release_time = 0;     /* Timing variables to control task periodicity */

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_INPUTS_period;

    /* Thread loop */
    while(1) 
    {       
        DB.BUTTON1 = button_state[0];
        DB.BUTTON2 = button_state[1];
        DB.BUTTON3 = button_state[2];
        DB.BUTTON4 = button_state[3];

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) 
        {
            k_msleep(release_time - fin_time);
            release_time += thread_INPUTS_period;
        }
    }
}

void thread_OUTPUTS_code()
{
    /* Local vars */
    int64_t fin_time = 0;
    int64_t release_time = 0;     /* Timing variables to control task periodicity */

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_OUTPUTS_period;
    /* Thread loop */
    while(1) 
    { 
        k_sem_take(&sem_outputs,  K_FOREVER);   
        if(DB.OUTPUT1 == 1)
        {
            gpio_pin_set_dt(&led0_dev,1);
        }
        else if(DB.OUTPUT1 == 0)
        {
            gpio_pin_set_dt(&led0_dev,0);
        } 

        if(DB.OUTPUT2 == 1)
        {
            gpio_pin_set_dt(&led1_dev,1);
        }
        else if(DB.OUTPUT2 == 0)
        {
            gpio_pin_set_dt(&led1_dev,0);
        } 
        k_sem_give(&sem_outputs);   

        if(DB.OUTPUT3 == 1)
        {
            gpio_pin_set_dt(&led2_dev,1);
        }
        else if(DB.OUTPUT3 == 0)
        {
            gpio_pin_set_dt(&led2_dev,0);
        } 
        k_sem_give(&sem_outputs);

        if(DB.OUTPUT4 == 1)
        {
            gpio_pin_set_dt(&led3_dev,1);
        }
        else if(DB.OUTPUT4 == 0)
        {
            gpio_pin_set_dt(&led3_dev,0);
        } 
        k_sem_give(&sem_outputs);

        fin_time = k_uptime_get();
        if( fin_time < release_time) 
        {
            k_msleep(release_time - fin_time);
            release_time += thread_OUTPUTS_period;
        }
    }
}

void thread_ADC_read()
{

    /* Local vars */
    int64_t fin_time=0;
    int64_t release_time=0;     /* Timing variables to control task periodicity */

    int err = 0;

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_UART_period;

    /* Main loop */
    while(1)
    {
    int8_t volt = 0;
    int8_t volt_to_temp = 0;

        /* Get one sample, checks for errors and prints the values */
        err = adc_sample();
        if(err) {
            printk("adc_sample() failed with error code %d\n\r",err);
        }
        else {
            if(adc_sample_buffer[0] > 1023) {
                printk("adc reading out of range (value is %u)\n\r", adc_sample_buffer[0]);
            }
            else {
                /* ADC is set to use gain of 1/4 and reference VDD/4, so input range is 0...VDD (3 V), with 10 bit resolution */
                volt = (uint8_t)(1000*adc_sample_buffer[0]*((float)3/1023));
                //printk("\nadc reading: raw:%4u / %4u mV: \n\r",adc_sample_buffer[0],volt);
                volt_to_temp = 60*volt-60;
            }
        }
    k_sem_take(&sem_adc,  K_FOREVER);
    DB.Pot_Voltage = volt_to_temp;
    k_sem_give(&sem_adc);
    
        fin_time = k_uptime_get();
        if( fin_time < release_time) 
        {
            k_msleep(release_time - fin_time);
            release_time += thread_UART_period;
        }
    }
}