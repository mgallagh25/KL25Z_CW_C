################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/elt3050_3d.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/elt3050_3d.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/elt3050_3d_c.obj \
./Sources/sa_mtb_c.obj \

C_DEPS += \
./Sources/elt3050_3d.d \
./Sources/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/elt3050_3d_c.obj" \
"./Sources/sa_mtb_c.obj" \

C_DEPS_QUOTED += \
"./Sources/elt3050_3d.d" \
"./Sources/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/elt3050_3d_c.obj \
./Sources/sa_mtb_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/elt3050_3d_c.obj: ../Sources/elt3050_3d.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/elt3050_3d.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -g3 -gdwarf-2 -gstrict-dwarf -o"Sources/elt3050_3d_c.obj"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb_c.obj: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -g3 -gdwarf-2 -gstrict-dwarf -o"Sources/sa_mtb_c.obj"
	@echo 'Finished building: $<'
	@echo ' '


