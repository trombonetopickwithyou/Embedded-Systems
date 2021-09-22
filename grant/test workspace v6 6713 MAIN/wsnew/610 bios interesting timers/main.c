/*  * main.c  */

#include "debugcfg.h"

//------------------------------------------------------
#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"

//------------------------------------------------------
#define FTN_NONE 		0
#define FTN_LED_ON 		1
#define FTN_LED_OFF 	2
#define FTN_LED_TOGGLE 	3

#define TICKERCOUNT 	10
//------------------------------------------------------
typedef unsigned int UINT;

//------------------------------------------------------
typedef struct _TICKER
{
	UINT Number;
	UINT Delay;
	UINT RepeatCount;
	UINT RepeatTime;
	UINT Function;
	int arg0;
	int arg1;
} Ticker;


Ticker TickerList[TICKERCOUNT];


//------------------------------------------------------
void Init(void);
void TickerSet(UINT Number, UINT Delay, UINT RepeatCount, UINT RepeatTime, UINT Function, int arg0, int arg1);


//------------------------------------------------------
int main(void)
{
	Init();
	return 0;
} // main
//------------------------------------------------------


//------------------------------------------------------
void Init(void)
{
	int i;
	Ticker *ptr;

	DSK6713_init();
	CSL_init();
	DSK6713_DIP_init();
	DSK6713_LED_init();

	ptr = TickerList;
	for(i=0; i<TICKERCOUNT; i++)
	{
		ptr->Number = i;
		ptr++;
		TickerSet(i,0,0,0,0,0,0);
	}
} // Init
//------------------------------------------------------


//------------------------------------------------------
PRD0_ftn(int arg0, int arg1)
{
	int i;
	Ticker *ptr;

	ptr = TickerList;									// first ticker in the list
	
	for(i=0; i<TICKERCOUNT; i++)
	{
		if(ptr->Delay > 0)								// this ticker is active
		{
			ptr->Delay--;								// age it
			if(ptr->Delay == 0)							// see it it is done
			{
				if(ptr->RepeatCount > 0)				// see if it should be reloaded
				{
					if(ptr->RepeatCount < 0xFFFF)		// see if not forever
						ptr->RepeatCount--;				// decrement repeat count
					ptr->Delay = ptr->RepeatTime;		// launch another cycle
				}
				switch(ptr->Function)					// see what specific function is of interest
				{
				case FTN_LED_ON:
					DSK6713_LED_on(ptr->arg0);
					break;
				case FTN_LED_OFF:
					DSK6713_LED_off(ptr->arg0);
					break;
				case FTN_LED_TOGGLE:
					DSK6713_LED_toggle(ptr->arg0);
					break;
				} // switch(ptr->Function)
			}
		}
	ptr++;												// next ticker
	}

} // PRD0_ftn
//------------------------------------------------------

volatile int flag=0;			// set this by stopping the execution

//------------------------------------------------------
PRD1_ftn(int arg0p, int arg1p)
{

	volatile UINT Number = 0, Delay = 0, RepeatCount = 0, RepeatTime= 0, Function = FTN_NONE;
	volatile int arg0 = 0, arg1 = 0;

	if(flag > 0)
	{
		flag--;					// put a break point here. Set the variables above to pass in
		TickerSet(Number, Delay, RepeatCount, RepeatTime, Function, arg0, arg1);
	}

} // PRD1_ftn
//------------------------------------------------------


//------------------------------------------------------
void TickerSet(UINT Number, UINT Delay, UINT RepeatCount, UINT RepeatTime, UINT Function, int arg0, int arg1)
{
	Ticker *ptr;

	if(Number >= TICKERCOUNT)
		return;
	ptr = &TickerList[Number];

	ptr->Delay = Delay;
	ptr->RepeatCount = RepeatCount;
	ptr->RepeatTime = RepeatTime;
	ptr->Function = Function;
	ptr->arg0 = arg0;
	ptr->arg1 = arg1;

} // void TickerSet(UINT Number, UINT Delay, UINT RepeatCount, UINT RepeatTime, UINT Function, int arg0, int arg1)
//------------------------------------------------------
