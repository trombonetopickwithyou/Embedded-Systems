//-------------------------------------------------------------------
#include "queuecfg.h"
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#define BUFFLEN 4
#define LISTLEN 8
//-------------------------------------------------------------------

//-------------------------------------------------------------------
typedef struct Msg_Obj 
{
	QUE_Elem elem;    /* first field for QUE */
	Char val[BUFFLEN];         /* message value */
} MsgObj;
//-------------------------------------------------------------------

MsgObj List[LISTLEN];


//-------------------------------------------------------------------
void main(void)
{
	int i;
	MsgObj *msg;
	
	for(i=0;i<LISTLEN;i++)                                  // eight elements end up being created
	  {
	  msg = &List[i];
	  msg->val[0] = 'A' + i; 
	  LOG_printf(&LOG0, "storage '%c'", msg->val[0]);
	  QUE_put(&QUE0_storage, msg);                    // all in the unused storage queue
	  } // for(i)
	  
     msg = QUE_get(&QUE0_storage);                     /* dequeue message from storage */
	 QUE_put(&QUE1_usenow, msg);                       // put message into high priority queue
     msg = QUE_get(&QUE0_storage);                     /* dequeue message from storage */
	 QUE_put(&QUE2_uselater, msg);                     // put message into low priority queue
     msg = QUE_get(&QUE0_storage);                     /* dequeue message from storage */
	 QUE_put(&QUE1_usenow, msg);                       // put message into high priority queue
     msg = QUE_get(&QUE0_storage);                     /* dequeue message from storage */
	 QUE_put(&QUE2_uselater, msg);                     // put message into low priority queue

     while (!QUE_empty(&QUE1_usenow)) 
     {
       msg = QUE_get(&QUE1_usenow);                    /* dequeue message */
       LOG_printf(&LOG0, "now '%c'", msg->val[0]);     /* print value */
       QUE_put(&QUE0_storage, msg);                    // put message into storage queue
     }

     while (!QUE_empty(&QUE2_uselater)) 
     {
       msg = QUE_get(&QUE2_uselater);                    /* dequeue message */
       LOG_printf(&LOG0, "later '%c'", msg->val[0]);     /* print value */
       QUE_put(&QUE0_storage, msg);                    // put message into storage queue
     }

     while (!QUE_empty(&QUE0_storage)) 
     {
       msg = QUE_get(&QUE0_storage);                    /* dequeue message */
       LOG_printf(&LOG0, "storage '%c'", msg->val[0]);   /* print value */
       QUE_put(&QUE0_storage, msg);                     // put message into storage queue
     }
	  
} // main
//-------------------------------------------------------------------

#ifdef NEVER
	  	msg = MEM_alloc(1, sizeof(MsgObj),0);
	  	if(msg == MEM_ILLEGAL)
	  	  SYS_abort("Memory allocation failed!\n");
	  	else
	  	  {	  	  
	  	  msg->val[0] = 'A' + i; 
	  	  LOG_printf(&LOG0, "storage '%c'", msg->val[0]);
	  	  QUE_put(&QUE0_storage, msg);                 // all in the unused storage queue
	  	  }
#endif
