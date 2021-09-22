// show use of fastmath from fastrts and tick count
// had to manipulate options in project linker

//---------------------------------------------------------------------------------------------
#include <stdio.h>
#include <c67fastMath.h>
// #include <math.h>

//---------------------------------------------------------------------------------------------
void main(void)           //
{                         // 18 ticks 	6
volatile int a = 1234;    // 24 ticks 	12
volatile int b = 345;     // 24 ticks 	12
volatile int c;
volatile float d = 1234.; // 30 ticks 	18
volatile float e = 345.;  // 30 ticks 	18
volatile float f, g, h;
volatile double i, j, k;

c = a / b;                // 66 ticks 	49

f = d / e;                // 841 ticks 	49
f = divsp(d,e);			  //            49
						  // out of c67fastMath.h

h = sqrtsp(f); 			  // 82 ticks 	60
						  // out of c67fastMath.h

h = 1 / h;                // 785 ticks	49
h = 1. / h;               // 2049 ticks 44
h = recipsp(h);           // 57 ticks 	40
						  // out of c67fastMath.h
h = _rcpsp(h);			  // intrinsic	42


i = (double) f;           // 38 ticks 	54

k = sqrtdp(i);            // 167 ticks 	150
						  // out of c67fastMath.h


k = 1 / k;                // 1957 ticks 114
k = 1. / k;               // 2098 ticks 104
k = recipdp(k);           // 122 ticks 	105
						  // out of c67fastMath.h
k = _rcpdp(k);			  // intrinsic  54

k = i / k;				  //            110
k = divdp(i,k);			  //            110
						  // out of c67fastMath.h


// g = sqrt(f);
// out of math.h 			 494 ticks 	317
// j = sqrt(i);
// out of math.h 			 402 ticks  256
} // main
//---------------------------------------------------------------------------------------------
