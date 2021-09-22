/*
 * main.c
 */

//-----------------------------------------------------------------------
#include "idlecfg.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"
#include <swi.h>
#include <sem.h>
#include <tsk.h>
#include <gbl.h>

//-----------------------------------------------------------------------
void init(void);
void initGlo(void);
//-----------------------------------------------------------------------

typedef struct _Globals
{
	unsigned int a_five_fence;
	SEM_Obj *SEM0;
	int universal_counter;
	int idle_counter;
	TSK_Obj *TSK0;
	unsigned int five_a_fence;
} Globals;

//-----------------------------------------------------------------------
Globals Glo;

//-----------------------------------------------------------------------
void initGlo(void)						// also launched before main to clear up idle tests in TCF file
{										// no BIOS calls allowed
	Glo.a_five_fence = 0xa5a5a5a5;
	Glo.five_a_fence = 0x5a5a5a5a;
	Glo.SEM0 = &SEM0;					// lets you look at SEM contents in debugger

	Glo.TSK0 = &TSK0;					// lets you look at TSK contents in debugger
	Glo.universal_counter = 0;
	Glo.idle_counter = 0;
	GBL_setPLLto225MHz();				// this is the original BIOS zinit function
} // void initGlo(void);
//-----------------------------------------------------------------------


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

} // init
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
int BIST(void)
{

if(Glo.a_five_fence != 0xa5a5a5a5)
	return 1;
if(Glo.five_a_fence != 0x5a5a5a5a)
	return 2;
if(Glo.universal_counter < 0)
	return 4;
if(Glo.SEM0 != &SEM0)
	return 3;
if(Glo.TSK0 != &TSK0)
	return 3;
return 0;
} // BIST
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void PRD0_ftn(int arg0, int arg1)
{
	SEM_postBinary(&SEM0);
} // PRD0_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void TSK0_ftn(int arg0, int arg1)
{
	for(;;)
	  {
	  	SEM_pendBinary(&SEM0, SYS_FOREVER);
		Glo.universal_counter += 1000000000;
	  }
}// TSK0_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void IDL0_ftn(void)						// launches before main, surprisingly.
{
	int status;
	Glo.idle_counter++;
	status = BIST();

} // IDL0_ftn
