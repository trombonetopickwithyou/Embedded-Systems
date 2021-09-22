//-----------------------------------------------------------------------

#include <std.h>
#include <log.h>
#include <prd.h>
#include <hwi.h>
//-----------------------------------------------------------------------
#include "bioshwicfg.h"

#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"

//-----------------------------------------------------------------------
void init(void);
void main(void);
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void main(void)
{
	init();

    /* fall into DSP/BIOS idle loop */
    return;
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void init(void)
{
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();
	DSK6713_LED_init();  				//init LEDs
	DSK6713_DIP_init();                 //init DIP SWs
	
    LOG_printf(&LOG0, "M 0000");

	IRQ_reset(IRQ_EVT_EXTINT4);    		//reset  INT 4
	IRQ_reset(IRQ_EVT_EXTINT5);    		//reset  INT 5
	IRQ_reset(IRQ_EVT_EXTINT6);    		//reset  INT 6
	IRQ_reset(IRQ_EVT_EXTINT7);    		//reset  INT 7

	IRQ_enable(IRQ_EVT_EXTINT4);		//enable  INT4
	IRQ_enable(IRQ_EVT_EXTINT5);		//enable  INT5
	IRQ_enable(IRQ_EVT_EXTINT6);		//enable  INT6
	IRQ_enable(IRQ_EVT_EXTINT7);		//enable  INT7
    
    LOG_printf(&LOG0, "M 0001");

	
} // void init(void)
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void PRD0_tick(int arg0, int arg1)
{
	LOG_printf(&LOG0, "PRD D");
	DSK6713_LED_off(0);
	DSK6713_LED_off(1);
	DSK6713_LED_off(2);
	DSK6713_LED_off(3);
} // 
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
int HWI4567(int arg0)
{
	LOG_printf(&LOG0, "HWI %d", arg0);
	DSK6713_LED_on(arg0);
	return arg0;
} // int HWI4567
//-----------------------------------------------------------------------


