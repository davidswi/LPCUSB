LIBNAME	= usbstack

# Package definitions
PKG_NAME	= target
DATE		= $$(date +%Y%m%d)

# Tool definitions
TGT		= arm-none-eabi-
CC      = $(TGT)gcc
LD      = $(TGT)ld -v
AR      = $(TGT)ar
AS      = $(TGT)as
CP      = $(TGT)objcopy
OD		= $(TGT)objdump
RM		= rm
TAR		= tar

CFLAGS  = -I./ -I../ -c -W -Wall -Os -g -DDEBUG -mcpu=arm7tdmi
ARFLAGS = -rcs

LIBSRCS = usbhw_lpc.c usbcontrol.c usbstdreq.c usbinit.c
LIBOBJS = $(LIBSRCS:.c=.o)

all: depend lib examples i2c

clean:
	$(RM) -f $(LIBNAME).a $(LIBOBJS) .depend
	make -C examples clean
	make -C HIDI2C clean

examples:
	make -C examples

i2c:
	make -C HIDI2C

# build lib
lib: $(LIBNAME).a

$(LIBNAME).a: $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

# Builds release tar file
dist: clean
	cd .. && $(TAR) --exclude={CVS,cvs,.svn} -cvzf $(PKG_NAME)-$(DATE).tar.gz $(PKG_NAME)

# recompile if the Makefile changes
$(LIBOBJS): Makefile

# dependency checking
depend: $(LIBSRCS)
	$(CC) $(CFLAGS) -MM $^ > .depend || rm -f .depend

# phony targets
.PHONY: all clean examples depend

-include .depend
