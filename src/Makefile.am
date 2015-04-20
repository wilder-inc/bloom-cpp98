SUBDIRS = \
	stream \
	shared \
	net

lib_LTLIBRARIES = libbloom++.la

libbloom___la_SOURCES = \
	debug.cpp \
	hash_functions.cpp \
	log.cpp \
	string.cpp \
	time.cpp \
	condition_variable.cpp \
	exception.cpp

libbloom___la_LIBADD = \
	stream/libbloom++-io.la \
	shared/libbloom++-sha.la

AM_CPPFLAGS = -I$(top_builddir)/include

