//Sine_led_ctrl.c Sine generation with DIP Switch control

#define _MAIN

//-------------------------------------------------------------------
#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file
#include "csl.h"
#include "piano.h"
//-------------------------------------------------------------------



/* Codec configuration settings */
DSK6713_AIC23_Config config1 = { 
	0x0017 , /* 0 Left line input channel volume */
	0x0017 , /* 1 Right line input channel volume */
	0x00F9 , /* 2 Left channel headphone volume */
	0x00F9 , /* 3 Right channel headphone volume */
	0x0011 , /* 4 Analog audio path control */
	0x0000 , /* 5 Digital audio path control */
	0x0000 , /* 6 Power down control */
	0x0043 , /* 7 Digital audio interface format */
	0x0081 , /* 8 Sample rate control (48 kHz) */
	0x0001 /* 9 Digital interface activation */
};



//-------------------------------------------------------------------
void main(void)
{
initialize();

MakeMusic(Glo.sine_table, TABLESIZE);
} // main()
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void initialize (void)
{

	CSL_init();
	DSK6713_init();
	DSK6713_LED_init();  								//init LEDs
	DSK6713_DIP_init();                         	//init DIP SWs



	InitializeTable(Glo.sine_table, TABLESIZE);

	Glo.hCodec = DSK6713_AIC23_openCodec(0, &config1);				// plays both L and R


//	* (volatile int *) 0x0190000C = 0x000000A0;						//
//	* (volatile int *) 0x01900010 = 0x000000A0;						// uncomment and only plays only R (zeros L)

} // void initialize (void);
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void output_sample(int out_data)    //for out to Left and Right channels
{

    while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE));//if ready to transmit

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, out_data);//write/output data

} // void output_sample(int out_data)
//-------------------------------------------------------------------


