//-------------------------------------------------------------------
#include "quecfg.h"
//-------------------------------------------------------------------


//-------------------------------------------------------------------
typedef struct Msg_Obj 
{
	QUE_Elem elem;    /* first field for QUE */
	Char val;         /* message value */
} MsgObj;
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void main(void)      // before DSP BIOS starts up
{
	int i;
	MsgObj *msg;
	QUE_Elem *moving_elem;
	
	for(i=0;i<26;i++)
	  {
	  	msg = MEM_alloc(1, sizeof(MsgObj),0);
	  	if(msg == MEM_ILLEGAL)
	  	  SYS_abort("Memory allocation failed!\n");
	  	else
	  	  {	  	  
	  	  LOG_printf(&LOG0, "alloc and put %d", i);
	  	  msg->val = 'A' + i;
	  	  QUE_put(&QUE0, msg);
	  	  }
	  }
	
	moving_elem = QUE_head(&QUE0);         // start at head 'A'
	moving_elem = QUE_next(moving_elem);   // work through 'B'
	moving_elem = QUE_next(moving_elem);   // 'C'
	moving_elem = QUE_next(moving_elem);   // 'D'
	moving_elem = QUE_next(moving_elem);   // 'E'

 	msg = MEM_alloc(1, sizeof(MsgObj),0);
  	if(msg == MEM_ILLEGAL)
  	  SYS_abort("Memory allocation failed!\n");
    LOG_printf(&LOG0, "alloc and insert");

	msg->val = 'x';

    QUE_insert(moving_elem, msg);          // put 'x' in front of 'E'

	moving_elem = QUE_next(moving_elem);   // 'F'
	moving_elem = QUE_next(moving_elem);   // 'G'
	moving_elem = QUE_next(moving_elem);   // 'H'

    QUE_remove(moving_elem);               // take out 'H'
    
    MEM_free(1, moving_elem, sizeof(MsgObj));         /* free msg */
	
} // main
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void PRD0_ftn(int arg0, int arg1)
{
	
    SEM_post(&SEM0);

} // PRD0_ftn
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void TSK0_ftn(int arg0, int arg1)
{
	MsgObj *msg;
	int status;
	
	for(;;)
      {
	  status = SEM_pend(&SEM0, SYS_FOREVER);
	  if (QUE_empty(&QUE0)) 
	    SYS_abort("queue empty\n");

      msg = QUE_get(&QUE0);                     /* dequeue message */
    
      LOG_printf(&LOG0, "get '%c'", msg->val); /* print value */
      MEM_free(1, msg, sizeof(MsgObj));         /* free msg */
      }
  	
} // TSK0_ftn
//-------------------------------------------------------------------

	
