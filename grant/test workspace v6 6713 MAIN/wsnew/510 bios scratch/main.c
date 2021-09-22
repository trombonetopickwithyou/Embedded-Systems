/*
 * main.c
 */
#include <log.h>

#include "dsk6713.h"
#include "csl.h"

#include "dsk6713BIOScfg.h"

int main(void) {

	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
    CSL_init();

    LOG_printf(&trace, "Hello World");


	return 0;
}
