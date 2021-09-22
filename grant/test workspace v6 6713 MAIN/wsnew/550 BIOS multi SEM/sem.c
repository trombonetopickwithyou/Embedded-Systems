//-----------------------------------------------------------------------
#include "semaphorecfg.h"
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
	SEM_Obj *SEM0;
	SEM_Obj *SEM1;
	SEM_Obj *SEM2;
	SEM_Obj *SEM3;
	TSK_Obj *TSK0;
	TSK_Obj *TSK1;
	TSK_Obj *TSK2;
	TSK_Obj *TSK3;
	SWI_Obj *SWI0;

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
	
	Glo.SEM0 = &SEM0;					// lets you look at SEM contents in debugger
	Glo.SEM1 = &SEM1;
	Glo.SEM2 = &SEM2;
	Glo.SEM3 = &SEM3;

	Glo.TSK0 = &TSK0;					// lets you look at TSK contents in debugger
	Glo.TSK1 = &TSK1;
	Glo.TSK2 = &TSK2;
	Glo.TSK3 = &TSK3;

	Glo.SWI0 = &SWI0;					// lets you look at SWI contents in debugger

} // init
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void PRD0_ftn(int arg0, int arg1)
{
	SWI_post(&SWI0);
} // PRD0_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void SWI0_ftn(int arg0, int arg1)
{
	  if(DSK6713_DIP_get(0)==0)         // SW0 is down
	  	DSK6713_LED_off(0);
	  else if(DSK6713_DIP_get(1)==0)         // SW1 is down
	  	DSK6713_LED_off(1);
	  else if(DSK6713_DIP_get(2)==0)         // SW2 is down
	  	DSK6713_LED_off(2);
	  else if(DSK6713_DIP_get(3)==0)         // SW3 is down
	  	DSK6713_LED_off(3);
	  else
	    SEM_postBinary(&SEM0);
	
} // SWI0_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void TSK0_ftn(int arg0, int arg1)
{
	for(;;)
	  {
	  	SEM_pendBinary(&SEM0, SYS_FOREVER);
	  	DSK6713_LED_on(0);
	    SEM_postBinary(&SEM1);
	  }
}// TSK0_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void TSK1_ftn(int arg0, int arg1)
{
	for(;;)
	  {
	  	SEM_pendBinary(&SEM1, SYS_FOREVER);
	  	DSK6713_LED_on(1);
	    SEM_postBinary(&SEM2);
	  }
	
}// TSK1_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void TSK2_ftn(int arg0, int arg1)
{
	for(;;)
	  {
	  	SEM_pendBinary(&SEM2, SYS_FOREVER);
	  	DSK6713_LED_on(2);
	    SEM_postBinary(&SEM3);
	  }
	
}// TSK2_ftn
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void TSK3_ftn(int arg0, int arg1)
{
	for(;;)
	  {
	  	SEM_pendBinary(&SEM3, SYS_FOREVER);
	  	DSK6713_LED_on(3);
	  }
	
} // TSK3_ftn
//-----------------------------------------------------------------------

