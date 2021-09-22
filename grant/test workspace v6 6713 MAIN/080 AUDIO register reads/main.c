//-------------------------------------------------------------------
#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file

//-------------------------------------------------------------------
//-------------------------------------------------------------------

//-------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------

#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data

//-------------------------------------------------------------------


//-------------------------------------------------------------------
void main(void);
void output_sample(int out_data);
Uint32 input_sample(void);

//-------------------------------------------------------------------


//-------------------------------------------------------------------
void main(void)
{
	volatile Uint16 regval;				// volatile forces the compiler to save it every time.

	CSL_init();

	DSK6713_init();
	hCodec = DSK6713_AIC23_openCodec(0, &config1);			// defaults to 16-bit frame.

	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_LEFTINVOL);	// 17 now read all the registers to see what got into them
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_RIGHTINVOL);	// 17
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_LEFTHPVOL);	// F9
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_RIGHTHPVOL);	// F9
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_ANAPATH);		// 11
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_DIGPATH);		// 00
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_POWERDOWN);	// 00
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_DIGIF);		// 43
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_SAMPLERATE);	// 81
	regval = DSK6713_AIC23_rget(hCodec, DSK6713_AIC23_DIGACT);		// 01

//	* (volatile Uint16 *) 0x0190000C = 0x00A0;						// prefer a double 16-bit frame
//	* (volatile Uint16 *) 0x01900010 = 0x00A0;

	for(;;) // ever
	{
		output_sample(input_sample());
	} // for(ever)
} // void main(void)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void output_sample(int out_data)    //for out to Left and Right channels
{

    while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE));//if ready to transmit

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, out_data);//write/output data

} // void output_sample(int out_data)
//-------------------------------------------------------------------


//-------------------------------------------------------------------
Uint32 input_sample(void)                      	  	//for 32-bit input
{
	while(!MCBSP_rrdy(DSK6713_AIC23_DATAHANDLE));//if ready to receive

	return(MCBSP_read(DSK6713_AIC23_DATAHANDLE));//read data

} // Uint32 input_sample(void)
//-------------------------------------------------------------------

