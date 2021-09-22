/*
 * main.c
 */

//---------------------------------------------------------------------------
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;

//---------------------------------------------------------------------------
#define XFORMSIZE 256
#define PI 3.14159265358979323846264338327950288419716939937510582

//---------------------------------------------------------------------------
#include "c67fastMath.h"
#include "QDMAcfg.h"
#include "csl.h"
#include "csl_edma.h"
#include "sem.h"

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
	  EDMA_SRC_OF(0),        /* Source address register
	                             &x    - source address                          */

	  EDMA_CNT_OF(XFORMSIZE), /* Transfer count parameter
	                             BUFFSIZE      - buffer size in # of floats (int)*/

	  EDMA_DST_OF(0),        /* Destination address parameter
	                             &y - destination address                        */

	  EDMA_IDX_OF(0),        /* Index parameter
	                             N/A for QDMA		                             */

	  EDMA_RLD_OF(0x00)      /* Count reload/link parameter
 							     N/A for QDMA		                             */
	};


//---------------------------------------------------------------------------
#pragma DATA_ALIGN (TWIDDLE256, 0x10)
int TWIDDLE256[(XFORMSIZE * 5)/4];        	// cos is 90 degrees lag from sin

#pragma DATA_ALIGN (BITREV256, 0x10)
BYTE BITREV256[XFORMSIZE];					// precalculated table that swaps address bits

#pragma DATA_ALIGN (DATAREAL, 0x10)
int DATAREAL[XFORMSIZE];					// input-real

#pragma DATA_ALIGN (DATAIMAG, 0x10)
int DATAIMAG[XFORMSIZE];					// input-imag

#pragma DATA_ALIGN (XDATAREAL, 0x10)
int XDATAREAL[XFORMSIZE];					// output-real

#pragma DATA_ALIGN (XDATAIMAG, 0x10)
int XDATAIMAG[XFORMSIZE];					// output-imag

#pragma DATA_ALIGN (TRANSREAL, 0x10)
int TRANSREAL[XFORMSIZE];				// working-real

#pragma DATA_ALIGN (TRANSIMAG, 0x10)
int TRANSIMAG[XFORMSIZE];				// working-imag

//---------------------------------------------------------------------------
void dfft(int *x, int *y, int *wcos, int *wsin, int n, int m);		// this is the ASM integer fft function
void Init(void);
void InitializeFFT(void);

//---------------------------------------------------------------------------
int main(void)
{

	InitializeFFT();
	CSL_init();
	Init();
	return 0;
} // main
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void Init(void)
{
    EDMA_intDisable(0x9);				// take care of old tcc codes from previous debugging
    EDMA_intClear(0x9);
    EDMA_intEnable(0x9);

    EDMA_clearPram(0x00000000);			// clear out everything

    IRQ_reset(IRQ_EVT_EDMAINT);
    IRQ_enable(IRQ_EVT_EDMAINT);        // c_int8

} // Init
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void InitializeFFT(void)
{
double Amplitude,
      Coeff;
int i,
    j;
int BitWidth,
    DataIn,
    DataOut,
    Temp;

Coeff = 2. * PI / (double) XFORMSIZE;
for(i = 0; i < (XFORMSIZE * 5)/4; i++)
  {                                    	// 1.25 waves. cos in 90 deg after sin
	Amplitude = sindp(Coeff * (double)(i));
  Amplitude *= (double) 0x7fffffff;		// as big an integer as possible
  if(Amplitude >= 0)					// now round it
	  TWIDDLE256[i] = (int) (Amplitude + .5);
  else
	  TWIDDLE256[i] = (int) (Amplitude - .5);
  }

j = XFORMSIZE >> 1;
BitWidth = 0;
while(j)
  {
  j >>= 1;
  BitWidth++;
  }
BitWidth -= 2;                         // so calculation is quicker
for(i=0;i<XFORMSIZE;i++)
  {
  DataOut = 0;
  DataIn = i;
  for(j=BitWidth; j>=0; j-=2)
    {
    Temp = DataIn & 0x3;
    DataOut |= Temp << j;
    DataIn >>= 2;
    }
  BITREV256[i] = DataOut;
  }

for(i=0;i<XFORMSIZE;i++)
  {
	DATAREAL[i] = i & 0xF;				// real ramp
	DATAIMAG[i] = 0;					// no imag
  }
} // InitializeFFT
//---------------------------------------------------------------------------


//------------------------------------------------------
PRD0_ftn(int arg0, int arg1)
{
	// move to SWI at lower priority in case SEM not ready
	SEM_postBinary(&SEM_FFT);

} // PRD0_ftn(int arg0, int arg1)
//------------------------------------------------------


//------------------------------------------------------
c_int8(int arg0)
{

	// move to SWI at lower priority level
	if(EDMA_intTest(0x9))           // qdma is completed. Could not poll EDMA reg. Must INT
	{
		EDMA_intClear(0x9);
		SEM_postBinary(&SEM_QDMA);
	}

} // c_int8
//------------------------------------------------------


//------------------------------------------------------
TSK_QDMA_ftn(int arg0, int arg1)
{
	int       i,
	          *sine,
	          *cosine;

	const BYTE *LookUp;
	cosine = &TWIDDLE256[(XFORMSIZE)/4];		// shifted into pre-calculated table
	sine = &TWIDDLE256[0];						// from start of pre-calcualted table
	LookUp = BITREV256;							// Another pre-calcualted table

	// Launch QDMA xfer
	for(;;) // ever
	{
		SEM_pendBinary(&SEM_FFT, SYS_FOREVER);	// wait for something to do
												// first xfer
		cfgQdma.src = (Uint32) DATAREAL;
		cfgQdma.dst = (Uint32) TRANSREAL;
		EDMA_qdmaConfig(&cfgQdma);				// copy a buffer using qdma. Takes care of itself.

		SEM_pendBinary(&SEM_QDMA, SYS_FOREVER);	// wait for xfer to complete
												// second xfer
		cfgQdma.src = (Uint32) DATAIMAG;
		cfgQdma.dst = (Uint32) TRANSIMAG;
		EDMA_qdmaConfig(&cfgQdma);				// copy a buffer using qdma. Takes care of itself.
		SEM_pendBinary(&SEM_QDMA, SYS_FOREVER);	// wait for xfer to complete

												// FFT
		dfft(TRANSREAL, TRANSIMAG, cosine, sine, XFORMSIZE, 4);
												// 4 is a magic number resulting from 256 size

		for(i = 0 ; i < XFORMSIZE; i++)			// undoing bit reversal can't use QDMA
		{										// also can't do it in place
			XDATAREAL[i] = TRANSREAL[LookUp[i]];
			XDATAIMAG[i] = TRANSIMAG[LookUp[i]];
		}
	} // for(ever)
} // TSK_QDMA_ftn
//------------------------------------------------------
