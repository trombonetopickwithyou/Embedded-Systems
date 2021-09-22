//-----------------------------------------------------------------------

#include "common.h"

void BitReverse(const int * restrict sourceREAL, const int * restrict sourceIMAG,  int * restrict destREAL,  int * restrict destIMAG);
void Downscale(const int * restrict sourceREAL, const int * restrict sourceIMAG,  int * restrict destREAL,  int * restrict destIMAG);
void FindMagnitudeSquared(const int *restrict sourcereal, const int *restrict sourceimag, int * restrict dest);
int FindMax(const int * restrict source, int * MaxVal,  int *MaxLoc);
void HalfCopyFlip(const short *restrict sourcereal, int * restrict destreal, int * restrict destimag);
void HalfCopyTrue(const short *restrict sourcereal, int * restrict destreal, int * restrict destimag);
void MultiplyComplex(const int * restrict source1real, const int * restrict source2real, const int * restrict source1imag, const int * restrict source2imag, int * restrict destreal, int * restrict destimag);


//-----------------------------------------------------------------------
void main(void)
{
	init();
	InitializeFFT();
    /* fall into DSP/BIOS idle loop */
    return;
} // void main(void)
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void init(void)
{
	int i;
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
	DSK6713_init();                   							//call BSL to init DSK-EMIF,PLL)
	CSL_init();

   	hCodec = DSK6713_AIC23_openCodec(0, &config1);
                                        						// codec has to be open before handle is referenced
                                        
    EDMA_clearPram(0x00000000);
                                        
    hEdma_rx = EDMA_open(EDMA_CHA_REVT1, EDMA_OPEN_RESET);
    hEdmaPing_rx = EDMA_allocTable(-1);
    hEdmaPong_rx = EDMA_allocTable(-1);

    cfgEdmaPing_rx.rld = EDMA_RLD_RMK(2,hEdmaPong_rx);
    cfgEdmaPong_rx.rld = EDMA_RLD_RMK(2,hEdmaPing_rx);
                                        
    cfgEdma_rx = cfgEdmaPing_rx;        						// start with ping
        
    EDMA_config(hEdma_rx, &cfgEdma_rx); 
    EDMA_config(hEdmaPing_rx, &cfgEdmaPing_rx); 
    EDMA_config(hEdmaPong_rx, &cfgEdmaPong_rx); 

    hEdma_tx = EDMA_open(EDMA_CHA_XEVT1, EDMA_OPEN_RESET);
    hEdmaPing_tx = EDMA_allocTable(-1);
    hEdmaPong_tx = EDMA_allocTable(-1);

    cfgEdmaPing_tx.rld = EDMA_RLD_RMK(2,hEdmaPong_tx);
    cfgEdmaPong_tx.rld = EDMA_RLD_RMK(2,hEdmaPing_tx);
                                        
    cfgEdma_tx = cfgEdmaPing_tx;        						// start with ping
        
    EDMA_config(hEdma_tx, &cfgEdma_tx); 
    EDMA_config(hEdmaPing_tx, &cfgEdmaPing_tx); 
    EDMA_config(hEdmaPong_tx, &cfgEdmaPong_tx); 

    EDMA_intDisable(0xF);               						// use 4 EDMA events
    EDMA_intClear(0xF);                 						// on one EDMA interrupt
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

    EDMA_enableChannel(hEdma_rx);       						// start on the copy of ping

    EDMA_enableChannel(hEdma_tx);       						// start on the copy of ping
                                        
    IRQ_reset(IRQ_EVT_EDMAINT);
    IRQ_enable(IRQ_EVT_EDMAINT);        						// c_int8
    
  	
  	output_sample(0);                   						// start TX to launch EDMA
	
} //void init(void)
//-----------------------------------------------------------------------

