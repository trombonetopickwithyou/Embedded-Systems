#ifndef DUAL_MIKES_APP_EDMA_H_
#define DUAL_MIKES_APP_EDMA_H_

/* Create the EDMA configuration structure for ping_rx transfers */
EDMA_Config cfgEdmaPing_rx = {  
  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT    */ 
  
   EDMA_OPT_PRI_HIGH,    /* Priority levels for EDMA events:
                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer*/
   
   EDMA_OPT_ESIZE_16BIT,/* Element size :-
                             EDMA_OPT_ESIZE_32BIT - 32 bit word 
                             EDMA_OPT_ESIZE_16BIT - 16 bit word 
                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */
   
   EDMA_OPT_2DS_NO,     /* Source dimension :-
                             EDMA_OPT_2DS_NO  - 1-dimensional source    
                             EDMA_OPT_2DS_YES - 2-dimensional source         */  
   
   EDMA_OPT_SUM_NONE,   /* Source address update mode :-           
                             EDMA_OPT_SUM_NONE - Fixed address mode 
                             EDMA_OPT_SUM_INC  - Increment address mode
                             EDMA_OPT_SUM_DEC  - Decrement address mode
                             EDMA_OPT_SUM_IDX  - Address modified by element 
                                                 index or frame Index        */
    EDMA_OPT_2DD_NO,     /* Destination dimension :-  
                             EDMA_OPT_2DD_NO   - 1-dimensional source            
                             EDMA_OPT_2DD_YES  - 2-dimensional source        */
   
    EDMA_OPT_DUM_IDX,    /* Destination address update mode :-                       
                             EDMA_OPT_DUM_NONE - Fixed address mode            
                             EDMA_OPT_DUM_INC  - Increment address mode        
                             EDMA_OPT_DUM_DEC  - Decrement address mode       
                             EDMA_OPT_DUM_IDX  - Address modified by element   
                                                 index or frame Index        */
    
    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
                             EDMA_OPT_TCINT_NO  - Indication disabled      
                             EDMA_OPT_TCINT_YES - Indication enabled         */  
    
    EDMA_OPT_TCC_OF(0xF),/* Transfer complete code                     */
    
    EDMA_OPT_LINK_YES,   /* Linking of event parameters
                             EDMA_OPT_LINK_NO   -  Disabled 
                             EDMA_OPT_LINK_YES  -  Enabled                   */    
    
    EDMA_OPT_FS_NO       /* Frame synchronization
                             EDMA_OPT_FS_NO  - Channel is element/array 
                                                synchronized 
                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
  ),
  EDMA_SRC_OF(0x34000000u),/* Source address register
                               MCBSP - source address                */
  
  EDMA_CNT_OF(0xFF0002),   /* Transfer count parameter
                                BUFF_SZ      - buffer sizes in # of ints     */
                                // 0x40 frames of 2 elements (L/R)
  
  EDMA_DST_OF(rx_ping),      /* Destination address parameter
                                 ping - destination address                  */ 
  
  EDMA_IDX_OF(0xFE020200),/* Index parameter                                 */
                                // back up(-) 0x7E, advance(+) 0x80
  
  EDMA_RLD_OF(0x00) /* Count reload/link parameter                     */
};


