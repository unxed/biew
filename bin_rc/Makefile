# a BSD compatible 'install' program
INSTALL = install

include ../config.mak

all:
clean:
distclean:
	echo "nothing to do"
install:
	$(INSTALL) -D -b -c -m 644 beye.hlp $(DESTDIR)$(DATADIR)/beye.hlp
	mkdir --parents $(DESTDIR)$(DATADIR)/skn
	$(INSTALL) -D -c -m 644 skn/*.skn $(DESTDIR)$(DATADIR)/skn
	mkdir --parents $(DESTDIR)$(DATADIR)/syntax
	$(INSTALL) -D -c -m 644 syntax/*.stx $(DESTDIR)$(DATADIR)/syntax
	mkdir --parents $(DESTDIR)$(DATADIR)/xlt/russian
	$(INSTALL) -D -c -m 644 xlt/russian/*.xlt $(DESTDIR)$(DATADIR)/xlt/russian
	$(INSTALL) -D -c -m 644 xlt/*.xlt xlt/readme $(DESTDIR)$(DATADIR)/xlt
uninstall:
	rm -f $(DESTDIR)$(DATADIR)/*
	$(RM) $(DESTDIR)$(DATADIR)/skn/*
	rmdir -p --ignore-fail-on-non-empty $(DESTDIR)$(DATADIR)/skn
	$(RM) $(DESTDIR)$(DATADIR)/syntax/*
	rmdir -p --ignore-fail-on-non-empty $(DESTDIR)$(DATADIR)/syntax
	$(RM) $(DESTDIR)$(DATADIR)/xlt/russian/*
	rmdir -p --ignore-fail-on-non-empty $(DESTDIR)$(DATADIR)/xlt/russian
	$(RM) $(DESTDIR)$(DATADIR)/xlt/*
	rmdir -p --ignore-fail-on-non-empty $(DESTDIR)$(DATADIR)/xlt
	$(RM) $(DESTDIR)$(DATADIR)/*
	rmdir -p --ignore-fail-on-non-empty $(DESTDIR)$(DATADIR)
dep:
