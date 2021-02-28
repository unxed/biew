include ../../config.mak

CFLAGS = -I.. $(CDEFOS) $(CDEFSYS) $(HOST_CFLAGS)
LDFLAGS = $(OSLDEF) $(HOST_LDFLAGS)

LIBNAME = $(LIBPREFIX)beye.$(LIBEXT)

SRCS= bbio.c \
      file_ini.c \
      libbeye.c \
      pmalloc.c \
      tw_class.c \
      twin.c \
      sysdep/$(MACHINE)/aclib.c \
      sysdep/$(MACHINE)/cpu_info.c \
      sysdep/$(MACHINE)/$(HOST)/fileio.c \
      sysdep/$(MACHINE)/$(HOST)/keyboard.c \
      sysdep/$(MACHINE)/$(HOST)/mmfio.c \
      sysdep/$(MACHINE)/$(HOST)/mouse.c \
      sysdep/$(MACHINE)/$(HOST)/misc.c \
      sysdep/$(MACHINE)/$(HOST)/nls.c \
      sysdep/$(MACHINE)/$(HOST)/os_dep.c \
      sysdep/$(MACHINE)/$(HOST)/timer.c \
      sysdep/$(MACHINE)/$(HOST)/vio.c


ifeq ($(HOST),qnx)
ASMS = \
      sysdep/$(MACHINE)/$(HOST)/3rto3s.o\
      sysdep/$(MACHINE)/$(HOST)/3sto3r.o
endif

OBJS=$(SRCS:.c=.o)
AOBJS=$(ASMS:.asm=.o)

.SUFFIXES: .asm .c .o

# .PHONY: all clean

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
.asm.o:
	$(CC) -c -o $@ $<

$(LIBNAME):     $(OBJS) $(AOBJS)
	$(AR) $(LIBNAME) $(OBJS) $(AOBJS)

all:    $(LIBNAME)

clean:
	rm -f *.o *.a *~

distclean:
	rm -f Makefile.bak *.o *.a *~ .depend

dep:    depend

depend:
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif

