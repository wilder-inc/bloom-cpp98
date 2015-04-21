define(BL_VERSION,0)
define(BL_MAJOR,3)
define(BL_MINOR,1)
define(BL_LOCALVER,-dev)
#
# bloom++ configuration
#

define(VERSIONSTR,$1.$2.$3$4)

AC_PREREQ([2.69])
#AC_REQUIRE_AUX_FILE(versionrc)
AC_INIT([bloompp], [VERSIONSTR(BL_VERSION,BL_MAJOR,BL_MINOR,BL_LOCALVER)], [sergei.khairulin@gmail.com])
LT_INIT
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([bloom++.pc bloom++-net.pc])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

CXXFLAGS="-DLINUX -O3"

# Checks for libraries.
AC_CHECK_LIB([c], [memcpy])
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stddef.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([clock_gettime inet_ntoa memchr memmove memset select socket])

AC_SUBST(CXXFLAGS)
AC_SUBST(BLOOM_VERSION)
AC_SUBST(BLOOM_MAJOR)
AC_SUBST(BLOOM_MINOR)
AC_SUBST(BLOOM_LOCALVER)

AC_CONFIG_FILES([Makefile
		 include/Makefile
                 include/bloom++/Makefile
                 include/bloom++/_bits/Makefile
                 include/bloom++/stream/Makefile
                 include/bloom++/shared/Makefile
                 include/bloom++/net/Makefile
                 include/bloom++/net/tcp/Makefile
                 include/bloom++/net/udp/Makefile
                 src/Makefile
                 src/stream/Makefile
                 src/shared/Makefile
                 src/net/Makefile
                 src/net/udp/Makefile
                 src/net/tcp/Makefile
])
AC_OUTPUT
