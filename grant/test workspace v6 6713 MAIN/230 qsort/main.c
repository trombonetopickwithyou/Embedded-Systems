/*
 * main.c
 */

//--------------------------------------------------------------------------------
#include <stdlib.h>

//--------------------------------------------------------------------------------
#define BUFLEN 4

//--------------------------------------------------------------------------------
typedef struct _DBLOCK
{
	int x;							// 3 fields in a structure. e.g. Points in space
	int y;
	int z;
} DBlock;

//--------------------------------------------------------------------------------
DBlock data[BUFLEN];

//--------------------------------------------------------------------------------
void Init(void);
void SetDBlock(DBlock *ptr, int x, int y, int z);
int CompareDBlock (const DBlock *ptr1, const DBlock *ptr2);

//--------------------------------------------------------------------------------
int main(void)
{
	Init();
	qsort(data, BUFLEN, sizeof(DBlock), (__TI_compar_fn) CompareDBlock);	// specific for CCS warning
	return 0;
} // main
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
int CompareDBlock (const DBlock *ptr1, const DBlock *ptr2)
{
	if(ptr1->z == ptr2->z)
	{									// tie-breaker logic in here
		if(ptr1->y == ptr2->y)
		{									// tie-breaker logic in here
			if(ptr1->x == ptr2->x)
			{									// tie-breaker logic in here
				return 0;					// same z&y&x
			}
			if (ptr1->x > ptr2->x)
				return 1;					// same z&y, higher x
			else // (ptr1->x > ptr2->x)
				return -1;					// same z&y, lower x
		}
		if (ptr1->y > ptr2->y)
			return 1;						// same z, higher y
		else // (ptr1->y > ptr2->y)
			return -1;						// same z, lower y
	}
	if (ptr1->z > ptr2->z)
		return 1;							// higher z
	else // (ptr1->z > ptr2->z)
		return -1;							// lower z
} // int CompareDBlock (DBlock *ptr1, DBlock *ptr2)
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void Init(void)
{
	int i;
	for(i=0; i<BUFLEN; i++)
		SetDBlock(&data[i], rand(), rand(), rand());	// not totally random 16838 578 10113
} // Init
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void SetDBlock(DBlock *ptr, int x, int y, int z)
{
	ptr->x = x;								// put xyz numbers into ptr structure elements
	ptr->y = y;
	ptr->z = z;
} // void SetDBlock(DBlock *ptr, int x, int y, int z);
//--------------------------------------------------------------------------------


