SUBDIRS = \
	tcp \
	udp

lib_LTLIBRARIES = libbloom++-net.la

libbloom___net_la_SOURCES = \
	addr_ipv4.cpp \
	socket_base.cpp
	
libbloom___net_la_LIBADD = \
	tcp/libbloom++-tcp.la \
	udp/libbloom++-udp.la

AM_CPPFLAGS = -I$(top_builddir)/include

