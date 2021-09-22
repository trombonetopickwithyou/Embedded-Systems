/*************************************************************************
* $RCSfile: c6713dsk.cmd,v $
* $Revision: 1.3 $
* $Date: 2000/12/16 06:25:41 $
* Copyright (c) 1997 Texas Instruments Incorporated
*
* Description: 
*	Sample Linker Control File for C6713 DSK target boards.
*
* Usage:  
*	lnk6x <obj files...>    -o <out file> -m <map file> lcf.cmd
*	cl6x  <src files...> -z -o <out file> -m <map file> lcf.cmd
*************************************************************************/

MEMORY
{
	PMEM:	o=00000000h	l=00010000h 	/* Internal RAM (L2) mem	*/
	BMEM:	o=80000000h	l=01000000h 	/* CE0, SDRAM, 16 MBytes	*/

}

SECTIONS
{
    .text		>	BMEM
    .rtdx_text	>	BMEM
    .far		>	BMEM
    .stack		>	BMEM
    .bss		>	BMEM
    .cinit		>	BMEM
    .pinit		>	PMEM
    .cio		>	BMEM
    .const		>	BMEM
    .data		>	BMEM
    .rtdx_data	>	BMEM
    .switch		>	BMEM
    .sysmem		>	BMEM
}
