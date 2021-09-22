// use the dsk board support library to hook up external interrupts, read DIP switches and control LEDs
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"
//-------------------------------------------------------------------

extern far void vectors();         //external function
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void foreverLoop(void);
void init(void);
void main(void);
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void main(void)
{
init();
foreverLoop();
} // main
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void foreverLoop(void)
{
for(;;) // ever
  {
  if(DSK6713_DIP_get(0)==0)         // SW0 is down
    DSK6713_LED_off(0);

  if(DSK6713_DIP_get(1)==0)         // SW1 is down
    DSK6713_LED_off(1);

  if(DSK6713_DIP_get(2)==0)         // SW2 is down
    DSK6713_LED_off(2);

  if(DSK6713_DIP_get(3)==0)         // SW3 is down
    DSK6713_LED_off(3);
  } // for(ever)
} // foreverLoop
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void init(void)
{
DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
CSL_init();
DSK6713_LED_init();  				//init LEDs
DSK6713_DIP_init();                 //init DIP SWs

IRQ_globalDisable();           	    //disable interrupts

IRQ_setVecs(vectors);     			//point to the IRQ vector table

IRQ_map(IRQ_EVT_EXTINT4, 4);		//map McBSP1 external 4 to INT4
IRQ_map(IRQ_EVT_EXTINT5, 5);		//map McBSP1 external 5 to INT5
IRQ_map(IRQ_EVT_EXTINT6, 6);		//map McBSP1 external 6 to INT6
IRQ_map(IRQ_EVT_EXTINT7, 7);		//map McBSP1 external 7 to INT7
                                    // these can be remapped at will

IRQ_reset(IRQ_EVT_EXTINT4);    		//reset  INT 4
IRQ_reset(IRQ_EVT_EXTINT5);    		//reset  INT 5
IRQ_reset(IRQ_EVT_EXTINT6);    		//reset  INT 6
IRQ_reset(IRQ_EVT_EXTINT7);    		//reset  INT 7

IRQ_globalEnable();       			//globally enable interrupts

IRQ_nmiEnable();          			//enable NMI interrupt

IRQ_enable(IRQ_EVT_EXTINT4);		//enable  INT4
IRQ_enable(IRQ_EVT_EXTINT5);		//enable  INT5
IRQ_enable(IRQ_EVT_EXTINT6);		//enable  INT6
IRQ_enable(IRQ_EVT_EXTINT7);		//enable  INT7
	
} // init
//-------------------------------------------------------------------
 
 
//-------------------------------------------------------------------
interrupt void c_int4()
{
DSK6713_LED_on(0);
} // c_int4
//-------------------------------------------------------------------


//-------------------------------------------------------------------
interrupt void c_int5()
{
DSK6713_LED_on(1);
} // c_int5
//-------------------------------------------------------------------


//-------------------------------------------------------------------
interrupt void c_int6()
{
DSK6713_LED_on(2);
} // c_int6
//-------------------------------------------------------------------


//-------------------------------------------------------------------
interrupt void c_int7()
{
DSK6713_LED_on(3);
} // c_int7
//-------------------------------------------------------------------
 

