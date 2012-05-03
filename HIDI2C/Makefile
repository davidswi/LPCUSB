LIBNAME	= ../usbstack

# Tool definitions
TGT		= arm-none-eabi-
CC      = $(TGT)gcc
LD      = $(TGT)ld -v
AR      = $(TGT)ar
AS      = $(TGT)as
CP      = $(TGT)objcopy
OD		= $(TGT)objdump
RM		= rm

# Tool flags
CFLAGS  = -I./ -I../ -c -W -Wall -Os -g -DDEBUG -mcpu=arm7tdmi
ASFLAGS = -ahls -mapcs-32 -o crt.o
LFLAGS  =  -nostartfiles
CPFLAGS = -O ihex
ODFLAGS	= -x --syms

LINKFILE = ../examples/lpc2148-rom.ld
CSRCS	= i2cdriver.c hidi2c.c hidi2c_descriptors.c
OBJS 	= ../examples/crt.o ../examples/printf.o ../examples/console.o ../examples/startup.o ../examples/armVIC.o $(CSRCS:.c=.o)

FIRMWARE = driver
all: depend $(FIRMWARE)

driver: $(OBJS) i2cdriver.o hidi2c.o hidi2c_descriptors.o $(LIBNAME).a

$(FIRMWARE):
	$(CC) -T $(LINKFILE) $(LFLAGS) $^ -o $@.elf -Wl,-Map,$@.map
	$(CP) $(CPFLAGS) $@.elf $@.hex
	$(OD) $(ODFLAGS) $@.elf > $@.dmp

clean:
	rm -f *.hex *.elf *.o *.lst *.dmp *.map .depend

# recompile if the Makefile changes
$(OBJS): Makefile

# dependency checking
depend: $(CSRCS)
	$(CC) $(CFLAGS) -MM $^ > .depend || rm -f .depend

# phony targets
.PHONY: clean

-include .depend
