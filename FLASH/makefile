################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 7
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm -rf"

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Project_Headers/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
KL25Z_C.elf \

EXECUTABLES_QUOTED += \
"KL25Z_C.elf" \

EXECUTABLES_OS_FORMAT += \
KL25Z_C.elf \


# All Target
all: KL25Z_C.elf secondary-outputs

# Tool invocations
KL25Z_C.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MKL25Z128_flash.ld
	@echo 'Building target: $@'
	@echo 'Executing target #8 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" $(OBJS_QUOTED) $(USER_OBJS_QUOTED) $(LIBS_QUOTED) -T"$(ProjDirPath)/Project_Settings/Linker_Files/MKL25Z128_flash.ld" -Xlinker --gc-sections -L"$(MCUToolsBaseDirEnv)/ARM_GCC_Support/ewl/lib/armv6-m" -Wl,-Map,KL25Z_C.map -n -specs=ewl_c.specs -Xlinker --undefined=__pformatter_ -Xlinker --defsym=__pformatter=__pformatter_ -Xlinker --undefined=__sformatter -Xlinker --defsym=__sformatter=__sformatter -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -g3 -gdwarf-2 -gstrict-dwarf -o"KL25Z_C.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) "./*/*.obj" "./*/*/*.obj"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*.d" "./*/*/*.d"  $(ProjDirPath)/FLASH/KL25Z_C.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
