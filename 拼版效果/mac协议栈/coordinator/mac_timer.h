#ifndef MAC_TIMER_H
#define MAC_TIMER_H

#include "top.h"

//-------------------------------------------------------------------------------------------------------
// Callback table
#define MAC_TIMER_COUNT 12

volatile uint32_t BeaconRxTime;//last beacon accpet time
//-------------------------------------------------------------------------------------------------------
// Function prototypes

// Callback timer setup (timeout in backoff slots (rounded downwards)
void msupInitCallbackTimer(void);
bool msupSetCallbackTimer(void (*pFunc)(), uint32_t  timeout);
bool msupCancelCallbackTimer(void (*pFunc)());

// Wait for the next backoff slot boandary (global interrupts must be turned off when the timer wraps!)
#define WAIT_FOR_BOUNDARY() while (!(TIFR & BM(OCF1A)))

// Callback timer adjustments (align this node's timer with the last received beacon)
void msupTimerBeaconAlignment(void);

// Idle looping (timeout in symbols)
void msupWait(uint16_t  timeout);

// Clock/time stamp functions
uint32_t  msupGetTime(void);
uint32_t  msupGetBeaconRxTime(void);
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Backoff slot counter
extern volatile uint32_t  macBosCounter;
//-------------------------------------------------------------------------------------------------------

#endif

