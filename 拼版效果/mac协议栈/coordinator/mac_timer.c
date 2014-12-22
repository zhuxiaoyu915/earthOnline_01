#include "mac_timer.h"

uint32_t  msupGetTime(void)
{
	return (systime32());
}

uint32_t  msupGetBeaconRxTime(void)
{	
	return (BeaconRxTime);
}