################## SETUP  COMPILER ##################
CC          = sdcc
LD          = sdld
AR          = sdar
AS          = sdasstm8
OBJCOPY     = sdobjcopy
SIZE        = size
MAKE        = make

DEBUG       = 1

TARGET      = main
STDLIB      = STM8S_StdPeriph_Lib/Libraries/STM8S_StdPeriph_Driver
INCLUDEDIR  = $(STDLIB)/inc Inc
LIBSRCDIR   = $(STDLIB)/src
#LIBSRC      = $(LIBSRCDIR)/stm8s_adc1.c  $(LIBSRCDIR)/stm8s_awu.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_beep.c  $(LIBSRCDIR)/stm8s_clk.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_exti.c  $(LIBSRCDIR)/stm8s_flash.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_gpio.c  $(LIBSRCDIR)/stm8s_i2c.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_itc.c   $(LIBSRCDIR)/stm8s_iwdg.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_rst.c   $(LIBSRCDIR)/stm8s_spi.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_tim1.c  $(LIBSRCDIR)/stm8s_tim2.c
#LIBSRC     += $(LIBSRCDIR)/stm8s_uart2.c $(LIBSRCDIR)/stm8s_wwdg.c
LIBSRC     += $(LIBSRCDIR)/stm8s_clk.c   $(LIBSRCDIR)/stm8s_uart2.c
LIBSRC     += $(LIBSRCDIR)/stm8s_gpio.c  $(LIBSRCDIR)/stm8s_tim2.c
LIBSRC     += $(SRCS)/stm8s_it.c         $(SRCS)/uart.c
LIBSRC     += $(SRCS)/tim.c              $(SRCS)/one-wire.c
LIBSRC     += $(SRCS)/DS18x20.c          $(LIBSRCDIR)/stm8s_itc.c

SRCS        = Src
OBJS        = $(LIBSRC:.c=.rel)

MCU         = STM8S105
COMPILER    = __SDCC__

ifeq ($(DEBUG), 1)
DEBUGOPTS   = --debug --all-callee-saves --debug --verbose --stack-auto --fverbose-asm  --float-reent --no-peep
endif

DEFINES    += -D$(COMPILER) -DUSE_STDPERIPH_DRIVER


CFLAGS      = -mstm8 $(DEFINES) --opt-code-speed
#CFLAGS     += --out-fmt-elf --all-callee-saves --debug --verbose --stack-auto --fverbose-asm
#CFLAGS     += --float-reent --no-peep
LDFLAGS     = $(addprefix -I ,$(INCLUDEDIR))

BUILD_DIR   = Build

IHX         = $(BUILD_DIR)/$(TARGET).ihx
ELF         = $(BUILD_DIR)/$(TARGET).elf


################### BUILD PROCESS ###################
.PHONY: all build clean flash

all: build

build: $(OBJS) $(ELF) $(IHX)

$(OBJS):

%.rel: %.c
	@echo
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/ $<

$(ELF): $(SRCS)/$(TARGET).c
	@echo
	$(CC) $(CFLAGS) $(DEBUGOPTS) --out-fmt-elf $(LDFLAGS) -o $(BUILD_DIR)/ $< \
                                                           $(BUILD_DIR)/stm8s_clk.rel \
                                                           $(BUILD_DIR)/stm8s_gpio.rel \
                                                           $(BUILD_DIR)/stm8s_uart2.rel \
                                                           $(BUILD_DIR)/stm8s_tim2.rel \
                                                           $(BUILD_DIR)/stm8s_itc.rel \
                                                           $(BUILD_DIR)/stm8s_it.rel \
                                                           $(BUILD_DIR)/uart.rel \
                                                           $(BUILD_DIR)/tim.rel \
                                                           $(BUILD_DIR)/one-wire.rel \
                                                           $(BUILD_DIR)/DS18x20.rel
	$(SIZE) $@

$(IHX): $(SRCS)/$(TARGET).c
#	mkdir -p $(BUILD_DIR)
#	$(CC) -c $(CFLAGS) -I$(INCLUDEDIR) -I. -L$(LIBSRCDIR) $(DEFINES) -o $(BUILD_DIR)/ $(LIBSRC)
#	$(CC) -c $(CFLAGS) $(LDFLAGS) $(LIBSRCDIR)/stm8s_beep.c -o $(BUILD_DIR)/
#	$(CC) -c $(CFLAGS) $(LDFLAGS) $(LIBSRCDIR)/stm8s_gpio.c -o $(BUILD_DIR)/
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/ $< $(BUILD_DIR)/*.rel
	@echo
	$(CC) $(CFLAGS) --out-fmt-ihx $(LDFLAGS) -o $(BUILD_DIR)/ $< \
	                                            $(BUILD_DIR)/stm8s_clk.rel \
	                                            $(BUILD_DIR)/stm8s_gpio.rel \
	                                            $(BUILD_DIR)/stm8s_uart2.rel \
	                                            $(BUILD_DIR)/stm8s_tim2.rel \
	                                            $(BUILD_DIR)/stm8s_itc.rel \
	                                            $(BUILD_DIR)/stm8s_it.rel \
	                                            $(BUILD_DIR)/uart.rel \
	                                            $(BUILD_DIR)/tim.rel \
	                                            $(BUILD_DIR)/one-wire.rel \
	                                            $(BUILD_DIR)/DS18x20.rel
	$(SIZE) $@

clean:
	rm -rf $(BUILD_DIR)/*

flash: $(IHX)
	stm8flash -c stlinkv2 -p stm8s105k4 -s flash -w $<

########################################################################
# Expandsion Note:
#
# if you wanna use %f with printf (sprintf, vprintf...), you can rebuild
# printf_large.c file in the following way:
#
# 1. Download SDCC source code from https://sourceforge.net/projects/sdcc/files/sdcc/
#
# 2. Extract file has just been downloaded
#
# 3. Build according to the project guidelines (http://sdcc.sourceforge.net/doc/sdccman.pdf, chapter 2)
#
# 4. Do not install immediately. Go to <SDCC-src-dir>/device/lib/stm8 and run the following line
#    ../../../bin/sdcc -mstm8 -I./../../include -I. --std-c99 -DUSE_FLOATS=1 -c ../printf_large.c
#
# 5. Install with ```sudo make install```
#
# Caution: STM8S105C4/S4/K4 may not be enough flash
