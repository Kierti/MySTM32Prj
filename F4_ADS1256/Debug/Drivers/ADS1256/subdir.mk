################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ADS1256/ADS1256.c 

OBJS += \
./Drivers/ADS1256/ADS1256.o 

C_DEPS += \
./Drivers/ADS1256/ADS1256.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ADS1256/%.o Drivers/ADS1256/%.su: ../Drivers/ADS1256/%.c Drivers/ADS1256/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/F4_ADS1256/Drivers/ADS1256" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ADS1256

clean-Drivers-2f-ADS1256:
	-$(RM) ./Drivers/ADS1256/ADS1256.d ./Drivers/ADS1256/ADS1256.o ./Drivers/ADS1256/ADS1256.su

.PHONY: clean-Drivers-2f-ADS1256

