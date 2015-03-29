
SUBDIRS = \
	src \
	include

EXTRA_DIST = \
	Doxyfile.config \
	bloom++-logo.png \
	LICENSE

pkgconfig_DATA = \
	bloom++.pc

pkgconfigdir = $(libdir)/pkgconfig

$(pkgconfig_DATA): config.status

uninstall-hook:
	rm -Rf $(includedir)/bloom++