//-------------------------------------------------------------------
void output_sample(int out_data)    							//for out to Left and Right channels
{

	AIC_data.uint=out_data;          							//32-bit data -->data structure

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.uint);		//write/output data
		
} // void output_sample(int out_data)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int c_int8(int arg0)            								// interrupt service routine dispatches this subroutine
{ 
	if(EDMA_intTest(0xF))         							 	// rx_ping is full
	{
		EDMA_intClear(0xF);
		SEM_post(&SEM_ping);									// rather than setting a flag
	}

	if(EDMA_intTest(0xE))          								// tx_ping has been emptied
	{
		EDMA_intClear(0xE);										// not using except to see activity when debugging
	}
	if(EDMA_intTest(0xD))          								// rx_pong is full
	{
		EDMA_intClear(0xD);
		SEM_post(&SEM_pong);									// rather than setting a flag
	}

	if(EDMA_intTest(0xC))          								// tx_pong has been emptied
	{
		EDMA_intClear(0xC);										// not using except to see activity when debugging
	}
	
	if(EDMA_intTest(0x9))          								// qdma_ping has been emptied
	{
		EDMA_intClear(0x9);
		SEM_post(&SEM_qdma);									// rather than setting a flag

	}
	if(EDMA_intTest(0x8))          								// qdma_pong has been emptied
	{
		EDMA_intClear(0x8);
		SEM_post(&SEM_qdma);									// rather than setting a flag
	}

	return arg0;

} // c_int8
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void TSK_ping_ftn(void)
{
	for(;;)
	{
		SEM_pend(&SEM_ping, SYS_FOREVER);
		SEM_pend(&SEM_qdma, SYS_FOREVER);
		EDMA_qdmaConfig(&cfgQdma_ping);							// copy a buffer using qdma. Takes care of itself. Runs speakers
		MoveData(rx_ping);										// go do FFT with same input buffer
	}
	
} // int SWI_ping_ftn(int arg0, int arg1)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void TSK_pong_ftn(void)
{
	for(;;)
	{
		SEM_pend(&SEM_pong, SYS_FOREVER);
		SEM_pend(&SEM_qdma, SYS_FOREVER);
		EDMA_qdmaConfig(&cfgQdma_pong);							// copy a buffer using qdma. Takes care of itself.
		MoveData(rx_pong);
	}
	
} // int SWI_pong_ftn(int arg0, int arg1)
//-------------------------------------------------------------------

//---------------------------------------------------------------------------
// This does a cross correlation, which is similar to a convolution, with lots of time reversing and conjugating
// It also uses a forward fft to achieve the effect of an inverse fft with more phase swapping
//---------------------------------------------------------------------------

