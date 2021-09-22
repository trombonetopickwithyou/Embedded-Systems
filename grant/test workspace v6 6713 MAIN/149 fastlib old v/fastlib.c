// show use of fastmath from fastrts and tick count

#include <stdio.h>
#include <fastrts67x.h>
// #include <math.h>
//---------------------------------------------------------------------------------------------

void main(void)           // 2335 ticks to get here from start of prog (not reset) + 
{                         // 18 ticks 	6
volatile int a = 1234;    // 24 ticks 	6
volatile int b = 345;     // 24 ticks 	12
volatile int c;
volatile float d = 1234.; // 30 ticks 	18
volatile float e = 345.;  // 30 ticks 	18
volatile float f, g, h;
volatile double i, j, k;

c = a / b;                // 66 ticks 	31

f = d / e;                // 841 ticks 	117
f = divsp(d,e);
						  // out of fastrts67x.h

h = sqrtsp(f); 			  // 82 ticks 	60
						  // out of fastrts67x.h

h = 1 / h;                // 785 ticks	76
h = 1. / h;               // 2049 ticks 131
h = recipsp(h);           // 57 ticks 	50
						  // out of fastrts67x.h

i = (double) f;           // 38 ticks 	4

k = sqrtdp(i);            // 167 ticks 	96
						  // out of fastrts67x.h

k = 1 / k;                // 1957 ticks 487
k = 1. / k;               // 2098 ticks 628
k = recipdp(k);           // 122 ticks 	105
						  // out of fastrts67x.h
k = i / k;
k = divdp(i,k);
						  // out of fastrts67x.h


// g = sqrt(f);
// out of math.h 			 494 ticks 	317
// j = sqrt(i);
// out of math.h 			 402 ticks  256
} // main
//---------------------------------------------------------------------------------------------