/* Create the EDMA configuration structure for pong_rx transfers */
EDMA_Config cfgEdmaPong_rx = {  
  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT    */ 
  
   EDMA_OPT_PRI_HIGH,    /* Priority levels for EDMA events:
                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer*/
   
   EDMA_OPT_ESIZE_16BIT,/* Element size :-
                             EDMA_OPT_ESIZE_32BIT - 32 bit word 
                             EDMA_OPT_ESIZE_16BIT - 16 bit word 
                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */
   
   EDMA_OPT_2DS_NO,     /* Source dimension :-
                             EDMA_OPT_2DS_NO  - 1-dimensional source    
                             EDMA_OPT_2DS_YES - 2-dimensional source         */  
   
   EDMA_OPT_SUM_NONE,   /* Source address update mode :-           
                             EDMA_OPT_SUM_NONE - Fixed address mode 
                             EDMA_OPT_SUM_INC  - Increment address mode
                             EDMA_OPT_SUM_DEC  - Decrement address mode
                             EDMA_OPT_SUM_IDX  - Address modified by element 
                                                 index or frame Index        */
    EDMA_OPT_2DD_NO,     /* Destination dimension :-  
                             EDMA_OPT_2DD_NO   - 1-dimensional source            
                             EDMA_OPT_2DD_YES  - 2-dimensional source        */
   
    EDMA_OPT_DUM_IDX,    /* Destination address update mode :-                       
                             EDMA_OPT_DUM_NONE - Fixed address mode            
                             EDMA_OPT_DUM_INC  - Increment address mode        
                             EDMA_OPT_DUM_DEC  - Decrement address mode       
                             EDMA_OPT_DUM_IDX  - Address modified by element   
                                                 index or frame Index        */
    
    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
                             EDMA_OPT_TCINT_NO  - Indication disabled      
                             EDMA_OPT_TCINT_YES - Indication enabled         */  
    
    EDMA_OPT_TCC_OF(0xD),/* Transfer complete code                     */
    
    EDMA_OPT_LINK_YES,   /* Linking of event parameters
                             EDMA_OPT_LINK_NO   -  Disabled 
                             EDMA_OPT_LINK_YES  -  Enabled                   */    
    
    EDMA_OPT_FS_NO       /* Frame synchronization
                             EDMA_OPT_FS_NO  - Channel is element/array 
                                                synchronized 
                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
  ),
  EDMA_SRC_OF(0x34000000u),/* Source address register
                               &ping_data    - source address                */
  
  EDMA_CNT_OF(0xFF0002),   /* Transfer count parameter
                                BUFF_SZ      - buffer sizes in # of ints     */      
  
  EDMA_DST_OF(rx_pong),      /* Destination address parameter
                                 ping - destination address                  */ 
  
  EDMA_IDX_OF(0xFE020200),/* Index parameter                                 */
  
  EDMA_RLD_OF(0x00) /* Count reload/link parameter                     */
};

/* Create the EDMA configuration structure for ping_tx transfers */
EDMA_Config cfgEdmaPing_tx = {  
  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT    */ 
  
   EDMA_OPT_PRI_HIGH,    /* Priority levels for EDMA events:
                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer*/
   
   EDMA_OPT_ESIZE_16BIT,/* Element size :-
                             EDMA_OPT_ESIZE_32BIT - 32 bit word 
                             EDMA_OPT_ESIZE_16BIT - 16 bit word 
                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */
   
   EDMA_OPT_2DS_NO,     /* Source dimension :-
                             EDMA_OPT_2DS_NO  - 1-dimensional source    
                             EDMA_OPT_2DS_YES - 2-dimensional source         */  
   
   EDMA_OPT_SUM_IDX,   /* Source address update mode :-           
                             EDMA_OPT_SUM_NONE - Fixed address mode 
                             EDMA_OPT_SUM_INC  - Increment address mode
                             EDMA_OPT_SUM_DEC  - Decrement address mode
                             EDMA_OPT_SUM_IDX  - Address modified by element 
                                                 index or frame Index        */
    EDMA_OPT_2DD_NO,     /* Destination dimension :-  
                             EDMA_OPT_2DD_NO   - 1-dimensional source            
                             EDMA_OPT_2DD_YES  - 2-dimensional source        */
   
    EDMA_OPT_DUM_NONE,    /* Destination address update mode :-                       
                             EDMA_OPT_DUM_NONE - Fixed address mode            
                             EDMA_OPT_DUM_INC  - Increment address mode        
                             EDMA_OPT_DUM_DEC  - Decrement address mode       
                             EDMA_OPT_DUM_IDX  - Address modified by element   
                                                 index or frame Index        */
    
    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
                             EDMA_OPT_TCINT_NO  - Indication disabled      
                             EDMA_OPT_TCINT_YES - Indication enabled         */  
    
    EDMA_OPT_TCC_OF(0xE),/* Transfer complete code                     */
    
    EDMA_OPT_LINK_YES,   /* Linking of event parameters
                             EDMA_OPT_LINK_NO   -  Disabled 
                             EDMA_OPT_LINK_YES  -  Enabled                   */    
    
    EDMA_OPT_FS_NO       /* Frame synchronization
                             EDMA_OPT_FS_NO  - Channel is element/array 
                                                synchronized 
                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
  ),
  EDMA_SRC_OF(tx_ping),/* Source address register 
                               &ping_data    - source address                */
  
  EDMA_CNT_OF(0xFF0002),   /* Transfer count parameter
                                BUFF_SZ      - buffer sizes in # of ints     */      
  
  EDMA_DST_OF(0x34000000u),      /* Destination address parameter
                                 ping - destination address                  */ 
  
  EDMA_IDX_OF(0xFE020200),/* Index parameter                                 */
  
  EDMA_RLD_OF(0x00) /* Count reload/link parameter                     */
};


