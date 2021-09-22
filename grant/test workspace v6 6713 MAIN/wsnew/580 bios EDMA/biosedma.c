//-----------------------------------------------------------------------

#include <std.h>
#include <log.h>
#include <prd.h>
#include <hwi.h>
#include <swi.h>

//-----------------------------------------------------------------------
#include "biosedmacfg.h"
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"
#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file
#include "csl_edma.h"
#include "app_codec.h"
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#define BUFLEN 0x40				  // EDMA config numbers are chaged to accommodate change in BUFLEN
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

#pragma DATA_SECTION (tx_ping, ".sysdata");
#pragma DATA_ALIGN   (tx_ping, 0x80);
short tx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_pong, ".sysdata");
#pragma DATA_ALIGN   (tx_pong, 0x80);
short tx_pong[BUFWIDTH][BUFLEN];
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

//-------------------------------------------------------------------
// declare the CSL objects 
EDMA_Handle hEdma_rx;     // Handle for the EDMA channel                 
EDMA_Handle hEdmaPing_rx; // Handle for the ping EDMA reload parameters  
EDMA_Handle hEdmaPong_rx; // Handle for the pong EDMA reload parameters  
EDMA_Config cfgEdma_rx;   //EDMA configuration structure  

EDMA_Handle hEdma_tx;
EDMA_Handle hEdmaPing_tx;
EDMA_Handle hEdmaPong_tx;              
EDMA_Config cfgEdma_tx;

#include "app_edma.h"      // structures are filled in the header
//-------------------------------------------------------------------

//-----------------------------------------------------------------------
void init(void);
void main(void);
void output_sample(int out_data);
Uint32 input_sample(void);

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void main(void)
{
	init();

    /* fall into DSP/BIOS idle loop */
    return;
} // void main(void)
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void init(void)
{
	int i;
    LOG_printf(&LOG0, "M 0000");
	for(i=0; i<BUFLEN; i++)
	  { 
	  	rx_ping[RIGHT][i] = 0;
	  	rx_ping[LEFT][i] = 0xffff;
	  	rx_pong[RIGHT][i] = 0;
	  	rx_pong[LEFT][i] = 0xffff;
	  	tx_ping[RIGHT][i] = 0;
	  	tx_ping[LEFT][i] = 0xffff;
	  	tx_pong[RIGHT][i] = 0;
	  	tx_pong[LEFT][i] = 0xffff;
	  }
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();

   	hCodec = DSK6713_AIC23_openCodec(0, &config1);
                                        // codec has to be open before handle is referenced
                                        
    EDMA_clearPram(0x00000000);
                                        
    hEdma_rx = EDMA_open(EDMA_CHA_REVT1, EDMA_OPEN_RESET);
    hEdmaPing_rx = EDMA_allocTable(-1);
    hEdmaPong_rx = EDMA_allocTable(-1);

    cfgEdmaPing_rx.rld = EDMA_RLD_RMK(2,hEdmaPong_rx);
    cfgEdmaPong_rx.rld = EDMA_RLD_RMK(2,hEdmaPing_rx);
                                        
    cfgEdma_rx = cfgEdmaPing_rx;        // start with ping
        
    EDMA_config(hEdma_rx, &cfgEdma_rx); 
    EDMA_config(hEdmaPing_rx, &cfgEdmaPing_rx); 
    EDMA_config(hEdmaPong_rx, &cfgEdmaPong_rx); 

    hEdma_tx = EDMA_open(EDMA_CHA_XEVT1, EDMA_OPEN_RESET);
    hEdmaPing_tx = EDMA_allocTable(-1);
    hEdmaPong_tx = EDMA_allocTable(-1);

    cfgEdmaPing_tx.rld = EDMA_RLD_RMK(2,hEdmaPong_tx);
    cfgEdmaPong_tx.rld = EDMA_RLD_RMK(2,hEdmaPing_tx);
                                        
    cfgEdma_tx = cfgEdmaPing_tx;        // start with ping
        
    EDMA_config(hEdma_tx, &cfgEdma_tx); 
    EDMA_config(hEdmaPing_tx, &cfgEdmaPing_tx); 
    EDMA_config(hEdmaPong_tx, &cfgEdmaPong_tx); 

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

    EDMA_enableChannel(hEdma_rx);       // start on the copy of ping

    EDMA_enableChannel(hEdma_tx);       // start on the copy of ping
                                        
    IRQ_reset(IRQ_EVT_EDMAINT);
    IRQ_enable(IRQ_EVT_EDMAINT);        // c_int8
    
  	
  	output_sample(0);                   // start TX to launch EDMA
    LOG_printf(&LOG0, "M 0001");
	
} //void init(void)
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
	
	return arg0;

} // c_int8
//-------------------------------------------------------------------

int Volume=1;

//-------------------------------------------------------------------
int SWI_ping_ftn(int arg0, int arg1)
{
	int i;
  	for(i=0;i<BUFLEN;i++)
 	  {
 	  	tx_ping[RIGHT][i] = Volume * rx_ping[RIGHT][i];
 	  	tx_ping[LEFT][i] = Volume * rx_ping[LEFT][i];
 	  }
	
	return arg0;
} // int SWI_ping_ftn(int arg0, int arg1)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int SWI_pong_ftn(int arg0, int arg1)
{
	int i;
  	for(i=0;i<BUFLEN;i++)
 	  {
 	  	tx_pong[RIGHT][i] = Volume * rx_pong[RIGHT][i];
 	  	tx_pong[LEFT][i] = Volume * rx_pong[LEFT][i];
 	  }
	
	return arg0;
} // int SWI_pong_ftn(int arg0, int arg1)
//-------------------------------------------------------------------




