# This builds the core file of the 3D graphics API
#

ifndef BASEDIR
BASEDIR = $(CURDIR)/..
endif

PLCORECSRC :=					\
	$(BASEDIR)/src/plcore/babinary.c	\
	$(BASEDIR)/src/plcore/babinmtx.c	\
	$(BASEDIR)/src/plcore/bacolor.c		\
	$(BASEDIR)/src/plcore/badebug.c		\
	$(BASEDIR)/src/plcore/baerr.c		\
	$(BASEDIR)/src/plcore/baimmedi.c	\
	$(BASEDIR)/src/plcore/balist.c		\
	$(BASEDIR)/src/plcore/bamatrix.c	\
	$(BASEDIR)/src/plcore/bamemory.c	\
	$(BASEDIR)/src/plcore/baresour.c	\
	$(BASEDIR)/src/plcore/bastream.c	\
	$(BASEDIR)/src/plcore/batkbin.c		\
	$(BASEDIR)/src/plcore/batkreg.c		\
	$(BASEDIR)/src/plcore/bavector.c	\
	$(BASEDIR)/src/plcore/resmem.c		\
	$(BASEDIR)/src/plcore/rwdbgerr.c	\
	$(BASEDIR)/src/plcore/rwgrp.c		\
	$(BASEDIR)/src/plcore/rwstring.c

PLCOREINITCSRC = $(BASEDIR)/src/plcore/baplcore.c

PLCORECSRC += $(OSCSRC)

CSRC :=						\
	$(BASEDIR)/src/babbox.c			\
	$(BASEDIR)/src/babincam.c		\
	$(BASEDIR)/src/babinfrm.c		\
	$(BASEDIR)/src/babintex.c		\
	$(BASEDIR)/src/bacamera.c		\
	$(BASEDIR)/src/badevice.c		\
	$(BASEDIR)/src/baframe.c		\
	$(BASEDIR)/src/baimage.c		\
	$(BASEDIR)/src/baimras.c		\
	$(BASEDIR)/src/baraster.c		\
	$(BASEDIR)/src/baresamp.c		\
	$(BASEDIR)/src/basync.c			\
	$(BASEDIR)/src/batextur.c		\
	$(BASEDIR)/src/batypehf.c

include $(BASEDIR)/makeincl/makecore

######################################################################
# Compilation rules

$(CURDIR)/$(OBJDIR)/%.$(O) : $(BASEDIR)/src/plcore/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(BASEDIR)/src/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(BASEDIR)/os/$(RWOS)/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(DRVGENERICSRCDIR)/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(DRVGENERICSRCDIR)/%.$(ASM)
	$(AS) $(ASFLAGS) $(AOUT)$@ $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(DRVSPECIFICSRCDIR)/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(DRVSTUBSRCDIR)/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) : $(BASEDIR)/src/pipe/$(PIPETYPE)/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

$(CURDIR)/$(OBJDIR)/%.$(O) :  $(BASEDIR)/src/pipe/$(PIPETYPE)/$(RWTARGET)/%.c
	$(CC) $(CFLAGS) $(CODEGENCFLAGS) \
	$(COUT)$(subst /,$(PATHSEP),$@) $<

######################################################################
