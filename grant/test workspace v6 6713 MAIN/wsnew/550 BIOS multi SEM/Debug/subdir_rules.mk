################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
sem.obj: ../sem.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/550 BIOS multi SEM/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/dsk6713/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="sem.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

semaphorecfg.cmd: ../semaphore.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti2/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti2/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

semaphorecfg.s??: | semaphorecfg.cmd
semaphorecfg_c.c: | semaphorecfg.cmd
semaphorecfg.h: | semaphorecfg.cmd
semaphorecfg.h??: | semaphorecfg.cmd
semaphore.cdb: | semaphorecfg.cmd

semaphorecfg.obj: ./semaphorecfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/550 BIOS multi SEM/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/dsk6713/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="semaphorecfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

semaphorecfg_c.obj: ./semaphorecfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/550 BIOS multi SEM/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/dsk6713/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="semaphorecfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


