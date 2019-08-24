#include <Arduino.h>
#include "nrf_timer.h"
#include "Timer.h"

static void defaultFunc() {};
static funcPtr_t Timer_callbackPtr;


TimerClass::TimerClass(int timer, int channel) {
    nrf_timer = nrf_timers[timer];
    cc_channel = nrf_timer_cc_channel_t(channel);

    Timer_callbackPtr = defaultFunc;
    Timers[timer] = this;

    // Timer mode with 32bit width
    nrf_timer_bit_width_set(nrf_timer, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_mode_set(nrf_timer, NRF_TIMER_MODE_TIMER);
    nrf_timer_frequency_set(nrf_timer, NRF_TIMER_FREQ_16MHz);
}


void TimerClass::attachInterrupt(funcPtr_t callback, int microsec) {
    // This function will be called when time out interrupt will occur
    if (callback) {
        Timer_callbackPtr = callback;
    } else {
        Serial.println("ERROR: callback function pointer is null.");
        return;
    }

    // Start if not already running (and reset?)
    nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_START);
    nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_CLEAR);

    // Clear and enable compare interrupt
    nrf_timer_int_mask_t chanel_mask = nrf_timer_compare_int_get(cc_channel);
    nrf_timer_int_enable(nrf_timer, chanel_mask);

    if (nrf_timer == nrf_timers[1]) NVIC_EnableIRQ(TIMER1_IRQn);
    if (nrf_timer == nrf_timers[2]) NVIC_EnableIRQ(TIMER2_IRQn);
    if (nrf_timer == nrf_timers[3]) NVIC_EnableIRQ(TIMER3_IRQn);
    if (nrf_timer == nrf_timers[4]) NVIC_EnableIRQ(TIMER4_IRQn);

    // Program compare register between 0 & 2**28 (max on 32 bits @16 ticks/us)
    if (microsec < 0)           microsec = 0;
    if (microsec >= 1 << 28)    microsec = (1 << 28) - 1;
    uint32_t ticks = nrf_timer_us_to_ticks(microsec, NRF_TIMER_FREQ_16MHz);
    nrf_timer_cc_write(nrf_timer, cc_channel, ticks);
}


// Should be called in the Timer_callbackPtr() function
inline void TimerClass::detachInterrupt() {
    // Stop timer
    nrf_timer_task_trigger(nrf_timer, NRF_TIMER_TASK_STOP);

    // Disable timer compare interrupt
    nrf_timer_int_mask_t chanel_mask = nrf_timer_compare_int_get(cc_channel);
    nrf_timer_int_disable(nrf_timer, chanel_mask);

    // Clear event - TODO?
    nrf_timer_event_t event = nrf_timer_compare_event_get(chanel_mask);
    nrf_timer_event_clear(nrf_timer, event);
}


// Timer 0 is used by the soft device but Timer 1, 2, 3 and 4 are available
extern "C" void TIMER1_IRQHandler(void) {
    if (Timers[1]) {
        Timers[1]->detachInterrupt();
        Timer_callbackPtr();
    }
}

extern "C" void TIMER2_IRQHandler(void) {
    if (Timers[2]) {
        Timers[2]->detachInterrupt();
        Timer_callbackPtr();
    }
}

extern "C" void TIMER3_IRQHandler(void) {
    if (Timers[3]) {
        Timers[3]->detachInterrupt();
        Timer_callbackPtr();
    }
}

extern "C" void TIMER4_IRQHandler(void) {
    if (Timers[4]) {
        Timers[4]->detachInterrupt();
        Timer_callbackPtr();
    }
}


NRF_TIMER_Type* nrf_timers[] = {NRF_TIMER0, NRF_TIMER1, NRF_TIMER2,
                                NRF_TIMER3, NRF_TIMER4};

TimerClass* Timers[5] = {0};

