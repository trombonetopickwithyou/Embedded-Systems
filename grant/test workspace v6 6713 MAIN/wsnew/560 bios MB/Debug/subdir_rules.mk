################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
mailbox.obj: ../mailbox.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/dsk6713/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/560 bios MB/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --display_error_number --issue_remarks --verbose_diagnostics --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="mailbox.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mbcfg.cmd: ../mb.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti2/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti2/bios_5_42_01_09/packages;" -js -w "$<"
	@echo 'Finished building: $<'
	@echo ' '

mbcfg.s??: | mbcfg.cmd
mbcfg_c.c: | mbcfg.cmd
mbcfg.h: | mbcfg.cmd
mbcfg.h??: | mbcfg.cmd
mb.cdb: | mbcfg.cmd

mbcfg.obj: ./mbcfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/dsk6713/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/560 bios MB/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --display_error_number --issue_remarks --verbose_diagnostics --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="mbcfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mbcfg_c.obj: ./mbcfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/dsk6713/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/560 bios MB/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --display_error_number --issue_remarks --verbose_diagnostics --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="mbcfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


