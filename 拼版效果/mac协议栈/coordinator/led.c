#include "led.h"

uint8_t LedsC_ledsOn;



//-----------------------------------------------------œ‘ æled«˝?
uint8_t Leds_init(void)

{
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
      LedsC_ledsOn = 0;
      OS_MAKE_RED_LED_OUTPUT();
      OS_MAKE_YELLOW_LED_OUTPUT();
      OS_MAKE_GREEN_LED_OUTPUT();
      OS_SET_RED_LED_PIN();
      OS_SET_YELLOW_LED_PIN();
      OS_SET_GREEN_LED_PIN();
    }

    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}
uint8_t Leds_yellowOn(void)
{
  
  { os_atomic_t _nesc_atomic = os_atomic_start();

    {
      OS_CLR_YELLOW_LED_PIN();
      LedsC_ledsOn |= LedsC_YELLOW_BIT;
    }
    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}
uint8_t Leds_yellowOff(void)

{
    { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      OS_SET_YELLOW_LED_PIN();
      LedsC_ledsOn &= ~LedsC_YELLOW_BIT;
    }
    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}

uint8_t Leds_yellowToggle(void)
{
  uint8_t rval;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      if (LedsC_ledsOn & LedsC_YELLOW_BIT) {
        rval = Leds_yellowOff();
        }
      else {
        rval = Leds_yellowOn();
        }
    }
    os_atomic_end(_nesc_atomic); }
  return rval;
}

uint8_t Leds_greenOn(void)

{
    { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      OS_CLR_GREEN_LED_PIN();
      LedsC_ledsOn |= LedsC_GREEN_BIT;
    }
    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}

uint8_t Leds_greenOff(void)
{
    { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      OS_SET_GREEN_LED_PIN();
      LedsC_ledsOn &= ~LedsC_GREEN_BIT;
    }
    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}
uint8_t Leds_greenToggle(void)

{
  uint8_t rval;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      if (LedsC_ledsOn & LedsC_GREEN_BIT) {
        rval = Leds_greenOff();
        }
      else {
        rval = Leds_greenOn();
        }
    }
    os_atomic_end(_nesc_atomic); }
  return rval;
}

uint8_t Leds_redOn(void)
{
   { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      OS_CLR_RED_LED_PIN();
      LedsC_ledsOn |= LedsC_RED_BIT;
    }
    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}

uint8_t Leds_redOff(void)
{
   { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      OS_SET_RED_LED_PIN();
      LedsC_ledsOn &= ~LedsC_RED_BIT;
    }
    os_atomic_end(_nesc_atomic); }
  return SUCCESS;
}

uint8_t Leds_redToggle(void)
{
  uint8_t rval;
  { os_atomic_t _nesc_atomic = os_atomic_start();
    {
      if (LedsC_ledsOn & LedsC_RED_BIT) {
        rval = Leds_redOff();
        }
      else {
        rval = Leds_redOn();
        }
    }
    os_atomic_end(_nesc_atomic); }
  return rval;
}

