#ifndef PIANO_H
#define PIANO_H

//---------------------------------------------------------------------------
#include "csl.h"
#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file
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
#define PI  3.14159265358979323846264338327950288419716939937510
#endif

//---------------------------------------------------------------------------
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short WORD;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void main(void);
void initialize(void);
void InitializeTable(short *table, int TableLength);
void MakeMusic(short *table, int TableSize);
double MySin(double x);
void output_sample(int);
//----------------------------------------------------------------------------


#endif
