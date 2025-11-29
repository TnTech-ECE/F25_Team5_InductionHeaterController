################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/delay.c \
../src/flow_sensor.c \
../src/lcd.c \
../src/max6675.c \
../src/rotary_encoder.c \
../src/run.c \
../src/save.c 

OBJS += \
./src/delay.o \
./src/flow_sensor.o \
./src/lcd.o \
./src/max6675.o \
./src/rotary_encoder.o \
./src/run.o \
./src/save.o 

C_DEPS += \
./src/delay.d \
./src/flow_sensor.d \
./src/lcd.d \
./src/max6675.d \
./src/rotary_encoder.d \
./src/run.d \
./src/save.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/mrpat/OneDrive/Documents/Github/F25_Team5_InductionHeaterController/Software/embedded_system/src" -I"C:/Users/mrpat/OneDrive/Documents/Github/F25_Team5_InductionHeaterController/Software/embedded_system/Middlewares" -I"C:/Users/mrpat/OneDrive/Documents/Github/F25_Team5_InductionHeaterController/Software/embedded_system/Middlewares/Third_Party/FatFs/src" -I"C:/Users/mrpat/OneDrive/Documents/Github/F25_Team5_InductionHeaterController/Software/embedded_system/Middlewares/Third_Party/FatFs/src/option" -I"C:/Users/mrpat/OneDrive/Documents/Github/F25_Team5_InductionHeaterController/Software/embedded_system/FATFS/App" -I"C:/Users/mrpat/OneDrive/Documents/Github/F25_Team5_InductionHeaterController/Software/embedded_system/FATFS/Target" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/delay.cyclo ./src/delay.d ./src/delay.o ./src/delay.su ./src/flow_sensor.cyclo ./src/flow_sensor.d ./src/flow_sensor.o ./src/flow_sensor.su ./src/lcd.cyclo ./src/lcd.d ./src/lcd.o ./src/lcd.su ./src/max6675.cyclo ./src/max6675.d ./src/max6675.o ./src/max6675.su ./src/rotary_encoder.cyclo ./src/rotary_encoder.d ./src/rotary_encoder.o ./src/rotary_encoder.su ./src/run.cyclo ./src/run.d ./src/run.o ./src/run.su ./src/save.cyclo ./src/save.d ./src/save.o ./src/save.su

.PHONY: clean-src

