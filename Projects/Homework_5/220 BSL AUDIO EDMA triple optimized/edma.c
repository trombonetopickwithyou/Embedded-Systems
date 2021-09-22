//ECE 5380 - Embedded Systems
//
//HW5
//Authors: Joshua Williams, Connor Collins
//
//Project Description: (without the use of floating point variables)
//		Implement a 3-band equilizer composed of a low-pass, band-pass,
//		and high-pass filter controlled by the dip switches.
//
//(up on switches = enabled)
//sw0: Bypass audio as is
//sw1: Low-pass
//sw2: Band-pass
//sw3: High-pass
//
//a combination of all three filter switches will pass all audio,
//turning off all three filter switches will pass no audio
//
//REFERENCES:	"220 BSL AUDIO EDMA triple optimized" Dr. Nutter, Texas Tech University
//				https://stackoverflow.com/questions/10067510/fixed-point-arithmetic-in-c-programming
//
//-------------------------------------------------------------------

#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file
#include "csl_edma.h"
#include "app_codec.h"
#include "filter_coefs.h"								//arrays of (modified) filter coefficients; all values shifted up by 16 bits

//-------------------------------------------------------------------


//-------------------------------------------------------------------
#define BUFLEN 0x80				// if you change this number, you have to go to the EDMA parameters
								// and change the number of reloads, the skip forward, and the skip backward
#define BUFWIDTH 2
#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data

#pragma DATA_SECTION (rx_ping, ".buffers");
#pragma DATA_ALIGN   (rx_ping, 0x80);
short rx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (rx_pong, ".buffers");
#pragma DATA_ALIGN   (rx_pong, 0x80);
short rx_pong[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (rx_pang, ".buffers");
#pragma DATA_ALIGN   (rx_pang, 0x80);
short rx_pang[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_ping, ".buffers");
#pragma DATA_ALIGN   (tx_ping, 0x80);
short tx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_pong, ".buffers");
#pragma DATA_ALIGN   (tx_pong, 0x80);
short tx_pong[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_pang, ".buffers");
#pragma DATA_ALIGN   (tx_pang, 0x80);
short tx_pang[BUFWIDTH][BUFLEN];

//-------------------------------------------------------------------
typedef struct _Globals
{
	volatile int move_ping;
	volatile int move_pong;
	volatile int move_pang;
} Globals;

Globals Glo = {FALSE, FALSE, FALSE};

//-------------------------------------------------------------------

//-------------------------------------------------------------------

// declare the CSL objects 
EDMA_Handle hEdma_rx;     // Handle for the EDMA channel                 
EDMA_Handle hEdmaPing_rx; // Handle for the ping EDMA reload parameters  
EDMA_Handle hEdmaPong_rx; // Handle for the pong EDMA reload parameters  
EDMA_Handle hEdmaPang_rx; // Handle for the pong EDMA reload parameters
EDMA_Config cfgEdma_rx;   //EDMA configuration structure  

EDMA_Handle hEdma_tx;
EDMA_Handle hEdmaPing_tx;
EDMA_Handle hEdmaPong_tx;              
EDMA_Handle hEdmaPang_tx;
EDMA_Config cfgEdma_tx;

#include "app_edma.h"      // structures are filled in the header


//-------------------------------------------------------------------
void main(void);
void forever_loop(void);
void output_sample(int out_data);
Uint32 input_sample(void);
void init(void);
void MoveData(short tx[BUFWIDTH][BUFLEN], short rx[BUFWIDTH][BUFLEN]);
void conv(int lp, int bp, int hp, short rx_current[BUFWIDTH][BUFLEN], short rx_past[BUFWIDTH][BUFLEN], short tx_current[BUFWIDTH][BUFLEN]);
//-------------------------------------------------------------------


