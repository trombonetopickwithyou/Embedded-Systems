################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
QDMAcfg.cmd: ../QDMA.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti2/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti2/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

QDMAcfg.s??: | QDMAcfg.cmd
QDMAcfg_c.c: | QDMAcfg.cmd
QDMAcfg.h: | QDMAcfg.cmd
QDMAcfg.h??: | QDMAcfg.cmd
QDMA.cdb: | QDMAcfg.cmd

QDMAcfg.obj: ./QDMAcfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/620 BIOS QDMA/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/Fastlib/inc" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="QDMAcfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

QDMAcfg_c.obj: ./QDMAcfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/620 BIOS QDMA/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/Fastlib/inc" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="QDMAcfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dspfft.obj: ../dspfft.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/620 BIOS QDMA/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/Fastlib/inc" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="dspfft.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS6.0/wsnew/620 BIOS QDMA/Debug" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti2/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/cslupdate/include" --include_path="C:/Documents and Settings/One/Desktop/DT Docs/CCS/libraries/Fastlib/inc" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


