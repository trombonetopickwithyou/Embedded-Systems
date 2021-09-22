################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
emptycfg.cmd: ../empty.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti2/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti2/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emptycfg.s??: | emptycfg.cmd
emptycfg_c.c: | emptycfg.cmd
emptycfg.h: | emptycfg.cmd
emptycfg.h??: | emptycfg.cmd
empty.cdb: | emptycfg.cmd

emptycfg.obj: ./emptycfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/500 BIOS empty/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="emptycfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

emptycfg_c.obj: ./emptycfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/500 BIOS empty/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="emptycfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/500 BIOS empty/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