void MoveData(short data[BUFWIDTH][BUFLEN])
{
int *sine,
	*cosine;

int MaxVal = 0x80000000;
int MaxLoc = -1;
static int WantLeft = 0;
static int WantRight = 0;
static int WantCenter = 0;

sine = &TWIDDLE256[0];											// from start of pre-calcualted table
cosine = &TWIDDLE256[(BUFLEN)/4];								// shifted into pre-calculated table

HalfCopyTrue(data[RIGHT], TRANSREAL[RIGHT], TRANSIMAG[RIGHT]);	// true copy with zero padding

HalfCopyFlip(data[LEFT], TRANSREAL[LEFT], TRANSIMAG[LEFT]);		// flipped copy with zero padding. Conjugate if complex input

dfft(TRANSREAL[RIGHT], TRANSIMAG[RIGHT], cosine, sine, BUFLEN, 4);
																// dfft of RIGHT

BitReverse(TRANSREAL[RIGHT], TRANSIMAG[RIGHT], XDATAREAL, XDATAIMAG);
																// undo bit reversal with pre-calcualted table

Downscale(XDATAREAL, XDATAIMAG, TRANSREAL[RIGHT], TRANSIMAG[RIGHT]);
																// Make numbers smaller so the MULT doesn't roll

dfft(TRANSREAL[LEFT], TRANSIMAG[LEFT], cosine, sine, BUFLEN, 4);
																// dfft of LEFT

BitReverse(TRANSREAL[LEFT], TRANSIMAG[LEFT], XDATAREAL, XDATAIMAG);

Downscale(XDATAREAL, XDATAIMAG, TRANSREAL[LEFT], TRANSIMAG[LEFT]);

MultiplyComplex(TRANSREAL[RIGHT], TRANSREAL[LEFT], TRANSIMAG[RIGHT], TRANSIMAG[LEFT], XDATAREAL, XDATAIMAG);

																// real and image swapped at input
dfft(XDATAIMAG, XDATAREAL, cosine, sine, BUFLEN, 4);			// dfft crosses real & imag at input and output when used for idfft

BitReverse(XDATAREAL, XDATAIMAG, TRANSIMAG[RIGHT], TRANSREAL[RIGHT]);
																// real and imag swapped again at output

Downscale(TRANSREAL[RIGHT], TRANSIMAG[RIGHT], XDATAREAL, XDATAIMAG);

FindMagnitudeSquared(XDATAREAL, XDATAIMAG, TRANSREAL[RIGHT]); 	// no reason to calc square roots

FindMax(TRANSREAL[RIGHT], &MaxVal, &MaxLoc);

if(MaxVal < 10)													// a minimum signal to turn on any lights
	{
	DSK6713_LED_off(0);
	DSK6713_LED_off(1);
	DSK6713_LED_off(2);
	DSK6713_LED_off(3);
	WantLeft = 0;
	WantRight = 0;
	WantCenter = 0;
	}

else if(MaxLoc == 0)											// centered
	{
	if(WantCenter >= 1)
		{
		DSK6713_LED_off(0);
		DSK6713_LED_on(1);
		DSK6713_LED_off(2);
		}
	else
		{
		WantCenter++;
		WantLeft = 0;
		WantRight = 0;
		}
	}
else if(MaxLoc < 128)											// off to the LEFT
	{
	if(WantLeft >= 1)
		{
		DSK6713_LED_on(0);
		DSK6713_LED_off(1);
		DSK6713_LED_off(2);
		}
	else
		{
		WantLeft++;
		WantCenter = 0;
		WantRight = 0;
		}
	}
else // MaxLoc >= 128											// off to the RIGHT
	{
	if(WantRight >= 1)
		{
		DSK6713_LED_off(0);
		DSK6713_LED_off(1);
		DSK6713_LED_on(2);
		}
	else
		{
		WantRight++;
		WantLeft = 0;
		WantCenter = 0;
		}
	}

}	// MoveData
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(BitReverse, "-o3")						// optimize this heavily - it works
void BitReverse(const int * restrict sourceREAL, const int * restrict sourceIMAG,  int * restrict destREAL,  int * restrict destIMAG)
{
int i;

for(i=0; i<BUFLEN;i++)
	{
	destREAL[i] = sourceREAL[BITREV256[i]];						// use lookup table to find correct address
	destIMAG[i] = sourceIMAG[BITREV256[i]];
	}

} // BitReverse
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(Downscale, "-o3")						// optimize this heavily - it works
void Downscale(const int * restrict sourceREAL, const int * restrict sourceIMAG,  int * restrict destREAL,  int * restrict destIMAG)
{
int i;
int data;

for(i=0; i<BUFLEN;i++)
	{
	if(sourceREAL[i] >= 0)
		data = _sadd(sourceREAL[i], 128);				// add 1/2 LSB if positive
	else
		data = _ssub(sourceREAL[i], 128);				// subtract 1/2 LSB if negative
	destREAL[i] = (data)/256;							// compiler will use bit shift

	if(sourceIMAG[i] >= 0)
		data = _sadd(sourceIMAG[i], 128);				// add 1/2 LSB if positive
	else
		data = _ssub(sourceIMAG[i], 128);				// subtract 1/2 LSB if negative
	destIMAG[i] = (data)/256;							// compiler will use bit shift

	}

} // Downscale
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(FindMagnitudeSquared, "-o3")			// optimize this heavily - it works
void FindMagnitudeSquared(const int *restrict sourcereal, const int *restrict sourceimag, int * restrict dest)
{
long lx;
int i;

for(i=0; i<BUFLEN;i++)
	{
	lx = _lsadd(sourcereal[i]*sourcereal[i], 0);				// intrinsics allow more bits to work
	lx = _lsadd(sourceimag[i]*sourceimag[i], lx);
	dest[i] = _sat(lx);
	}
} // FindMagnitude
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(FindMax, "-o3")						// optimize this heavily - it works
int FindMax(const int * restrict source, int * MaxVal,  int *MaxLoc)
{
int i;

for(i=0; i<BUFLEN;i++)											// bubble sort for one bubble IMAG is very small, although not all zero
	{
	if(source[i] > *MaxVal)
		{
		*MaxVal = source[i];
		*MaxLoc = i;
		}
	}
return *MaxVal;
} // FindMax
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(HalfCopyFlip, "-o3")					// optimize this heavily - it works
void HalfCopyFlip(const short *restrict sourcereal, int * restrict destreal, int * restrict destimag)
{
int i;

for(i=0; i<BUFLEN;i++)
	{

	if(i == 0)
		{
		destreal[0] = (int) sourcereal[0];						// reverse time around 0 for LEFT
		destimag[i]  = -0;										// need to conjugate incoming LEFT data.
		}
	else if(i <= BUFLEN/2)
		{
		destreal[i] = 0;										// zero pad half of LEFT
		destimag[i]  = -0;										// need to conjugate incoming LEFT data.
		}
	else // i > BUFLEN/2
		{
		destreal[i]  = (int) sourcereal[BUFLEN-i];				// take incoming LEFT backwards in time around 0
		destimag[i]  = -0;										// need to conjugate incoming LEFT data.
		}
	}

} // HalfCopyTrue
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(HalfCopyTrue, "-o3")					// optimize this heavily - it works
void HalfCopyTrue(const short *restrict sourcereal, int * restrict destreal, int * restrict destimag)
{
int i;

for(i=0; i<BUFLEN;i++)
	{
	if(i<BUFLEN/2)
		{
		destreal[i] = (int) sourcereal[i];						// take half of the RIGHT block
		destimag[i] = 0;
		}
	else
		{
		destreal[i] = 0;										// zero pad the other half of RIGHT
		destimag[i] = 0;
		}
	}

} // HalfCopyTrue
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void InitializeFFT(void)
{
double Amplitude,
	Factor,
      Coeff;
int i,
    j;
int BitWidth,
    DataIn,
    DataOut,
    Temp;

Coeff = 2. * PI / (double) BUFLEN;
for(i = 0; i < (BUFLEN * 5)/4; i++)
  {                                    							// 1.25 waves. cos in 90 deg after sin
	Amplitude = sindp(Coeff * (double)(i));						// from fast RTS
	Factor = (double) 0x7fffffff;
	if(Amplitude < 1.)
		Factor = Factor + 1.;									// A little cheating here.
																// +1 (90 degrees) gets scaled by 0x7fffffff
																// all other numbers get scaled by 0x80000000
																// this makes the undo scaling function more accurate

  Amplitude *= Factor;											// as big an integer as possible
  if(Amplitude >= 0)											// now round it
	  TWIDDLE256[i] = (int) (Amplitude + .5);
  else
	  TWIDDLE256[i] = (int) (Amplitude - .5);
  }

j = BUFLEN >> 1;
BitWidth = 0;
while(j)
  {
  j >>= 1;
  BitWidth++;
  }
BitWidth -= 2;                         							// so calculation is quicker
for(i=0;i<BUFLEN;i++)
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
} // InitializeFFT
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma FUNCTION_OPTIONS(MultiplyComplex, "-o3")				// optimize this heavily - it works
void MultiplyComplex(const int * restrict source1real, const int * restrict source2real,
		const int * restrict source1imag, const int * restrict source2imag,
		int * restrict destreal, int * restrict destimag)
{
int i;
long lx;

for(i=0; i<BUFLEN;i++)											// multiply RIGHT * LEFT
	{
	lx = _lsadd(source1real[i]*source2real[i], 0);
	lx = _lsadd(-source1imag[i]*source2imag[i], lx);
	destreal[i] = _sat(lx);
//	XDATAREAL[i] = TRANSREAL[RIGHT][i]*TRANSREAL[LEFT][i] - TRANSIMAG[RIGHT][i]*TRANSIMAG[LEFT][i];

	lx = _lsadd(source1real[i]*source2imag[i], 0);
	lx = _lsadd(source1imag[i]*source2real[i], lx);
	destimag[i] = _sat(lx);
//	XDATAIMAG[i] = TRANSREAL[RIGHT][i]*TRANSIMAG[LEFT][i] + TRANSIMAG[RIGHT][i]*TRANSREAL[LEFT][i];
	}

} //  MultiplyComplex
//---------------------------------------------------------------------------

