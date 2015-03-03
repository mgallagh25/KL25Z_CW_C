################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Headers/arm_cm0.c" \
"../Project_Headers/mcg.c" \

C_SRCS += \
../Project_Headers/arm_cm0.c \
../Project_Headers/mcg.c \

OBJS += \
./Project_Headers/arm_cm0_c.obj \
./Project_Headers/mcg_c.obj \

C_DEPS += \
./Project_Headers/arm_cm0.d \
./Project_Headers/mcg.d \

OBJS_QUOTED += \
"./Project_Headers/arm_cm0_c.obj" \
"./Project_Headers/mcg_c.obj" \

C_DEPS_QUOTED += \
"./Project_Headers/arm_cm0.d" \
"./Project_Headers/mcg.d" \

OBJS_OS_FORMAT += \
./Project_Headers/arm_cm0_c.obj \
./Project_Headers/mcg_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Project_Headers/arm_cm0_c.obj: ../Project_Headers/arm_cm0.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Project_Headers/arm_cm0.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -g3 -gdwarf-2 -gstrict-dwarf -o"Project_Headers/arm_cm0_c.obj"
	@echo 'Finished building: $<'
	@echo ' '

Project_Headers/mcg_c.obj: ../Project_Headers/mcg.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Project_Headers/mcg.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -g3 -gdwarf-2 -gstrict-dwarf -o"Project_Headers/mcg_c.obj"
	@echo 'Finished building: $<'
	@echo ' '


