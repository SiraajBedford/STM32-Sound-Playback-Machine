################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
E-Design\ 314.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
E-Design\ 314.list \

OBJCOPY_BIN += \
E-Design\ 314.bin \


# All Target
all: E-Design\ 314.elf secondary-outputs

# Tool invocations
E-Design\ 314.elf: $(OBJS) $(USER_OBJS) C:\Users\User\ 1\STM32CubeIDE\workspace_1.2.0\CloneFolder\E-Design\ 314\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "E-Design 314.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\User 1\STM32CubeIDE\workspace_1.2.0\CloneFolder\E-Design 314\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="E-Design 314.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

E-Design\ 314.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "E-Design 314.list"
	@echo 'Finished building: $@'
	@echo ' '

E-Design\ 314.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "E-Design 314.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