//-------------------------------------------------------------------
extern far void vectors();         //external function
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void main(void)
{
	init();
	forever_loop();
} // void main(void)
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void forever_loop(void)
{
	for(;;)
	  {
	  	if(Glo.move_ping == TRUE)
	  	  {
	  		Glo.move_ping = FALSE;

	  		if(DSK6713_DIP_get(0) == 1)	//sw0 just bypasses all filtering
	  			MoveData(tx_ping, rx_ping);
	  		else
	  		{
	  			//other three switches determine which filters are active
	  			conv(DSK6713_DIP_get(1), DSK6713_DIP_get(2), DSK6713_DIP_get(3), rx_ping, rx_pang, tx_ping);
	  		}

	  	  }
	  	if(Glo.move_pong == TRUE)
	  	  {
	  		Glo.move_pong = FALSE;

	  		if(DSK6713_DIP_get(0) == 1)
	  			MoveData(tx_pong, rx_pong);
	  		else
	  		{
	  			conv(DSK6713_DIP_get(1), DSK6713_DIP_get(2), DSK6713_DIP_get(3), rx_pong, rx_ping, tx_pong);
	  		}

	  	  }
	  	if(Glo.move_pang == TRUE)
	  	  {
	  		Glo.move_pang = FALSE;

	  		if(DSK6713_DIP_get(0) == 1)
	  			MoveData(tx_pang, rx_pang);
	  		else
	  		{
	  			conv(DSK6713_DIP_get(1), DSK6713_DIP_get(2), DSK6713_DIP_get(3), rx_pang, rx_pong, tx_pang);
	  		}
	  	  }
	  } // ever
		 
	
} // void forever_loop(void)
//-------------------------------------------------------------------


//-------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(MoveData, "-o3")
//-------------------------------------------------------------------
void MoveData(short tx[BUFWIDTH][BUFLEN], short rx[BUFWIDTH][BUFLEN])
{
	int i;

	  	for(i=0;i<BUFLEN;i++)
	  	  {
	  	  	tx[RIGHT][i] = rx[RIGHT][i];
	  	  	tx[LEFT][i] = rx[LEFT][i];
	  	  }
} // void MoveData(short tx[BUFWIDTH][BUFLEN], short rx[BUFWIDTH][BUFLEN]);
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
						sum = _lsadd(sum,(long)(rx_current[i][j-k] * lp_fil[80-k]));
					if(bp==1)
						sum = _lsadd(sum,(long)(rx_current[i][j-k] * bp_fil[80-k]));
					if(hp==1)
						sum = _lsadd(sum,(long)(rx_current[i][j-k] * hp_fil[80-k]));
				}
				else 				//look at the end of the previous buffer, going back by 1 more with each loop (because j--)
				{
					if(lp==1)
						sum = _lsadd(sum,(long)(rx_past[i][BUFLEN + (j-k)] * lp_fil[80-k]));
					if(bp==1)
						sum = _lsadd(sum,(long)(rx_past[i][BUFLEN + (j-k)] * bp_fil[80-k]));
					if(hp==1)
						sum = _lsadd(sum,(long)(rx_past[i][BUFLEN + (j-k)] * hp_fil[80-k]));
				}

			}

			//take final convolution for this index and put in transmit buffer
			tx_current[i][j] = sum >> 16; 	//filter coefs were shifted up, so shift back down (truncates down to integers)
		}
	}

	return;
}





//-------------------------------------------------------------------
void output_sample(int out_data)    //for out to Left and Right channels
{

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, out_data);//write/output data
		
} // void output_sample(int out_data)
//-------------------------------------------------------------------


//-------------------------------------------------------------------
Uint32 input_sample(void)                      	  	//for 32-bit input
{

	return MCBSP_read(DSK6713_AIC23_DATAHANDLE);//read data

} // Uint32 input_sample(void)
//-------------------------------------------------------------------



