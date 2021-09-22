/* ECE 5380 Homework 2
 * Authors: Joshua Williams
 * 			Connor Collins
 *
 * Adaptations: "070 AUDIO" Dr. Nutter, Texas Tech University
 *
 * Project Description: This program interfaces with the
 * Spectrum Digital TMS320C6713 DSK Board, and the switches SW1,
 * to control the volume levels on the input audio.
 *
 */



//-------------------------------------------------------------------

#include "dsk6713.h"
#include "dsk6713_dip.h"
//#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file

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

union {
	Uint32 uint;
	short channel[2];
	} AIC_data;

//---------------------------------------------------------------------------------------------

#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data

//-------------------------------------------------------------------
void main(void);
void output_sample(int out_data);
Uint32 input_sample(void);

unsigned int buff[16];
//int j=0;						// universal counter of samples

//-------------------------------------------------------------------

//-------------------------------------------------------------------
void main(void)
{
	volatile int i=0;		//local counter for timing when to check the switches (used in place of an interrupt)
	volatile Uint16 sw_num = 0;	//0-15 decimal

	CSL_init();				//initialize chip support library
	DSK6713_init();			//initialize dsk
	DSK6713_DIP_init(); 	//initialize switches

	hCodec = DSK6713_AIC23_openCodec(0, &config1);			// defaults to 16-bit frame.

//	* (volatile int *) 0x0190000C = 0x000000A0;				// prefer a double 16-bit frame
//	* (volatile int *) 0x01900010 = 0x000000A0;

	
	for(;;) // ever
	{
		//when "i" (masked to 4 bits) overflows, check the switches
		//this should probably just be in an interrupt...
		if(i==0)
		{
			//get the value off the switches
			//this is probably not an efficient way to do this...
			sw_num = 8*DSK6713_DIP_get(3) + 4*DSK6713_DIP_get(2) + 2*DSK6713_DIP_get(1) + DSK6713_DIP_get(0);
			
			//Set the codec output gain based on switches
			if (sw_num == 0)
				DSK6713_AIC23_mute(hCodec, 1);
			else
			{
				DSK6713_AIC23_mute(hCodec, 0);
				DSK6713_AIC23_outGain(hCodec, 2*sw_num+90);	//max val = 2*15 + 90 = 120, actual max = 127
			}
		}
		

		//buff[i]= input_sample();
		//output_sample(buff[i]);
		i++;
		//j++;
		i &= 0xF;

		output_sample(input_sample());
	} // for(ever)

} // void main(void)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void output_sample(int out_data)    //for out to Left and Right channels
{

	AIC_data.uint=out_data;          //32-bit data -->data structure

    while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE));//if ready to transmit

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.uint);//write/output data

} // void output_sample(int out_data)
//-------------------------------------------------------------------


//-------------------------------------------------------------------
Uint32 input_sample(void)                      	  	//for 32-bit input
{
	while(!MCBSP_rrdy(DSK6713_AIC23_DATAHANDLE));//if ready to receive

	AIC_data.uint=MCBSP_read(DSK6713_AIC23_DATAHANDLE);//read data

	return(AIC_data.uint);
} // Uint32 input_sample(void)
//-------------------------------------------------------------------
