/*
 * main.c
 */

//---------------------------------------------------------------------------------------------
#include <stdio.h>
#include "DSK6713.h"
												// option to set access models to far
//---------------------------------------------------------------------------------------------
int main(void)
{
int x;

	DSK6713_init();								// option include paths to BSL header, CSL header
	CSL_init();
												// option link DSK6713.lib
												// optioned predefined CHIP_6713
	x = DSK6713_getVersion();
	printf("Library version %d\n", x);			// option to printf suport
	
	return 0;
} // main
//---------------------------------------------------------------------------------------------