//-------------------------------------------------------------------
interrupt void c_int8()            // interrupt service routine
{ 
	if(EDMA_intTest(0xF))          // rx_ping is full
	{
		EDMA_intClear(0xF);
		Glo.move_ping = TRUE;
	}

	if(EDMA_intTest(0xE))          // tx_ping has been emptied
	{
		EDMA_intClear(0xE);
	}
	if(EDMA_intTest(0xD))          // rx_pong is full
	{
		EDMA_intClear(0xD);
		Glo.move_pong = TRUE;
	}

	if(EDMA_intTest(0xC))          // tx_pong has been emptied
	{
		EDMA_intClear(0xC);
	}
	if(EDMA_intTest(0xB))          // rx_pang is full
	{
		EDMA_intClear(0xB);
		Glo.move_pang = TRUE;
	}

	if(EDMA_intTest(0xA))          // tx_pang has been emptied
	{
		EDMA_intClear(0xA);
	}
	return;

} // c_int8
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void init(void)	     	 	//for communication/init using interrupt
{
	int i;
	for(i=0; i<BUFLEN; i++)
	  { 
	  	rx_ping[RIGHT][i] = 0;
	  	rx_ping[LEFT][i] = 0xffff;
	  	rx_pong[RIGHT][i] = 0;
	  	rx_pong[LEFT][i] = 0xffff;
	  	rx_pang[RIGHT][i] = 0;
	  	rx_pang[LEFT][i] = 0xffff;
	  	tx_ping[RIGHT][i] = 0;
	  	tx_ping[LEFT][i] = 0xffff;
	  	tx_pong[RIGHT][i] = 0;
	  	tx_pong[LEFT][i] = 0xffff;
	  	tx_pang[RIGHT][i] = 0;
	  	tx_pang[LEFT][i] = 0xffff;
	  }
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();
    IRQ_globalDisable();           	    //disable interrupts
   	hCodec = DSK6713_AIC23_openCodec(0, &config1);
                                        // codec has to be open before handle is referenced
   	/* Set codec sampling rate to 8kHz */
   	//DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_8KHZ);
                                        
    EDMA_clearPram(0x00000000);
                                        
    hEdma_rx = EDMA_open(EDMA_CHA_REVT1, EDMA_OPEN_RESET);
    hEdmaPing_rx = EDMA_allocTable(-1);
    hEdmaPong_rx = EDMA_allocTable(-1);
    hEdmaPang_rx = EDMA_allocTable(-1);

    cfgEdmaPing_rx.rld = EDMA_RLD_RMK(2,hEdmaPong_rx);
    cfgEdmaPong_rx.rld = EDMA_RLD_RMK(2,hEdmaPang_rx);
    cfgEdmaPang_rx.rld = EDMA_RLD_RMK(2,hEdmaPing_rx);
                                        
    cfgEdma_rx = cfgEdmaPing_rx;        // start with ping
        
    EDMA_config(hEdma_rx, &cfgEdma_rx); 
    EDMA_config(hEdmaPing_rx, &cfgEdmaPing_rx); 
    EDMA_config(hEdmaPong_rx, &cfgEdmaPong_rx); 
    EDMA_config(hEdmaPang_rx, &cfgEdmaPang_rx);

    hEdma_tx = EDMA_open(EDMA_CHA_XEVT1, EDMA_OPEN_RESET);
    hEdmaPing_tx = EDMA_allocTable(-1);
    hEdmaPong_tx = EDMA_allocTable(-1);
    hEdmaPang_tx = EDMA_allocTable(-1);

    cfgEdmaPing_tx.rld = EDMA_RLD_RMK(2,hEdmaPong_tx);
    cfgEdmaPong_tx.rld = EDMA_RLD_RMK(2,hEdmaPang_tx);
    cfgEdmaPang_tx.rld = EDMA_RLD_RMK(2,hEdmaPing_tx);
                                        
    cfgEdma_tx = cfgEdmaPing_tx;        // start with ping
        
    EDMA_config(hEdma_tx, &cfgEdma_tx); 
    EDMA_config(hEdmaPing_tx, &cfgEdmaPing_tx); 
    EDMA_config(hEdmaPong_tx, &cfgEdmaPong_tx); 
    EDMA_config(hEdmaPang_tx, &cfgEdmaPang_tx);

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

    EDMA_intDisable(0xB);
    EDMA_intClear(0xB);
    EDMA_intEnable(0xB);

    EDMA_intDisable(0xA);
    EDMA_intClear(0xA);
    EDMA_intEnable(0xA);

    EDMA_enableChannel(hEdma_rx);       // start on the copy of ping

    EDMA_enableChannel(hEdma_tx);       // start on the copy of ping
                                        

  	IRQ_setVecs(vectors);     			//point to the IRQ vector table
    IRQ_map(IRQ_EVT_EDMAINT, 8);
    IRQ_reset(IRQ_EVT_EDMAINT);
    IRQ_enable(IRQ_EVT_EDMAINT);        // c_int8
    

    IRQ_globalEnable();       			//globally enable interrupts
  	IRQ_nmiEnable();          			//enable NMI interrupt
  	
  	output_sample(0);                   // start TX to launch EDMA
} // init
//-------------------------------------------------------------------



