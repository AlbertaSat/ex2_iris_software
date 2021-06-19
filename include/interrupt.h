/**
 * @file interrupt.h
 * @author Andrew R. Rooney
 * @brief 
 * @version 0.1
 * @date 2021-06-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "hwlib.h"
#include "alt_interrupt.h"

/*
 * Initializes and enables the interrupt controller.
*/
static inline ALT_STATUS_CODE socfpga_int_start(const ALT_INT_INTERRUPT_t int_id,
                                  const alt_int_callback_t callback,
                                  void * context)
{
    ALT_STATUS_CODE status = ALT_E_SUCCESS;

    /*
     * Initialize the global and CPU interrupt items
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_global_init();
    }

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_cpu_init();
    }

    /*
     * Setup the interrupt specific items
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_isr_register(int_id, callback, context);
    }

    if ((status == ALT_E_SUCCESS)
        && (int_id >= 32)) /* Ignore target_set() for non-SPI interrupts. */
    {
        int target = 0x3; /* 1 = CPU0, 2=CPU1 */ 
        status = alt_int_dist_target_set(int_id, target);
    }

    /*
     * Enable the distributor, CPU, and global interrupt
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_dist_enable(int_id);
    }

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_cpu_enable();
    }

    if (status == ALT_E_SUCCESS)
    {
        status = alt_int_global_enable();
    }

    return status;
}

static inline void socfpga_int_stop(const ALT_INT_INTERRUPT_t int_id)
{
    /*
     * Disable the global, CPU, and distributor interrupt
    */

    alt_int_global_disable();

    alt_int_cpu_disable();

    alt_int_dist_disable(int_id);

    /*
     * Unregister the ISR.
    */

    alt_int_isr_unregister(int_id);

    /*
     * Uninitialize the CPU and global data structures.
    */

    alt_int_cpu_uninit();

    alt_int_global_uninit();
}

#endif //__INTERRUPT_H