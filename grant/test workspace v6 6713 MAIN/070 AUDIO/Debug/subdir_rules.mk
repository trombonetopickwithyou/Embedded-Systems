################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti2/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="D:/Embedded/Homework_2/070 AUDIO/libraries/dsk6713/c6000/bios/include" --include_path="D:/Embedded/Homework_2/070 AUDIO/libraries/dsk6713/c6000/dsk6713/include" --include_path="D:/Embedded/Homework_2/070 AUDIO/libraries/dsk6713/c6000/dsk6713/lib" --include_path="D:/Embedded/Homework_2/070 AUDIO/libraries/dsk6713/c6000/bios/lib" --define=c6713 --define=CHIP_6713 --diag_wrap=off --diag_warning=225 --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


