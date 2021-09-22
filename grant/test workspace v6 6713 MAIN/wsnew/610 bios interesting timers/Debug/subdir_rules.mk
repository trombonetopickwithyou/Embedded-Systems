################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
debugcfg.cmd: ../debug.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti2/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti2/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

debugcfg.s??: | debugcfg.cmd
debugcfg_c.c: | debugcfg.cmd
debugcfg.h: | debugcfg.cmd
debugcfg.h??: | debugcfg.cmd
debug.cdb: | debugcfg.cmd

debugcfg.obj: ./debugcfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/610 bios interesting timers/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="D:/Program Files/ti/C6xCSL/include" --include_path="D:/Program Files/ti/DSK6713/c6000/dsk6713/include" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --preproc_with_compile --preproc_dependency="debugcfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

debugcfg_c.obj: ./debugcfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/610 bios interesting timers/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="D:/Program Files/ti/C6xCSL/include" --include_path="D:/Program Files/ti/DSK6713/c6000/dsk6713/include" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --preproc_with_compile --preproc_dependency="debugcfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/610 bios interesting timers/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="D:/Program Files/ti/C6xCSL/include" --include_path="D:/Program Files/ti/DSK6713/c6000/dsk6713/include" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


