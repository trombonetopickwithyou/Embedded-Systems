//ECE 5380 - Embedded Systems
//HW7
//11/8/2019
//Author: Joshua Williams
//Co-author: Connor Collins
//
//Description: Use the BIOS to create a PRD that controls the frequency LED0 blinks at.
//				If all the switches are 0, stop the PRD until the switches are no longer 0
//
//References: Example source code provided by Dr. Brian Nutter, Texas Tech University
//
//http://www.mosaic-industries.com/embedded-systems/c-ide-software-development/multitasking-real-time-operating-system-rtos/c-programming
//
//-----------------------------------------------------------------------
#include "josh_semcfg.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"
#include <swi.h>
#include <sem.h>
#include <tsk.h>

//-----------------------------------------------------------------------
void init(void);
//-----------------------------------------------------------------------

typedef struct _Globals
{
	int LED_last;

	int SWcurrent;
	int SWpast;

	TSK_Obj *TSK0;

} Globals;

Globals Glo;

//-----------------------------------------------------------------------
void main(void)
{
	init();
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void init(void)
{
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();
	DSK6713_LED_init();  				//init LEDs
	DSK6713_DIP_init();                 //init DIP SWs

	Glo.TSK0 = &TSK0;					//lets you look at TSK contents in debugger

	DSK6713_LED_off(0);					//turn the LED off initially
	Glo.LED_last = 0;					//set the global variable to remember the LED state

	Glo.SWpast = 1;						//set the past SW state to a nonzero number
										//(otherwise, if switches all start at zero, the program will not disable the PRD until a SW change occurs)

} // init
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void PRD0_ftn(int arg0, int arg1)
{
	//Read the switches
	PRD0.period = 500/(DSK6713_DIP_get(0) + 2*DSK6713_DIP_get(1) + 4*DSK6713_DIP_get(2) + 8*DSK6713_DIP_get(3));	//milliseconds

	//toggle the LED
	if(Glo.LED_last == 1)
	{
		DSK6713_LED_off(0);
		Glo.LED_last = 0;
	}
	else
	{
		DSK6713_LED_on(0);
		Glo.LED_last = 1;
	}
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------

//for enabling/disabling the PRD
void TSK0_ftn(int arg0, int arg1)
{
	for(;;)	//ever
	{
		//read switches
		Glo.SWcurrent = DSK6713_DIP_get(0) + 2*DSK6713_DIP_get(1) + 4*DSK6713_DIP_get(2) + 8*DSK6713_DIP_get(3);

		//if the switches have changed
		if(Glo.SWcurrent != Glo.SWpast)
		{
			//if 0 on the switches, stop the PRD
			if(Glo.SWcurrent == 0)
			{
				PRD_stop(&PRD0);
			}
			else
			{
				PRD_start(&PRD0);
			}

			//we've done everything concerning switches, update SWpast variable to current switch state
			Glo.SWpast = Glo.SWcurrent;
		}
	}

}// TSK0_ftn

//-----------------------------------------------------------------------