/* Create the EDMA configuration structure for pong_tx transfers */
EDMA_Config cfgEdmaPong_tx = {  
  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT    */ 
  
   EDMA_OPT_PRI_HIGH,    /* Priority levels for EDMA events:
                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer*/
   
   EDMA_OPT_ESIZE_16BIT,/* Element size :-
                             EDMA_OPT_ESIZE_32BIT - 32 bit word 
                             EDMA_OPT_ESIZE_16BIT - 16 bit word 
                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */
   
   EDMA_OPT_2DS_NO,     /* Source dimension :-
                             EDMA_OPT_2DS_NO  - 1-dimensional source    
                             EDMA_OPT_2DS_YES - 2-dimensional source         */  
   
   EDMA_OPT_SUM_IDX,   /* Source address update mode :-           
                             EDMA_OPT_SUM_NONE - Fixed address mode 
                             EDMA_OPT_SUM_INC  - Increment address mode
                             EDMA_OPT_SUM_DEC  - Decrement address mode
                             EDMA_OPT_SUM_IDX  - Address modified by element 
                                                 index or frame Index        */
    EDMA_OPT_2DD_NO,     /* Destination dimension :-  
                             EDMA_OPT_2DD_NO   - 1-dimensional source            
                             EDMA_OPT_2DD_YES  - 2-dimensional source        */
   
    EDMA_OPT_DUM_NONE,    /* Destination address update mode :-                       
                             EDMA_OPT_DUM_NONE - Fixed address mode            
                             EDMA_OPT_DUM_INC  - Increment address mode        
                             EDMA_OPT_DUM_DEC  - Decrement address mode       
                             EDMA_OPT_DUM_IDX  - Address modified by element   
                                                 index or frame Index        */
    
    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
                             EDMA_OPT_TCINT_NO  - Indication disabled      
                             EDMA_OPT_TCINT_YES - Indication enabled         */  
    
    EDMA_OPT_TCC_OF(0xC),/* Transfer complete code                     */
    
    EDMA_OPT_LINK_YES,   /* Linking of event parameters
                             EDMA_OPT_LINK_NO   -  Disabled 
                             EDMA_OPT_LINK_YES  -  Enabled                   */    
    
    EDMA_OPT_FS_NO       /* Frame synchronization
                             EDMA_OPT_FS_NO  - Channel is element/array 
                                                synchronized 
                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
  ),
  EDMA_SRC_OF(tx_pong),/* Source address register 
                               &ping_data    - source address                */
  
  EDMA_CNT_OF(0xFF0002),   /* Transfer count parameter
                                BUFF_SZ      - buffer sizes in # of ints     */      
  
  EDMA_DST_OF(0x34000000u),      /* Destination address parameter
                                 ping - destination address                  */ 
  
  EDMA_IDX_OF(0xFE020200),/* Index parameter                                 */
  
  EDMA_RLD_OF(0x00) /* Count reload/link parameter                     */
};


