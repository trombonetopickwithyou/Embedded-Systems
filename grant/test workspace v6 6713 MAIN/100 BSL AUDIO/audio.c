//Sine_led_ctrl.c Sine generation with DIP Switch control

#define _MAIN

//----------------------------------------------------------------------------
#include "piano.h"
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
short sine_table[TABLESIZE+1];                        //sine values with one extraq entry for graphing or interpolation

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

DSK6713_AIC23_CodecHandle hCodec;
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void main(void)
{

	initialize();
	MakeMusic(sine_table, TABLESIZE);
} // main()
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void initialize(void)
{
	CSL_init();
	DSK6713_init();

	DSK6713_LED_init();  								//init LEDs
	DSK6713_DIP_init();                         	//init DIP SWs

	InitializeTable(sine_table, TABLESIZE);

	hCodec = DSK6713_AIC23_openCodec(0, &config1);

	//	* (volatile int *) 0x0190000C = 0x000000A0;						//
	//	* (volatile int *) 0x01900010 = 0x000000A0;						// uncomment and only plays only R (zeros L)

} // void initialize(void)
//----------------------------------------------------------------------------


// EOF

