#include <stdint.h>
#include "MKL25Z4.h"
#include "wdog.h"
#include "sysinit.h"

#define DISABLE_WDOG    1

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) 
{
wdog_disable();	/* WDOG disable */
sysinit(); /* System init */
}
