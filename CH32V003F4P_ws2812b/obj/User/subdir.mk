################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v00x_it.c \
../User/main.c \
../User/system_ch32v00x.c \
../User/ws2812.c 

C_DEPS += \
./User/ch32v00x_it.d \
./User/main.d \
./User/system_ch32v00x.d \
./User/ws2812.d 

OBJS += \
./User/ch32v00x_it.o \
./User/main.o \
./User/system_ch32v00x.o \
./User/ws2812.o 

DIR_OBJS += \
./User/*.o \

DIR_DEPS += \
./User/*.d \

DIR_EXPANDS += \
./User/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/Debug" -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/Core" -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/User" -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/Peripheral/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

