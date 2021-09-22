/*
 * main.c
 */

//---------------------------------------------------------------------------
typedef unsigned char 	BYTE;
typedef unsigned short 	WORD;
typedef unsigned int 	UINT;

//---------------------------------------------------------------------------
#define XFORMSIZE 256
#define PI 3.14159265358979323846264338327950288419716939937510582

//---------------------------------------------------------------------------
#include "c67fastMath.h"

//---------------------------------------------------------------------------
#pragma DATA_ALIGN (TWIDDLE256, 0x10)
int TWIDDLE256[(XFORMSIZE * 5)/4];        						// cos is 90 degrees lag from sin

#pragma DATA_ALIGN (BITREV256, 0x10)
BYTE BITREV256[XFORMSIZE];										// precalculated table that swaps address bits

#pragma DATA_ALIGN (DATAREAL, 0x10)
int DATAREAL[XFORMSIZE];										// input-real

#pragma DATA_ALIGN (DATAIMAG, 0x10)
int DATAIMAG[XFORMSIZE];										// input-imag

#pragma DATA_ALIGN (XDATAREAL, 0x10)
int XDATAREAL[XFORMSIZE];										// output-real

#pragma DATA_ALIGN (XDATAIMAG, 0x10)
int XDATAIMAG[XFORMSIZE];										// output-imag

#pragma DATA_ALIGN (TRANSREAL, 0x10)
int TRANSREAL[XFORMSIZE];										// working-real

#pragma DATA_ALIGN (TRANSIMAG, 0x10)
int TRANSIMAG[XFORMSIZE];										// working-imag

//---------------------------------------------------------------------------
void InitializeFFT(void);
void ComplexFFT(const int * restrict InX, const int * restrict InY,
                                      int * restrict OutX, int *restrict OutY);
void dfft(int *x, int *y, int *wcos, int *wsin, int n, int m);	// this is the ASM integer function

//---------------------------------------------------------------------------
int main(void)
{

	InitializeFFT();											// ramp 0 to 15
	ComplexFFT(DATAREAL, DATAIMAG, XDATAREAL, XDATAIMAG);		// DC = 7.5 -> 1920
	ComplexFFT(XDATAREAL, XDATAIMAG, DATAREAL, DATAIMAG);		// ramp goes backwards from 3840 (3832) to 0 (-50)
	ComplexFFT(DATAREAL, DATAIMAG, XDATAREAL, XDATAIMAG);		// DC term grows to 491520 (489984)
	ComplexFFT(XDATAREAL, XDATAIMAG, DATAREAL, DATAIMAG);		// ramp goes forwards again from 0 (-13386) to 988160 (979958)

	return 0;
} // main
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void ComplexFFT(const int * restrict InX, const int * restrict InY,
                                      int * restrict OutX, int *restrict OutY)
{
int       i,
          *sine,
          *cosine;

const BYTE *LookUp;

cosine = &TWIDDLE256[(XFORMSIZE)/4];
sine = &TWIDDLE256[0];
LookUp = BITREV256;

for(i = 0; i < XFORMSIZE; i++)								// dfft will put answer in same place as source
  {															// qdma is better way to do this
	TRANSREAL[i] = InX[i];
	TRANSIMAG[i] = InY[i];
  }

dfft(TRANSREAL, TRANSIMAG, cosine, sine, XFORMSIZE, 4);		// 4 is a magic number resulting from 256 size

for(i = 0 ; i < XFORMSIZE; i++)								// undoing bit reversal can't use QDMA
  {															// also can't do it in place
  OutX[i] = TRANSREAL[LookUp[i]];
  OutY[i] = TRANSIMAG[LookUp[i]];
  }

} // ComplexFFT
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

Coeff = 2. * PI / (double) XFORMSIZE;
for(i = 0; i < (XFORMSIZE * 5)/4; i++)
  {                                    						// 1.25 waves. cos in 90 deg after sin
	Amplitude = sindp(Coeff * (double)(i));
	Factor = (double) 0x7fffffff;

	if(Amplitude < 1.0)
		Factor += 1.0;


  Amplitude *= Factor;										// as big an integer as possible
  if(Amplitude >= 0)										// now round it
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
BitWidth -= 2;                         						// so calculation is quicker
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
	DATAREAL[i] = i & 0xF;									// real ramp 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0 1 2 3 ...
															// average = 7.5
	DATAIMAG[i] = 0;										// zero imag
  }
} // InitializeFFT
//---------------------------------------------------------------------------




