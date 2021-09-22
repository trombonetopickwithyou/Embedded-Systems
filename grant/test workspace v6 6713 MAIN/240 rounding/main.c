/*
 * main.c
 */

//--------------------------------------------------------------------------
int roundd(double x);
int roundf(float x);
int roundf2(float x);


//--------------------------------------------------------------------------
int main(void)
{
	volatile int i;

	i = 1.4;			// 1
	i = 1.6;			// 1
	i = -1.4;			// -1
	i = -1.6;			// -1

	i = 3 / 2;			// 1		round toward 0
	i = -3 / 2;			// -1		round toward 0

	i = 3 >> 1;			// 1		same answer as /2
	i = -3 >> 1;		// -2		not same answer as /2

	i = roundf(1.4);	// watch tick counts for different options
	i = roundf(1.6);	//
	i = roundf(-1.4);	//
	i = roundf(-1.6);	//

	i = roundd(1.4);	//
	i = roundd(1.6);	//
	i = roundd(-1.4);	//
	i = roundd(-1.6);	//
	
	i = roundf2(1.4);	//
	i = roundf2(1.6);	//
	i = roundf2(-1.4);	//
	i = roundf2(-1.6);	//

	volatile unsigned int j;

	j = 1.4;			// 1
	j = 1.6;			// 1
	j = -1.4;			// 0xffffffff	- gets a warning cuz j is unsigned
	j = -1.6;			// 0xffffffff	- gets a warning cuz j is unsigned

	j = 3 / 2;			// 1
	j = -3 / 2;			// 0xffffffff	- gets a warning cuz j is unsigned

	j = 3 >> 1;			// 1
	j = -3 >> 1;		// 0xfffffffe	- gets a warning cuz j is unsigned

	j = roundf(1.4);	//
	j = roundf(1.6);	//
	j = roundf(-1.4);	//
	j = roundf(-1.6);	//

	j = roundd(1.4);	//
	j = roundd(1.6);	//
	j = roundd(-1.4);	//
	j = roundd(-1.6);	//

	j = roundf2(1.4);	//
	j = roundf2(1.6);	//
	j = roundf2(-1.4);	//
	j = roundf2(-1.6);	//


	return i+j;			//
} // main
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
int roundd(double x)
{
	if(x >= 0.0)
		return x + .5;
	else
		return x - .5;
} // roundd
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
int roundf(float x)
{
	if(x >= 0.0F)				// uses float
		return x + .5F;			// uses float
	else
		return x - .5F;			// uses float
} // roundf
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int roundf2(float x)
{
	if(x >= 0.0)				// uses double precision
		return x + .5;			// uses double precision
	else
		return x - .5;			// uses double precision
} // roundf2
//--------------------------------------------------------------------------

