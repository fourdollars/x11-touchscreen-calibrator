ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = x11-touchscreen-calibrator

x11_touchscreen_calibrator_SOURCES = x11-touchscreen-calibrator.c
x11_touchscreen_calibrator_CFLAGS  = @X11_CFLAGS@
x11_touchscreen_calibrator_LDFLAGS = @X11_LIBS@

xsessiondir   = $(sysconfdir)/X11/Xsession.d/
xsession_DATA = 75x11-touchscreen-calibrator

EXTRA_DIST = 75x11-touchscreen-calibrator.in \
             x11-touchscreen-calibrator.spec
CLEANFILES = $(xsession_DATA)

do_subst = sed -e 's,[@]bindir[@],$(bindir),g'

75x11-touchscreen-calibrator: 75x11-touchscreen-calibrator.in
	$(do_subst) < $< > $@

dist_man_MANS = x11-touchscreen-calibrator.1

ChangeLog:
	git log --date-order --date=short | \
		sed -e '/^commit.*$$/d' | \
		awk '/^Author/ {sub(/\\$$/,""); getline t; print $$0 t; next}; 1' | \
		sed -e 's/^Author: //g' | \
		sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' | \
		sed -e 's/^\(.*\) \(<.*>\)\t\(.*\)/\3  \1 \2/g' -e 's/^    /\t/g' > $@
