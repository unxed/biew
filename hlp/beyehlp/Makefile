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
# All rights reserved. This software is redistributable under the licence    #
# This software is redistributable under the licence given in the file       #
# "Licence.en" ("Licence.ru" in russian translation) distributed in the BEYE #
##############################################################################

##############################################################################
#  Main configure section of this makefile                                   #
##############################################################################
HOST_CFLAGS=
HOST_LDFLAGS=

include ../../config.mak

CFLAGS = $(CDEFOS) $(CDEFSYS) $(HOST_CFLAGS)
LDFLAGS = $(OSLDEF) $(HOST_LDFLAGS)
###########################################################################
# TARGET: put name of executable image here                               #
###########################################################################
TARGET = beyehlp

##########################################################################
#                Please not modify contents below                        #
##########################################################################

INCS = -I. -I../../src
LIBS = -L../../src/libbeye -lbeye $(OS_LIBS)
OBJS =\
beyehlp.o
all: $(TARGET)
help: $(TARGET)

distclean:
clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)
	$(RM) beye.map

cleansys:
	$(RM) lib/sysdep/$(MACHINE)/$(HOST)/*.o

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

beyehlp.o:                     beyehlp.c
