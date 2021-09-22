//-----------------------------------------------------------------------

#include <std.h>
#include <log.h>
#include <prd.h>

//-----------------------------------------------------------------------
#include "biostestcfg.h"

#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"

typedef struct _Globals
{
	PRD_Obj *PRD0;

} Globals;

Globals Glo;
//-----------------------------------------------------------------------
void main(void)
{
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();
	DSK6713_LED_init();  				//init LEDs
	DSK6713_DIP_init();                 //init DIP SWs
	
    LOG_printf(&trace, "Point A");
    LOG_printf(&trace, "Message B");
    LOG_printf(&trace, "Condition C");

    Glo.PRD0 = &PRD0;
    /* fall into DSP/BIOS idle loop */

    return;
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void timer_tick(int arg0, int arg1)
{
	
	LOG_printf(&trace, "PRD D");
	DSK6713_LED_toggle(0);
	LOG_printf(&trace, "old %d", PRD0.period);
	PRD0.period *= 2;
	LOG_printf(&trace, "new %d", PRD0.period);
}
//-----------------------------------------------------------------------
