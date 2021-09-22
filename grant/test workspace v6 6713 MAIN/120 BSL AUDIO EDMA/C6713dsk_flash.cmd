/*C6713dsk_flash.cmd  Linker command file for flash*/

MEMORY
{
  VECS:	    org = 0h,  len =      0x200 
  BOOT:     org = 0x00000200, len = 0x200  													
  IRAM:     org = 0x00000400, len = 0x00007C00
  IRAM2:    org = 0x00008000, len = 0x0002C000
  SRAM_EXT: org = 0x80000000, len = 0x01000000
  FLASH:    org = 0x90000000, len = 0x00020000                     
}

SECTIONS
{
  "bootload" :> BOOT  /*boot section.Runs when C6713 starts*/
  .vecs      :> VECS		
  .text      :> IRAM		
  .bss       :> IRAM
  .cinit     :> IRAM
  .stack     :> IRAM2
  .sysmem    :> IRAM2
  .const     :> IRAM
  .switch    :> IRAM
  .far     :> IRAM2
  .cio     :> IRAM
  .csldata :> IRAM
  .buffers :> IRAM2
  .offchip  :> SRAM_EXT
}
