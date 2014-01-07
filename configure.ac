AC_PREREQ([2.68])
AC_INIT([printer-driver-indexbraille], [1.0.1], [support@indexbraille.com])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([filter/indexbraille.sh])

AC_PROG_INSTALL
AC_CHECK_PROG([PPDC], [ppdc], [ppdc])
if ! test x$PPDC = xppdc
then
	 AC_MSG_ERROR([Unable to find ppdc, required to generate the ppd-files.])
fi
AC_SUBST(PPDC)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([mkdeb.sh], [chmod +x mkdeb.sh])
AC_OUTPUT
