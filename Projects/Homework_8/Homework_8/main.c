//ECE 5380 - Embedded Systems
//HW8
//11/25/2019
//Author: Joshua Williams
//Co-author: Connor Collins
//
//Description: Using the PRD to read the dip switches (once a second), this code uses SWIs and the EDMA
//				to process audio. The SWIs process the convolution for the LP, BP, and HP filters.

//Issues: A small "crackle" occurs only in the left ear. This is most likely due to small buffer sizes,
//			and/or the lack of a third buffer.
//
//References: Example source code provided by Dr. Brian Nutter, Texas Tech University
//
//http://www.mosaic-industries.com/embedded-systems/c-ide-software-development/multitasking-real-time-operating-system-rtos/c-programming

//-----------------------------------------------------------------------
#include "josh_semcfg.h"
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file

#include "csl_edma.h"
#include "app_codec.h"

#include "filter_coefs.h"						//arrays of rounded filter coefficients, shifted up 16 bits
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#define BUFLEN 0x80				  // EDMA config numbers are changed to accommodate change in BUFLEN
#define BUFWIDTH 2
#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#pragma DATA_SECTION (rx_ping, ".sysdata");
#pragma DATA_ALIGN   (rx_ping, 0x80);
short rx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (rx_pong, ".sysdata");
#pragma DATA_ALIGN   (rx_pong, 0x80);
short rx_pong[BUFWIDTH][BUFLEN];
/*
#pragma DATA_SECTION (rx_pang, ".sysdata");
#pragma DATA_ALIGN   (rx_pang, 0x80);
short rx_pang[BUFWIDTH][BUFLEN];
*/
#pragma DATA_SECTION (tx_ping, ".sysdata");
#pragma DATA_ALIGN   (tx_ping, 0x80);
short tx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_pong, ".sysdata");
#pragma DATA_ALIGN   (tx_pong, 0x80);
short tx_pong[BUFWIDTH][BUFLEN];
/*
#pragma DATA_SECTION (tx_pang, ".sysdata");
#pragma DATA_ALIGN   (tx_pang, 0x80);
short tx_pang[BUFWIDTH][BUFLEN];
*/
//-----------------------------------------------------------------------

//-------------------------------------------------------------------
// declare the CSL objects 
EDMA_Handle hEdma_rx;     // Handle for the EDMA channel                 
EDMA_Handle hEdmaPing_rx; // Handle for the ping EDMA reload parameters  
EDMA_Handle hEdmaPong_rx; // Handle for the pong EDMA reload parameters
//EDMA_Handle hEdmaPang_rx; // Handle for the pong EDMA reload parameters
EDMA_Config cfgEdma_rx;   //EDMA configuration structure  

EDMA_Handle hEdma_tx;
EDMA_Handle hEdmaPing_tx;
EDMA_Handle hEdmaPong_tx;
//EDMA_Handle hEdmaPang_tx;
EDMA_Config cfgEdma_tx;

#include "app_edma.h"      // structures are filled in the header
//-------------------------------------------------------------------

//-----------------------------------------------------------------------
void init(void);
void main(void);
void output_sample(int out_data);
Uint32 input_sample(void);
void conv(int lp, int bp, int hp, short rx_current[BUFWIDTH][BUFLEN], short rx_past[BUFWIDTH][BUFLEN], short tx_current[BUFWIDTH][BUFLEN]);
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

typedef struct _Globals
{
	int lpf;
	int bpf;
	int hpf;
	int bypass;

} Globals;

