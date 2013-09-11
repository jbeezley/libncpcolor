AC_INIT([libncpcolor], [0.0.0], [jon.beezley@gmail.com], [libncpcolor], [https://github.com/jbeezley/libncpcolor])
AC_PREREQ()
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([config.h])
LT_INIT()

AC_SUBST([LIBNCPCOLOR_SO_VERSION], [0:0:0])
AC_SUBST([LIBNCPCOLOR_API_VERSION], [0.0.0])

AC_PROG_CXX
AC_PROG_CC
PKG_CHECK_MODULES([NETCDF], [netcdf >= 3.6])
PKG_CHECK_MODULES([LIBPNG], [libpng >= 1.5])
AC_CONFIG_FILES([Makefile libncpcolor.pc:libncpcolor.pc.in])
AC_OUTPUT
