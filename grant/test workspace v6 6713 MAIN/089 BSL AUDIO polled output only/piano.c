//----------------------------------------------------------------------------
// piano.c
//----------------------------------------------------------------------------

#undef _MAIN

#include "piano.h"


//----------------------------------------------------------------------------
// InitializeTable: Fill in the sine table 
//----------------------------------------------------------------------------
void InitializeTable(short *table, int TableLength)
{
int     i;
double  Coeff;
    
Coeff = 2. * PI / (double) TableLength;
for(i = 0; i < TableLength; i++)
  {                                     // 1 period
  table[i] = (double) 0x7FFF * MySin(Coeff * (double)(i));
  	  	  	  	  	  	  	  	  	  	  // no rounding. Rounding is a to-do topic from a future example
  }                                     // scaled to 16 bits

table[TableLength] = 0;					// redundant zero at the end for graphs and interpolation in the .999 range
} // InitializeTable
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// MakeMusic:
//----------------------------------------------------------------------------
void MakeMusic(short *table, int TableSize)
{
int RateVal;
int TableVal = 0;
unsigned int OutVal;

for(;;) // ever
  {
  RateVal = 0;
  if(DSK6713_DIP_get(0)==0) // SW0 is down
    {
    RateVal |= 1;
	DSK6713_LED_on(0);
	}
  else // SW0 is UP
    DSK6713_LED_off(0);

  if(DSK6713_DIP_get(1)==0)
    {
    RateVal |= 2;
	DSK6713_LED_on(1);
	}
  else // SW1 is UP
    DSK6713_LED_off(1);

  if(DSK6713_DIP_get(2)==0)
    {
    RateVal |= 4;
	DSK6713_LED_on(2);
	}
  else // SW2 is UP
    DSK6713_LED_off(2);

  if(DSK6713_DIP_get(3)==0)
    {
    RateVal |= 8;
	DSK6713_LED_on(3);
	}
  else // SW3 is UP
    DSK6713_LED_off(3);

  TableVal += RateVal;
  while(TableVal >= TableSize)
    TableVal -= TableSize;
  OutVal = table[TableVal];
  output_sample(OutVal);
  } // for(ever)
} // MakeMusic
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// MySin : We are cutting out the rts calls by including a Taylor series 
// expansion.
//----------------------------------------------------------------------------
 double MySin(double x)
{
int i;
double retval = 0.;
double xpow;
double ifac;
double xsquared;

while(x >= 2. * PI)                    // get into -360 to +360 degrees
  x -= 2. * PI;
while(x <= -2. * PI)
  x += 2. * PI;

xsquared = x*x;

xpow = x;
ifac = 1;

for(i=1; i<29;)
  {
  retval += xpow / ifac;               // ifac cannot go to zero
  xpow *= xsquared;
  ifac *= (i+1) * (i+2);
  i += 2;

  retval -= xpow / ifac;  
  xpow *= xsquared;
  ifac *= (i+1) * (i+2);
  i += 2;
  }
  
return retval;
} // MySin
 //----------------------------------------------------------------------------


// EOF