Globals Glo;

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void main(void)
{
	init();

	//DSP/BIOS idle loop should happen

	return;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void init(void)
{
	int i;
    //LOG_printf(&LOG0, "M 0000");
	for(i=0; i<BUFLEN; i++)
	  { 
		rx_ping[RIGHT][i] = 0;
		rx_ping[LEFT][i] = 0xffff;
		rx_pong[RIGHT][i] = 0;
		rx_pong[LEFT][i] = 0xffff;
		//rx_pang[RIGHT][i] = 0;
		//rx_pang[LEFT][i] = 0xffff;
		tx_ping[RIGHT][i] = 0;
		tx_ping[LEFT][i] = 0xffff;
		tx_pong[RIGHT][i] = 0;
		tx_pong[LEFT][i] = 0xffff;
		//tx_pang[RIGHT][i] = 0;
		//tx_pang[LEFT][i] = 0xffff;
	  }

	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();

   	hCodec = DSK6713_AIC23_openCodec(0, &config1);	// codec has to be open before handle is referenced
                                                                       
    EDMA_clearPram(0x00000000);
                                        
    hEdma_rx = EDMA_open(EDMA_CHA_REVT1, EDMA_OPEN_RESET);
    hEdmaPing_rx = EDMA_allocTable(-1);
    hEdmaPong_rx = EDMA_allocTable(-1);
    //hEdmaPang_rx = EDMA_allocTable(-1);

    cfgEdmaPing_rx.rld = EDMA_RLD_RMK(2,hEdmaPong_rx);
    cfgEdmaPong_rx.rld = EDMA_RLD_RMK(2,hEdmaPing_rx);
    //cfgEdmaPang_rx.rld = EDMA_RLD_RMK(2,hEdmaPing_rx);

    cfgEdma_rx = cfgEdmaPing_rx;        // start with ping
        
    EDMA_config(hEdma_rx, &cfgEdma_rx); 
    EDMA_config(hEdmaPing_rx, &cfgEdmaPing_rx); 
    EDMA_config(hEdmaPong_rx, &cfgEdmaPong_rx); 
    //EDMA_config(hEdmaPang_rx, &cfgEdmaPang_rx);

    hEdma_tx = EDMA_open(EDMA_CHA_XEVT1, EDMA_OPEN_RESET);
    hEdmaPing_tx = EDMA_allocTable(-1);
    hEdmaPong_tx = EDMA_allocTable(-1);
    //hEdmaPang_tx = EDMA_allocTable(-1);

    cfgEdmaPing_tx.rld = EDMA_RLD_RMK(2,hEdmaPong_tx);
    cfgEdmaPong_tx.rld = EDMA_RLD_RMK(2,hEdmaPing_tx);
    //cfgEdmaPang_tx.rld = EDMA_RLD_RMK(2,hEdmaPing_tx);

    cfgEdma_tx = cfgEdmaPing_tx;        // start with ping
        
    EDMA_config(hEdma_tx, &cfgEdma_tx); 
    EDMA_config(hEdmaPing_tx, &cfgEdmaPing_tx); 
    EDMA_config(hEdmaPong_tx, &cfgEdmaPong_tx); 
    //EDMA_config(hEdmaPang_tx, &cfgEdmaPang_tx);

    EDMA_intDisable(0xF);               // use 4 EDMA events
    EDMA_intClear(0xF);                 // on one EDMA interrupt
    EDMA_intEnable(0xF);

    EDMA_intDisable(0xE);
    EDMA_intClear(0xE);  
    EDMA_intEnable(0xE);

    EDMA_intDisable(0xD);
    EDMA_intClear(0xD);  
    EDMA_intEnable(0xD);

    EDMA_intDisable(0xC);
    EDMA_intClear(0xC);  
    EDMA_intEnable(0xC);

    /*
    EDMA_intDisable(0xB);
	EDMA_intClear(0xB);
	EDMA_intEnable(0xB);

	EDMA_intDisable(0xA);
	EDMA_intClear(0xA);
	EDMA_intEnable(0xA);
	*/

    EDMA_enableChannel(hEdma_rx);       // start on the copy of ping

    EDMA_enableChannel(hEdma_tx);       // start on the copy of ping
                                        
    IRQ_reset(IRQ_EVT_EDMAINT);
    IRQ_enable(IRQ_EVT_EDMAINT);        // c_int8
    
  	
  	output_sample(0);                   // start TX to launch EDMA
    //LOG_printf(&LOG0, "M 0001");

	//initialize switches
  	Glo.bypass = DSK6713_DIP_get(0);
	Glo.lpf = DSK6713_DIP_get(1);
	Glo.bpf = DSK6713_DIP_get(2);
	Glo.hpf = DSK6713_DIP_get(3);


} // init
//-----------------------------------------------------------------------

//-------------------------------------------------------------------
void output_sample(int out_data)    //for out to Left and Right channels
{
	AIC_data.uint=out_data;          //32-bit data -->data structure

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.uint);//write/output data
		
} // void output_sample(int out_data)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int c_int8(int arg0)            	// interrupt service routine dispatches this subroutine
{ 
	if(EDMA_intTest(0xF))          	// rx_ping is full
	{
		EDMA_intClear(0xF);
		SWI_post(&SWI_ping);		// rather than setting a flag
	}

	if(EDMA_intTest(0xE))          	// tx_ping has been emptied
	{
		EDMA_intClear(0xE);
	}
	if(EDMA_intTest(0xD))          	// rx_pong is full
	{
		EDMA_intClear(0xD);
		SWI_post(&SWI_pong);		// rather than setting a flag
	}

	if(EDMA_intTest(0xC))          	// tx_pong has been emptied
	{
		EDMA_intClear(0xC);
	}
	/*
	if(EDMA_intTest(0xB))			//rx_pang is full
	{
		EDMA_intClear(0xB);
		SWI_post(&SWI_pang);
	}
	
	if(EDMA_intTest(0xA))			//tx_pang has been emptied
	{
		EDMA_intClear(0xA);
	}
	*/
	return arg0;

} // c_int8
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void conv(int lp, int bp, int hp, short rx_current[BUFWIDTH][BUFLEN], short rx_past[BUFWIDTH][BUFLEN], short tx_current[BUFWIDTH][BUFLEN])
{
	int i = 0;		//left and right
	int j = BUFLEN;	//size of one buffer
	int k = 0;		//for convolution with 81 elements

	long sum = 0;	//40 bits, convolution of shorts (with saturated add)

	for(i=0; i<BUFWIDTH; i++)	//left + right
	{
		for(j=BUFLEN-1; j>=0; j--)	//one entire buffer size
		{
			sum = 0;
			for(k=0; k<81; k++)		//multiply previous samples by specific coef(s), add up all 81 of these and place in index j in tx_current
			{
				if( (j-k) >= 0 )	//if going back does not require us to access the previous buffer
				{
					if(lp==1)
						sum = _lsadd((rx_current[i][j-k] * lp_fil[80-k]),sum);
					if(bp==1)
						sum = _lsadd((rx_current[i][j-k] * bp_fil[80-k]),sum);
					if(hp==1)
						sum = _lsadd((rx_current[i][j-k] * hp_fil[80-k]),sum);
				}
				else 				//look at the end of the previous buffer, going back by 1 more with each loop (because j--)
				{
					if(lp==1)
						sum = _lsadd((rx_past[i][BUFLEN + (j-k)] * lp_fil[80-k]),sum);
					if(bp==1)
						sum = _lsadd((rx_past[i][BUFLEN + (j-k)] * bp_fil[80-k]),sum);
					if(hp==1)
						sum = _lsadd((rx_past[i][BUFLEN + (j-k)] * hp_fil[80-k]),sum);
				}

			}

			//take final convolution for this index and put in transmit buffer
			tx_current[i][j] = sum >> 16; 	//filter coefs were shifted up, so shift back down
		}
	}

	return;
}

