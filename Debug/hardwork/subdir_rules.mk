################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
hardwork/%.o: ../hardwork/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"D:/TI/ccs/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/TI/TI_Sum/workspace_ccstheia/NEW_24YEARH" -I"D:/TI/TI_Sum/workspace_ccstheia/NEW_24YEARH/hardwork" -I"D:/TI/TI_Sum/workspace_ccstheia/NEW_24YEARH/Debug" -I"C:/TI/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_11_00_07/source" -g -Wall -MMD -MP -MF"hardwork/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


