################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
josh_semcfg.cmd: ../josh_sem.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"c:/ti/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="c:/ti/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

josh_semcfg.s??: | josh_semcfg.cmd
josh_semcfg_c.c: | josh_semcfg.cmd
josh_semcfg.h: | josh_semcfg.cmd
josh_semcfg.h??: | josh_semcfg.cmd
josh_sem.cdb: | josh_semcfg.cmd

josh_semcfg.obj: ./josh_semcfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="D:/Embedded/libraries/dsk6713/include" --include_path="D:/Embedded/libraries/bios/include" --include_path="D:/Embedded/Projects/Homework_7/HW7/Debug" --include_path="c:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="c:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --diag_wrap=off --diag_warning=225 --display_error_number --mem_model:data=far --preproc_with_compile --preproc_dependency="josh_semcfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

josh_semcfg_c.obj: ./josh_semcfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="D:/Embedded/libraries/dsk6713/include" --include_path="D:/Embedded/libraries/bios/include" --include_path="D:/Embedded/Projects/Homework_7/HW7/Debug" --include_path="c:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="c:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --diag_wrap=off --diag_warning=225 --display_error_number --mem_model:data=far --preproc_with_compile --preproc_dependency="josh_semcfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="D:/Embedded/libraries/dsk6713/include" --include_path="D:/Embedded/libraries/bios/include" --include_path="D:/Embedded/Projects/Homework_7/HW7/Debug" --include_path="c:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="c:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --diag_wrap=off --diag_warning=225 --display_error_number --mem_model:data=far --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