/* Create the EDMA configuration structure for QDMA ping transfers */
//---------------------------------------------------------------------------------
	EDMA_Config cfgQdma_ping = {
	  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT     */

	   EDMA_OPT_PRI_LOW,     /* Priority levels for EDMA events:
	                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
	                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer */

	   EDMA_OPT_ESIZE_16BIT, /* Element size :-
	                             EDMA_OPT_ESIZE_32BIT - 32 bit word (float is 32 bits)
	                             EDMA_OPT_ESIZE_16BIT - 16 bit word
	                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */

	   EDMA_OPT_2DS_NO,      /* Source dimension :-
	                             EDMA_OPT_2DS_NO  - 1-dimensional source
	                             EDMA_OPT_2DS_YES - 2-dimensional source         */

	   EDMA_OPT_SUM_INC,     /* Source address update mode :-
	                             EDMA_OPT_SUM_NONE - Fixed address mode
	                             EDMA_OPT_SUM_INC  - Increment address mode
	                             EDMA_OPT_SUM_DEC  - Decrement address mode
	                             EDMA_OPT_SUM_IDX  - Address modified by element
	                                                 index or frame Index        */
	    EDMA_OPT_2DD_NO,     /* Destination dimension :-
	                             EDMA_OPT_2DD_NO   - 1-dimensional source
	                             EDMA_OPT_2DD_YES  - 2-dimensional source        */

	    EDMA_OPT_DUM_INC,    /* Destination address update mode :-
	                             EDMA_OPT_DUM_NONE - Fixed address mode
	                             EDMA_OPT_DUM_INC  - Increment address mode
	                             EDMA_OPT_DUM_DEC  - Decrement address mode
	                             EDMA_OPT_DUM_IDX  - Address modified by element
	                                                 index or frame Index        */

	    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
	                             EDMA_OPT_TCINT_NO  - Indication disabled
	                             EDMA_OPT_TCINT_YES - Indication enabled         */

	    EDMA_OPT_TCC_OF(0x9),/* Transfer complete code                           */

	    EDMA_OPT_LINK_NO,    /* Linking of event parameters
	                             EDMA_OPT_LINK_NO   -  Disabled
	                             EDMA_OPT_LINK_YES  -  Enabled                   */

	    EDMA_OPT_FS_NO       /* Frame synchronization
	                             EDMA_OPT_FS_NO  - Channel is element/array
	                                                synchronized
	                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
	  ),
	  EDMA_SRC_OF(rx_ping),        /* Source address register
	                             &x    - source address                          */

	  EDMA_CNT_OF(BUFLEN*BUFWIDTH), /* Transfer count parameter
	                             BUFFSIZE      - buffer size in # of shorts*/

	  EDMA_DST_OF(tx_ping),        /* Destination address parameter
	                             &y - destination address                        */

	  EDMA_IDX_OF(0),        /* Index parameter
	                             N/A for QDMA		                             */

	  EDMA_RLD_OF(0x00)      /* Count reload/link parameter
 							     N/A for QDMA		                             */
	};


	/* Create the EDMA configuration structure for QDMA ping transfers */
	//---------------------------------------------------------------------------------
		EDMA_Config cfgQdma_pong = {
		  EDMA_OPT_RMK(          /* Making Options parameter register - EDMA_OPT     */

		   EDMA_OPT_PRI_LOW,     /* Priority levels for EDMA events:
		                             EDMA_OPT_PRI_LOW  - Low priority EDMA transfer
		                             EDMA_OPT_PRI_HIGH - High priority EDMA transfer */

		   EDMA_OPT_ESIZE_16BIT, /* Element size :-
		                             EDMA_OPT_ESIZE_32BIT - 32 bit word (float is 32 bits)
		                             EDMA_OPT_ESIZE_16BIT - 16 bit word
		                             EDMA_OPT_ESIZE_8BIT  -  8 bit word              */

		   EDMA_OPT_2DS_NO,      /* Source dimension :-
		                             EDMA_OPT_2DS_NO  - 1-dimensional source
		                             EDMA_OPT_2DS_YES - 2-dimensional source         */

		   EDMA_OPT_SUM_INC,     /* Source address update mode :-
		                             EDMA_OPT_SUM_NONE - Fixed address mode
		                             EDMA_OPT_SUM_INC  - Increment address mode
		                             EDMA_OPT_SUM_DEC  - Decrement address mode
		                             EDMA_OPT_SUM_IDX  - Address modified by element
		                                                 index or frame Index        */
		    EDMA_OPT_2DD_NO,     /* Destination dimension :-
		                             EDMA_OPT_2DD_NO   - 1-dimensional source
		                             EDMA_OPT_2DD_YES  - 2-dimensional source        */

		    EDMA_OPT_DUM_INC,    /* Destination address update mode :-
		                             EDMA_OPT_DUM_NONE - Fixed address mode
		                             EDMA_OPT_DUM_INC  - Increment address mode
		                             EDMA_OPT_DUM_DEC  - Decrement address mode
		                             EDMA_OPT_DUM_IDX  - Address modified by element
		                                                 index or frame Index        */

		    EDMA_OPT_TCINT_YES,  /* Transfer complete interrupt :-
		                             EDMA_OPT_TCINT_NO  - Indication disabled
		                             EDMA_OPT_TCINT_YES - Indication enabled         */

		    EDMA_OPT_TCC_OF(0x8),/* Transfer complete code                           */

		    EDMA_OPT_LINK_NO,    /* Linking of event parameters
		                             EDMA_OPT_LINK_NO   -  Disabled
		                             EDMA_OPT_LINK_YES  -  Enabled                   */

		    EDMA_OPT_FS_NO       /* Frame synchronization
		                             EDMA_OPT_FS_NO  - Channel is element/array
		                                                synchronized
		                             EDMA_OPT_FS_YES -  Channel is frame synchronized*/
		  ),
		  EDMA_SRC_OF(rx_pong),        /* Source address register
		                             &x    - source address                          */

		  EDMA_CNT_OF(BUFLEN*BUFWIDTH), /* Transfer count parameter
		                             BUFFSIZE      - buffer size in # of shorts*/

		  EDMA_DST_OF(tx_pong),        /* Destination address parameter
		                             &y - destination address                        */

		  EDMA_IDX_OF(0),        /* Index parameter
		                             N/A for QDMA		                             */

		  EDMA_RLD_OF(0x00)      /* Count reload/link parameter
	 							     N/A for QDMA		                             */
		};


#endif /*230_BIOS_DUAL_MIKES_APP_EDMA_H_*/
