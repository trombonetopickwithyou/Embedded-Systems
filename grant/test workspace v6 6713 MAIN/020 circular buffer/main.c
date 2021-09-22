#include <string.h>

//-------------------------------------------------------------------
typedef struct _Element {
      int x;
      int y;
      char z[32];
} Element;
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#define NULL 0
//-------------------------------------------------------------------

//-------------------------------------------------------------------
typedef struct _Globals {
      Element *ListBegin;
      Element *ListHead;
      Element *ListTail;
      Element *ListEnd;
} Globals;

Globals Glo;									// CMD file was tweaked to keep the BSS from starting at 0
//-------------------------------------------------------------------

#define LISTSIZE 8
//-------------------------------------------------------------------
#pragma DATA_ALIGN (List, 0x100)
Element List[LISTSIZE];
//-------------------------------------------------------------------
Element *ListAdd(Element *data);
Element *ListRemove(Element *data);

//-------------------------------------------------------------------
void main(void)
{
	
Element elem;
Element *retptr;

Glo.ListBegin = List;
Glo.ListHead = Glo.ListBegin;
Glo.ListTail = Glo.ListBegin;
Glo.ListEnd = &Glo.ListBegin[LISTSIZE];

retptr = ListRemove(&elem);						// nothing has been put on, so nothing gets removed. returns NULL

for(;;)											// add items until the list overflows
{
	elem.x = 1;
	elem.y = 10;
	strcpy(elem.z, "one");
	retptr = ListAdd(&elem);						// add an element with x = 1
	if(retptr == NULL)
		break;
	elem.x = 2;
	elem.y = 20;
	strcpy(elem.z, "two");
	retptr = ListAdd(&elem);
	if(retptr == NULL)
		break;
	elem.x = 3;
	elem.y = 30;
	strcpy(elem.z, "three");
	retptr = ListAdd(&elem);
	if(retptr == NULL)
		break;
} // for(ever)

retptr = ListRemove(&elem);						// populates with 1 / 10 / one
retptr = ListRemove(&elem);						// populates with 2 / 20 / two
retptr = ListRemove(&elem);						// populates with 3 / 30 / three

for(;;)											// remove items until the list underflows
{
	retptr = ListRemove(&elem);						// returns NULL eventually
	if(retptr == NULL)
		break;

} // for(ever)
} // main
//---------------------------------------------------------------------------------------------


//-------------------------------------------------------------------
Element *ListAdd(Element *data)
{
Element *ListNext;
Element *ListPrev;

ListNext = Glo.ListHead;
ListPrev = Glo.ListHead;
ListNext++;

if(ListNext == Glo.ListEnd)
  ListNext = Glo.ListBegin;
if(ListNext == Glo.ListTail)
  return (Element *) NULL;

*ListPrev = *data;
Glo.ListHead = ListNext;
return ListPrev;
} // ListAdd
//-------------------------------------------------------------------

//-------------------------------------------------------------------
Element *ListRemove(Element *data)
{
Element *ListNext;
Element *ListLast;

if(Glo.ListHead == Glo.ListTail)
  return (Element *) NULL;

ListLast = Glo.ListTail;
ListNext = Glo.ListTail;
ListNext++;

if(ListNext == Glo.ListEnd)
  ListNext = Glo.ListBegin;

*data = *ListLast;
Glo.ListTail = ListNext;
return ListLast;
} // ListRemove
//-------------------------------------------------------------------

