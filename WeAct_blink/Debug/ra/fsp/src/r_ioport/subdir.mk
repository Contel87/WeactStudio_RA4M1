################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/r_ioport/r_ioport.c 

C_DEPS += \
./ra/fsp/src/r_ioport/r_ioport.d 

OBJS += \
./ra/fsp/src/r_ioport/r_ioport.o 

SREC += \
WeAct_blink.srec 

MAP += \
WeAct_blink.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/r_ioport/%.o: ../ra/fsp/src/r_ioport/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/src" -I"." -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra/fsp/inc" -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra/fsp/inc/api" -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra/fsp/inc/instances" -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra_gen" -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra_cfg/fsp_cfg/bsp" -I"C:/Users/conte/e2_studio/workspace/WeAct_blink/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"
