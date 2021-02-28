##############################################################################
#   MAKEFILE - this file is part of Binary EYE project (BEYE)                #
##############################################################################
#   Copyrights:           1998, 2000 Nickols_K                               #
#   License:              See below                                          #
#   Author and developer: Nickols_K                                          #
#   Requirement:          GNU make                                           #
#   Original file name:   makefile                                           #
####################### [ D e s c r i p t i o n ] ############################
#  This file is script for make utility of GNU development system.           #
########################### [ L i c e n c e ] ################################
# The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.                     #
# All rights reserved.                                                       #
# This software is redistributable under the licence given in the file       #
# "Licence" distributed in the BEYE archive.                                 #
##############################################################################
HOST_CFLAGS=
HOST_LDFLAGS=

include ../config.mak

SUBDIRS=libbeye

DO_MAKE = @ for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit; done
DO_ALL  = @ for i in $(SUBDIRS); do $(MAKE) -C $$i all || exit; done

CFLAGS = -I../hlp $(CDEFOS) $(CDEFSYS) $(HOST_CFLAGS)
LDFLAGS = $(OSLDEF) $(HOST_LDFLAGS)

###########################################################################
# TARGET: put name of executable image here                               #
###########################################################################
TARGET = beye
BEYELIB = ./libbeye/$(LIBPREFIX)$(TARGET).$(LIBEXT)
##########################################################################
#                Please not modify contents below                        #
##########################################################################

INCS = -I.
LIBS = -L./libbeye -l$(TARGET) $(OS_LIBS)
SRCS = \
addendum.c\
bconsole.c\
beye.c\
beyehelp.c\
beyeutil.c\
bin_util.c\
bmfile.c\
codeguid.c\
colorset.c\
dialogs.c\
editors.c\
events.c\
fileutil.c\
info_win.c\
mainloop.c\
refs.c\
search.c\
setup.c\
sysinfo.c\
tstrings.c\
addons/sys/ascii.c\
addons/sys/consinfo.c\
addons/sys/cpu_perf.c\
addons/sys/inview.c\
addons/tools/dig_conv.c\
addons/tools/eval.c\
plugins/bin/aout.c\
plugins/bin/arch.c\
plugins/bin/asf.c\
plugins/bin/avi.c\
plugins/bin/bin.c\
plugins/bin/bmp.c\
plugins/bin/coff386.c\
plugins/bin/dos_sys.c\
plugins/bin/elf386.c\
plugins/bin/jpeg.c\
plugins/bin/jvmclass.c\
plugins/bin/le.c\
plugins/bin/lmf.c\
plugins/bin/lx.c\
plugins/bin/mov.c\
plugins/bin/mp3.c\
plugins/bin/mpeg.c\
plugins/bin/mz.c\
plugins/bin/ne.c\
plugins/bin/nlm386.c\
plugins/bin/opharlap.c\
plugins/bin/pe.c\
plugins/bin/pharlap.c\
plugins/bin/rdoff.c\
plugins/bin/rdoff2.c\
plugins/bin/realmedia.c\
plugins/bin/sis.c\
plugins/bin/sisx.c\
plugins/bin/wav.c\
plugins/binmode.c\
plugins/disasm.c\
plugins/hexmode.c\
plugins/textmode.c\
plugins/nls/russian.c\
plugins/disasm/null_da.c\
plugins/disasm/arm/arm.c\
plugins/disasm/arm/arm16.c\
plugins/disasm/arm/arm32.c\
plugins/disasm/avr/avr.c\
plugins/disasm/ppc/ppc.c\
plugins/disasm/java/java.c\
plugins/disasm/ix86/ix86.c\
plugins/disasm/ix86/ix86_fpu.c\
plugins/disasm/ix86/ix86_fun.c

OBJS=$(SRCS:.c=.o)

HLP_SUBDIRS=tools/beyehlp tools/lzss
DO_HELP = @ for i in $(HLP_SUBDIRS); do $(MAKE) -C $$i $@ || exit; done

all:     $(SUBDIRS) $(TARGET)

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
ifeq ($(filter 3.81,$(firstword $(sort $(MAKE_VERSION) \ 3.81))),)
$(error 'make' utility is too old. Required version is 3.81)
@exit 1
endif
	$(MAKE) -C $@

clean:
	$(DO_MAKE)
	$(RM) $(OBJS)
	$(RM) $(TARGET)
	$(RM) beye.map
	$(RM) *.err

distclean: clean
	$(DO_MAKE)
	$(RM) config.log config.mak

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

install:
ifeq ($(INSTALL),)
	@echo "*** 'install' utility was not found and you can't run automatic"
	@echo "*** installation. Please download 'fileutils' from ftp://ftp.gnu.org and"
	@echo "*** install them to have possibility perform autiomatic installation"
	@echo "*** of this project"
	@exit 1
endif
	$(INSTALL) -D -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
