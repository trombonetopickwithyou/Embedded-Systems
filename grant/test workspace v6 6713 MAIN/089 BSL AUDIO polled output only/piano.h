#ifndef PIANO_H
#define PIANO_H

//---------------------------------------------------------------------------
#include "dsk6713_aic23.h"  	  						//codec-dsk support file
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#define TABLESIZE 256

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE  0
#endif
#ifndef PI
#define PI  3.14159265358979323846264338327950288419716939937510	// according to Internet
#endif

#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data
//-------------------------------------------------------------------

//-------------------------------------------------------------------
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short WORD;
//-------------------------------------------------------------------

//-------------------------------------------------------------------
typedef struct _Globals {
	short sine_table[TABLESIZE+1];                      //sine values
	DSK6713_AIC23_CodecHandle hCodec;
} Globals;
//-------------------------------------------------------------------


//-------------------------------------------------------------------
#ifndef _MAIN
extern
#endif
Globals Glo;
//-------------------------------------------------------------------


//---------------------------------------------------------------------------
void main(void);
void initialize (void);
void InitializeTable(short *table, int TableLength);
void MakeMusic(short *table, int TableSize);
double MySin(double x);
void output_sample(int);
//-------------------------------------------------------------------
#endif


