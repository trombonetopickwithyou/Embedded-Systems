/*
 * common.h
 *
 *  Created on: Oct 28, 2015
 *      Author: bnutter
 */

#ifndef BIOS_DUAL_MIKES_COMMON_H_
#define BIOS_DUAL_MIKES_COMMON_H_

#include <std.h>
#include <log.h>
#include <prd.h>
#include <hwi.h>
#include <swi.h>

//-----------------------------------------------------------------------
#include "biosedmacfg.h"
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713.h"
#include "dsk6713.h"
#include "dsk6713_dip.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file
#include "csl_edma.h"
#include "app_codec.h"
#include "c67fastMath.h"

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#define BUFLEN 0x100				  // EDMA config numbers must be changed to accommodate change in BUFLEN
#define BUFWIDTH 2
#define LEFT  1                  //data structure for union of 32-bit data
#define RIGHT 0                  //into two 16-bit data
//-----------------------------------------------------------------------
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;


//-----------------------------------------------------------------------
#define PI 3.14159265358979323846264338327950288419716939937510582
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
#pragma DATA_SECTION (rx_ping, ".sysdata");
#pragma DATA_ALIGN   (rx_ping, 0x80);
short rx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (rx_pong, ".sysdata");
#pragma DATA_ALIGN   (rx_pong, 0x80);
short rx_pong[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_ping, ".sysdata");
#pragma DATA_ALIGN   (tx_ping, 0x80);
short tx_ping[BUFWIDTH][BUFLEN];

#pragma DATA_SECTION (tx_pong, ".sysdata");
#pragma DATA_ALIGN   (tx_pong, 0x80);
short tx_pong[BUFWIDTH][BUFLEN];

//-----------------------------------------------------------------------
#pragma DATA_ALIGN (TWIDDLE256, 0x10)
int TWIDDLE256[(BUFLEN * 5)/4];        	// cos is 90 degrees lag from sin

#pragma DATA_ALIGN (BITREV256, 0x10)
BYTE BITREV256[BUFLEN];					// precalculated table that swaps address bits

#pragma DATA_ALIGN (DATAREAL, 0x10)
int DATAREAL[BUFWIDTH][BUFLEN];					// input-real LR

#pragma DATA_ALIGN (DATAIMAG, 0x10)
int DATAIMAG[BUFWIDTH][BUFLEN];					// input-imag LR

#pragma DATA_ALIGN (TRANSREAL, 0x10)
int TRANSREAL[BUFWIDTH][BUFLEN];				// working-real LR

#pragma DATA_ALIGN (TRANSIMAG, 0x10)
int TRANSIMAG[BUFWIDTH][BUFLEN];				// working-imag LR

#pragma DATA_ALIGN (XDATAREAL, 0x10)
int XDATAREAL[BUFLEN];					// output-real LR

#pragma DATA_ALIGN (XDATAIMAG, 0x10)
int XDATAIMAG[BUFLEN];					// output-imag LR


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

//-------------------------------------------------------------------
// declare the CSL objects
EDMA_Handle hEdma_rx;     // Handle for the EDMA channel
EDMA_Handle hEdmaPing_rx; // Handle for the ping EDMA reload parameters
EDMA_Handle hEdmaPong_rx; // Handle for the pong EDMA reload parameters
EDMA_Config cfgEdma_rx;   //EDMA configuration structure

EDMA_Handle hEdma_tx;
EDMA_Handle hEdmaPing_tx;
EDMA_Handle hEdmaPong_tx;
EDMA_Config cfgEdma_tx;

#include "app_edma.h"      // structures are filled in the header
//-------------------------------------------------------------------

//-----------------------------------------------------------------------
void init(void);
void main(void);
void output_sample(int out_data);
Uint32 input_sample(void);
void InitializeFFT(void);
void MoveData(short data[BUFWIDTH][BUFLEN]);
void dfft(int *x, int *y, int *wcos, int *wsin, int n, int m);		// this is the ASM integer fft function

//-----------------------------------------------------------------------



#endif /* BIOS_DUAL_MIKES_COMMON_H_ */
