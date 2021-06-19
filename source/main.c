/**
 * @file timer.h
 * @author Andrew Rooney
 * @date 2021-06-19
 */
#include "alt_printf.h"
#include "alt_generalpurpose_io.h"
#include "alt_interrupt.h"
#include "alt_timers.h"
#include "alt_watchdog.h"
#include "alt_16550_uart.h"
#include <string.h>

#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_rstmgr.h"

#include "interrupt.h"
#include "timer.h"

/* enable semihosting with Arm DS by defining an __auto_semihosting symbol 
   will be removed by linker if semihosting is disabled */
int __auto_semihosting;

#define BUFFERSIZE 128

static const ALT_16550_DEVICE_t g_UartDevice = ALT_16550_DEVICE_SOCFPGA_UART0;
static ALT_16550_HANDLE_t g_Handle;
static const alt_freq_t g_BaudRate = 115200;
static const uint32_t g_TickPeriodMS = 50;

static char g_DoubleBuffer[2*BUFFERSIZE];
static char * const g_BaseA = g_DoubleBuffer;
static int g_APtr = 0;
static char * const g_BaseB = g_DoubleBuffer + BUFFERSIZE;
static int g_BPtr = 0;
static bool g_WriteA_WriteB_b = false;

static bool _Tick = false;

static inline size_t process_uart_data(char * const buffer);

/**
 * @brief Main loop of this bare metal application. Will process available data once
 * every tick
 * 
 */
static void main_processing_loop()
{
    char buffer[BUFFERSIZE];
    size_t bufferSize = 0;
    for ( ;; )
    {
        while(!_Tick);
        _Tick = false;
        alt_wdog_reset(ALT_WDOG0);
        /**
         * Process data, service requests.
         */
        bufferSize = process_uart_data(buffer);
        if (bufferSize > 0) {
            // Process control characters.
        }
        alt_16550_fifo_write(&g_Handle, buffer, bufferSize);
    }
}

/**
 * @brief Read UART ping-pong buffer into given buffer
 * 
 * @param buffer buffer to write incoming data to
 * @return size_t size read from ping-pong buffer
 */
static inline size_t process_uart_data(char * const buffer)
{
    size_t bufferSize = 0;

    if (g_APtr > 0 || g_BPtr > 0) {
        if (g_WriteA_WriteB_b == true) {
            // Read from A
            memcpy(buffer, g_BaseA, g_APtr);
            bufferSize = g_APtr;
            g_APtr = 0;
        } else {
            // Read from B
            memcpy(buffer, g_BaseB, g_BPtr);
            bufferSize = g_BPtr;
            g_BPtr = 0;
        }
        g_WriteA_WriteB_b = !g_WriteA_WriteB_b;
    }
    return bufferSize;
}

/**
 * @brief Timer callback: set tick flag to orechestrate applciaiton timing
 * 
 * @param icciar 
 * @param context 
 */
void test_timer_int_callback(uint32_t icciar, void * context)
{

    /*
     * Clear the timer interrupt, set tick flag
    */
    alt_gpt_int_if_pending_clear(ALT_GPT_CPU_PRIVATE_TMR);

    _Tick = true;

    return;
}

/**
 * @brief Process uart interrupt. If data is available, write to the ping-pong buffer
 * 
 * @param icciar 
 * @param context 
 */
void uart_int_callback(uint32_t icciar, void * context)
{
    ALT_16550_INT_STATUS_t interruptStatus;
    ALT_STATUS_CODE status;
    uint32_t level;

    status = alt_16550_int_status_get(&g_Handle, &interruptStatus);

    if (status == ALT_E_SUCCESS) {
        if (interruptStatus == ALT_16550_INT_STATUS_RX_DATA) {
            status = alt_16550_fifo_level_get_rx(&g_Handle, &level);
            if (status == ALT_E_SUCCESS) {
                if (g_WriteA_WriteB_b == true) {
                    // write to buffer A
                   alt_16550_fifo_read(&g_Handle, g_BaseA + g_APtr, level);
                   g_APtr += level;
                } else {
                    // write to buffer B
                    alt_16550_fifo_read(&g_Handle, g_BaseB + g_BPtr, level);
                    g_BPtr += level;
                }
            }
        }
    }
    return;
}


int main(int argc, char** argv)
{
    ALT_STATUS_CODE status = ALT_E_SUCCESS;

    if (status == ALT_E_SUCCESS) {
        status = alt_wdog_init();
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_wdog_counter_set(ALT_WDOG0, ALT_WDOG_TIMEOUT2M);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_wdog_response_mode_set(ALT_WDOG0, ALT_WDOG_WARM_RESET);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_wdog_start(ALT_WDOG0);

    }

    if (status == ALT_E_SUCCESS) {
        status = alt_wdog_stop(ALT_WDOG1);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_16550_init(g_UartDevice, NULL, g_BaudRate, &g_Handle);
    }


    if (status == ALT_E_SUCCESS) {
        status = alt_16550_fifo_enable(&g_Handle);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_16550_enable(&g_Handle);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_16550_int_enable_rx(&g_Handle);
    }

    if (status == ALT_E_SUCCESS)
    {
        status = socfpga_int_start(ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE,
                                   test_timer_int_callback, NULL);
    }

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_dist_enable(ALT_INT_INTERRUPT_UART0);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_int_dist_target_set(ALT_INT_INTERRUPT_UART0, 0x3);
    }

    if (status == ALT_E_SUCCESS) {
        status = alt_int_isr_register(ALT_INT_INTERRUPT_UART0, uart_int_callback, NULL);
    }

    if (status == ALT_E_SUCCESS)
    {
        status = socfpga_timer_start(ALT_GPT_CPU_PRIVATE_TMR, g_TickPeriodMS);
    }

    main_processing_loop();

    // SHOULD NOT GET HERE!

    /*
     * Stop the timer system
    */

    socfpga_timer_stop(ALT_GPT_CPU_PRIVATE_TMR);
    

    /*
     * Stop the interrupt system
    */

    socfpga_int_stop(ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE);

    if (status == ALT_E_SUCCESS)
    {
        printf("RESULT: Example completed successfully.\n");
        return 0;
    }
    else
    {
        printf("RESULT: Some failures detected.\n");
        return 1;
    }
}
