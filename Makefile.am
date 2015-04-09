
SUBDIRS = \
	src \
	include

EXTRA_DIST = \
	Doxyfile.config \
	bloom++-logo.png \
	COPYING

pkgconfig_DATA = \
	bloom++.pc \
	bloom++-net.pc

pkgconfigdir = $(libdir)/pkgconfig

$(pkgconfig_DATA): config.status

uninstall-hook:
	rm -Rf $(DESTDIR)$(includedir)/bloom++
