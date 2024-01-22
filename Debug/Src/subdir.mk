################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/GC9A01.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/trident_fpu_f4.c \
../Src/trident_gpio_f4.c \
../Src/trident_spi_f4.c \
../Src/trident_timebase.c \
../Src/trident_uart_f4.c 

OBJS += \
./Src/GC9A01.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/trident_fpu_f4.o \
./Src/trident_gpio_f4.o \
./Src/trident_spi_f4.o \
./Src/trident_timebase.o \
./Src/trident_uart_f4.o 

C_DEPS += \
./Src/GC9A01.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/trident_fpu_f4.d \
./Src/trident_gpio_f4.d \
./Src/trident_spi_f4.d \
./Src/trident_timebase.d \
./Src/trident_uart_f4.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401CCUx -DSTM32F401xC -DSTM32 -DSTM32F4 -c -I../Inc -I"C:/Users/User/Documents/Code_Library/Trident_Repo/GC9A01_Display/chip_headers" -I"C:/Users/User/Documents/Code_Library/Trident_Repo/GC9A01_Display/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/User/Documents/Code_Library/Trident_Repo/GC9A01_Display/chip_headers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/GC9A01.cyclo ./Src/GC9A01.d ./Src/GC9A01.o ./Src/GC9A01.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/trident_fpu_f4.cyclo ./Src/trident_fpu_f4.d ./Src/trident_fpu_f4.o ./Src/trident_fpu_f4.su ./Src/trident_gpio_f4.cyclo ./Src/trident_gpio_f4.d ./Src/trident_gpio_f4.o ./Src/trident_gpio_f4.su ./Src/trident_spi_f4.cyclo ./Src/trident_spi_f4.d ./Src/trident_spi_f4.o ./Src/trident_spi_f4.su ./Src/trident_timebase.cyclo ./Src/trident_timebase.d ./Src/trident_timebase.o ./Src/trident_timebase.su ./Src/trident_uart_f4.cyclo ./Src/trident_uart_f4.d ./Src/trident_uart_f4.o ./Src/trident_uart_f4.su

.PHONY: clean-Src

