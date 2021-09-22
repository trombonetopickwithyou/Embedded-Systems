/*  * main.c  */

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

#include "sts_atmcfg.h"
#include <sts.h>
#include <atm.h>
#include <clk.h>

//-----------------------------------------------------------------------
typedef struct _Globals
{
	unsigned int a_five_fence;
	STS_Obj *STS0;
	SEM_Obj *SEM0;
	TSK_Obj *TSK0;
	int universal_counter;
	unsigned int five_a_fence;

} Globals;

Globals Glo;

//-----------------------------------------------------------------------
int BIST(void);

//-----------------------------------------------------------------------
int main(void)
{
	Glo.a_five_fence = 0xa5a5a5a5;
	Glo.STS0 = &STS0;
//	Glo.SEM0 = &SEM0;
//	Glo.TSK0 = &TSK0;
	Glo.universal_counter = 0;
	Glo.five_a_fence = 0x5a5a5a5a;
	return 0;
} // main
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
PRD0_ftn(int arg0, int arg1)
{
int status;
int i;


STS_set(&STS0, CLK_getltime());
for(i=0; i<1000000; i++)
	status = ATM_inci(&Glo.universal_counter);
STS_delta(&STS0, CLK_getltime());
} // PRD0_ftn


//-----------------------------------------------------------------------
void TSK0_ftn(int arg0, int arg1)
{
	for(;;)
	  {
	  	SEM_pendBinary(&SEM0, SYS_FOREVER);
		Glo.universal_counter += 1000000000;		// not atomic, so PRD could cut in on it. Tweak SEM initial count
	  }
}// TSK0_ftn
//-----------------------------------------------------------------------




