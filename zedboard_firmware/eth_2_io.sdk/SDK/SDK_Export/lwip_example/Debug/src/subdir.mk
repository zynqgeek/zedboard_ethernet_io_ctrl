################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/echo.c \
../src/main.c \
../src/platform.c \
../src/platform_mb.c \
../src/platform_ppc.c \
../src/platform_zynq.c \
../src/zed_pmod_io.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/echo.o \
./src/main.o \
./src/platform.o \
./src/platform_mb.o \
./src/platform_ppc.o \
./src/platform_zynq.o \
./src/zed_pmod_io.o 

C_DEPS += \
./src/echo.d \
./src/main.d \
./src/platform.d \
./src/platform_mb.d \
./src/platform_ppc.d \
./src/platform_zynq.d \
./src/zed_pmod_io.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: ARM gcc compiler
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../lwip_example_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


