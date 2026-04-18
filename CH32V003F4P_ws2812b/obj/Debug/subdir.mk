################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

C_DEPS += \
./Debug/debug.d 

OBJS += \
./Debug/debug.o 

DIR_OBJS += \
./Debug/*.o \

DIR_DEPS += \
./Debug/*.d \

DIR_EXPANDS += \
./Debug/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/Debug" -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/Core" -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/User" -I"c:/Users/toppi/OneDrive/Professional Developement/ws2812bWCH/CH32V003F4P_ws2812b/Peripheral/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

