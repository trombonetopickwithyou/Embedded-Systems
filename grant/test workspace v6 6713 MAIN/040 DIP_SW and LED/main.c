/*
 * main.c
 */

//---------------------------------------------------------------------------------------------
#include <stdio.h>
#include "DSK6713.h"
#include "DSK6713_dip.h"
#include "DSK6713_led.h"
#include "csl.h"
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
void main(void)
{
int i;

volatile char *CPLD_REG0_ADDR = (volatile char *) DSK6713_CPLD_BASE; // this is the syntax to go direct and get 8 bits at that address
volatile char CPLD_REG0_COPY;					// this one is volatile to stop the compiler warning - less efficient when optimized

	DSK6713_init();								// option include paths to BSL header, CSL header
	CSL_init();									// CSL is used in BSL, but BSL does not initialize CSL. You MUST do this to use BSL
	DSK6713_LED_init();
	DSK6713_DIP_init();
												// option link DSK6713.lib
												// optioned predefined CHIP_6713
	i = DSK6713_getVersion();					// this is not necessary for functionality. It just tests BSL library.
	printf("Library version %d\n", i);			// option to printf suport. Not necessary for functionality. It just tests printf()

	for(;;)
	{
		CPLD_REG0_COPY = *CPLD_REG0_ADDR;		// this is the syntax to go direct and get 8 bits at that address

		for(i=0;i<4;i++)
		{
			if(DSK6713_DIP_get(i))				// look at one DIP SWITCH
			{
				DSK6713_LED_on(i);				// ONE LED follows the DIP SWITCH
			}
			else // !DSK6713_DIP_get(i)
			{
				DSK6713_LED_off(i);
			}
		} // for(i)
	} // for(ever)
} // main
//---------------------------------------------------------------------------------------------