//-------------------------------------------------------------------

//-------------------------------------------------------------------
void PRD0_dips_ftn(int arg0, int arg1)
{
	//initialize switches
	Glo.bypass = DSK6713_DIP_get(0);

	//if(Glo.bypass == 0)	//if we aren't bypassing audio, read the filtering switches
	//{
		Glo.lpf = DSK6713_DIP_get(1);
		Glo.bpf = DSK6713_DIP_get(2);
		Glo.hpf = DSK6713_DIP_get(3);
	//}
}
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int Volume=1;
int SWI_ping_ftn(int arg0, int arg1)
{
	if(Glo.bypass == 0)
	{
		//conv(int lp, int bp, int hp, short rx_current[BUFWIDTH][BUFLEN], short rx_past[BUFWIDTH][BUFLEN], short tx_current[BUFWIDTH][BUFLEN])
		conv(Glo.lpf, Glo.bpf, Glo.hpf, rx_ping, rx_pong, tx_ping);
	}
	else
	{
		int i;
		for(i=0;i<BUFLEN;i++)
		  {
			tx_ping[RIGHT][i] = Volume * rx_ping[RIGHT][i];
			tx_ping[LEFT][i] = Volume * rx_ping[LEFT][i];
		  }
	}
	return arg0;
} // int SWI_ping_ftn(int arg0, int arg1)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int SWI_pong_ftn(int arg0, int arg1)
{
	if(Glo.bypass == 0)
	{
		//conv(int lp, int bp, int hp, short rx_current[BUFWIDTH][BUFLEN], short rx_past[BUFWIDTH][BUFLEN], short tx_current[BUFWIDTH][BUFLEN])
		conv(Glo.lpf, Glo.bpf, Glo.hpf, rx_pong, rx_ping, tx_pong);
	}
	else
	{
		int i;
		for(i=0;i<BUFLEN;i++)
		  {
			tx_pong[RIGHT][i] = Volume * rx_pong[RIGHT][i];
			tx_pong[LEFT][i] = Volume * rx_pong[LEFT][i];
		  }
	}
	return arg0;
} // int SWI_pong_ftn(int arg0, int arg1)
//-------------------------------------------------------------------

/*
//-------------------------------------------------------------------
int SWI_pang_ftn(int arg0, int arg1)
{
	if(Glo.bypass == 0)
	{
		//conv(int lp, int bp, int hp, short rx_current[BUFWIDTH][BUFLEN], short rx_past[BUFWIDTH][BUFLEN], short tx_current[BUFWIDTH][BUFLEN])
		conv(Glo.lpf, Glo.bpf, Glo.hpf, rx_pang, rx_pong, tx_pang);
	}
	else
	{
		int i;
		for(i=0;i<BUFLEN;i++)
		  {
			tx_pang[RIGHT][i] = Volume * rx_pang[RIGHT][i];
			tx_pang[LEFT][i] = Volume * rx_pang[LEFT][i];
		  }
	}
	return arg0;
}
*/
