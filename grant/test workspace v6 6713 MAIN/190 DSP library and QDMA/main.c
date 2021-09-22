/*
 * main.c
 */

//---------------------------------------------------------------------------------
#include "csl.h"
#include "csl_edma.h"
#include "DSPF_sp_minval.h"
#include "DSPF_sp_maxval.h"

//---------------------------------------------------------------------------------
#define BUFFSIZE 100		// 1000 leads to UnDoneCount = 77
							// 100 -> 3
							// 10 -> 1

//---------------------------------------------------------------------------------

#pragma DATA_SECTION (x, ".far")
#pragma DATA_ALIGN (x, 0x10)
float x[BUFFSIZE] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10.};

#pragma DATA_SECTION (y, ".far")
#pragma DATA_ALIGN (y, 0x10)
float y[BUFFSIZE+10] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};

typedef struct _Globals
{
	volatile int DoneFlag;

} Globals;

Globals Glo;

//---------------------------------------------------------------------------------
void init(void);

//---------------------------------------------------------------------------------
extern far void vectors();         //external function

//---------------------------------------------------------------------------------
	EDMA_Config cfgQdma = {
	  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT     */

	   EDMA_OPT_PRI_LOW,     /* Priority levels for EDMA events:
	                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
	                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer */

	   EDMA_OPT_ESIZE_32BIT, /* Element size :-
	                             EDMA_OPT_ESIZE_32BIT - 32 bit word (float is 32 bits)
	                             EDMA_OPT_ESIZE_16BIT - 16 bit word
	                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */

	   EDMA_OPT_2DS_NO,      /* Source dimension :-
	                             EDMA_OPT_2DS_NO  - 1-dimensional source
	                             EDMA_OPT_2DS_YES - 2-dimensional source         */

	   EDMA_OPT_SUM_INC,     /* Source address update mode :-
	                             EDMA_OPT_SUM_NONE - Fixed address mode
	                             EDMA_OPT_SUM_INC  - Increment address mode
	                             EDMA_OPT_SUM_DEC  - Decrement address mode
	                             EDMA_OPT_SUM_IDX  - Address modified by element
	                                                 index or frame Index        */
	    EDMA_OPT_2DD_NO,     /* Destination dimension :-
	                             EDMA_OPT_2DD_NO   - 1-dimensional source
	                             EDMA_OPT_2DD_YES  - 2-dimensional source        */

	    EDMA_OPT_DUM_INC,    /* Destination address update mode :-
	                             EDMA_OPT_DUM_NONE - Fixed address mode
	                             EDMA_OPT_DUM_INC  - Increment address mode
	                             EDMA_OPT_DUM_DEC  - Decrement address mode
	                             EDMA_OPT_DUM_IDX  - Address modified by element
	                                                 index or frame Index        */

	    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
	                             EDMA_OPT_TCINT_NO  - Indication disabled
	                             EDMA_OPT_TCINT_YES - Indication enabled         */

	    EDMA_OPT_TCC_OF(0x9),/* Transfer complete code                           */

	    EDMA_OPT_LINK_NO,    /* Linking of event parameters
	                             EDMA_OPT_LINK_NO   -  Disabled
	                             EDMA_OPT_LINK_YES  -  Enabled                   */

	    EDMA_OPT_FS_NO       /* Frame synchronization
	                             EDMA_OPT_FS_NO  - Channel is element/array
	                                                synchronized
	                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
	  ),
	  EDMA_SRC_OF(x),        /* Source address register
	                             &x    - source address                          */

	  EDMA_CNT_OF(BUFFSIZE), /* Transfer count parameter
	                             BUFFSIZE      - buffer size in # of floats (int)*/

	  EDMA_DST_OF(y),        /* Destination address parameter
	                             &y - destination address                        */

	  EDMA_IDX_OF(0),        /* Index parameter
	                             N/A for QDMA		                             */

	  EDMA_RLD_OF(0x00)      /* Count reload/link parameter
 							     N/A for QDMA		                             */
	};

//---------------------------------------------------------------------------------
	int main(void)
	{
	float min, max;
	
	int UnDoneCount = 0;

	init();

	EDMA_qdmaConfig(&cfgQdma);		// copy a buffer using qdma. Takes care of itself.

	if(!Glo.DoneFlag)
		UnDoneCount++;

	min = DSPF_sp_minval(x, 10);	// use the dsp library

	if(!Glo.DoneFlag)
		UnDoneCount++;

	max = DSPF_sp_maxval(x, 10);	// use the dsp library again

									//
	while(!Glo.DoneFlag)
		UnDoneCount++;

	cfgQdma.dst = (Uint32) &y[10];	// make another copy
	Glo.DoneFlag = FALSE;

	EDMA_qdmaConfig(&cfgQdma);		// copy a buffer using qdma. Takes care of itself.

	while(!Glo.DoneFlag)
		UnDoneCount++;


	if(max > min)					// meaningless operation to keep optimization from removing calls
	return 0;
	else
		return 1;
} // main
//---------------------------------------------------------------------------------

//-------------------------------------------------------------------
interrupt void c_int8()             // interrupt service routine
{
	if(EDMA_intTest(0x9))           // qdma is completed. Could not poll EDMA reg. Must INT
	{
		EDMA_intClear(0x9);
		Glo.DoneFlag = TRUE;			// set the flag
	}

} // c_int8
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void init(void)
{
	Glo.DoneFlag = FALSE;
	CSL_init();

	IRQ_globalDisable();           	    //disable interrupts

    EDMA_intDisable(0x9);				// take care of old tcc codes from previous debugging
    EDMA_intClear(0x9);
    EDMA_intEnable(0x9);

    EDMA_clearPram(0x00000000);			// clear out everything

  	IRQ_setVecs(vectors);     			//point to the IRQ vector table
    IRQ_map(IRQ_EVT_EDMAINT, 8);		// EDMA to INT 8
    IRQ_reset(IRQ_EVT_EDMAINT);
    IRQ_enable(IRQ_EVT_EDMAINT);        // c_int8

    IRQ_globalEnable();       			//globally enable interrupts
} // init(void)
//-------------------------------------------------------------------


