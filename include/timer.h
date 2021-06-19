/**
 * @file timer.h
 * @author Andrew R. Rooney
 * @brief 
 * @version 0.1
 * @date 2021-06-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __TIMER_H
#define __TIMER_H

#include "hwlib.h"
#include "alt_timers.h"

#include "timer.h"

#include "alt_printf.h"
#include "alt_watchdog.h"
#include "alt_clock_manager.h"

#include "socal/socal.h"


static inline ALT_STATUS_CODE socfpga_timer_start(ALT_GPT_TIMER_t timer, const uint32_t period_in_ms)
{
    ALT_STATUS_CODE status = ALT_E_SUCCESS;
    uint32_t freq;

    /*
     * Determine the frequency of the timer
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_clk_freq_get(ALT_CLK_MPU_PERIPH, &freq);
        printf("INFO: Frequency = %" PRIu32 ".\n", freq);
    }

    /*
     * Set the counter of the timer, which determines how frequently it fires.
    */

    if (status == ALT_E_SUCCESS)
    {
        uint32_t counter = (freq / 1000) * period_in_ms;

        status = alt_gpt_counter_set(timer, counter);
        printf("INFO: Period    = %" PRIu32 " millisecond(s).\n", period_in_ms);
        printf("INFO: Counter   = %" PRIu32 ".\n", counter);
    }

    /*
     * Set to periodic, meaning it fires repeatedly.
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_gpt_mode_set(timer, ALT_GPT_RESTART_MODE_PERIODIC);
    }
    
    /*
     * Set the prescaler of the timer to 0.
    */
    if (status == ALT_E_SUCCESS)
    {
        status = alt_gpt_prescaler_set(timer, 0);
    }

    /*
     * Clear pending interrupts
    */

    if (status == ALT_E_SUCCESS)
    {
        if (alt_gpt_int_if_pending_clear(timer) == ALT_E_BAD_ARG)
        {
            status = ALT_E_BAD_ARG;
        }
    }

    /*
     * Enable timer interrupts
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_gpt_int_enable(timer);
    }

    /*
     * Start the timer.
    */

    if (status == ALT_E_SUCCESS)
    {
        status = alt_gpt_tmr_start(timer);
    }

    return status;
}

static inline void socfpga_timer_stop(const ALT_GPT_TIMER_t timer)
{
    alt_gpt_tmr_stop(timer);

    alt_gpt_int_disable(timer);
}


#endif //__TIMER_H
