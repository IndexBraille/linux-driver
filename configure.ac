AC_PREREQ([2.68])
AC_INIT([printer-driver-indexbraille], [1.1.0], [support@indexbraille.com])
AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_SRCDIR([filter/index-direct-braille.c])

AC_PROG_CC
AC_PROG_INSTALL
AC_CHECK_PROG([PPDC], [ppdc], [ppdc])
if ! test x$PPDC = xppdc
then
	 AC_MSG_ERROR([Unable to find ppdc, required to generate the ppd-files.])
fi
AC_SUBST(PPDC)

AC_CANONICAL_HOST
case $host_os in
	darwin*)
		OSX_CUPS=/usr/share/cups
		OSX_PRINTERS=/Library/Printers

		MIME_PATH="${OSX_CUPS}/mime"
		DRV_PATH="${OSX_PRINTERS}/Index-Braille/drv"
		PPD_PATH="${OSX_PRINTERS}/PPDs/Contents/Resources"
		PPD_PREFIX="Index-Braille-"
		FILTER_PATH="${OSX_PRINTERS}/Index-Braille/filter"
		PPD_FILTER="s|^\(\*cupsFilter: \\\".*\)\(index-direct-braille\\\"\)|\1${FILTER_PATH}/\2|g"
		;;
	*)
		MIME_PATH="${datadir}/cups/mime"
		DRV_PATH="${datadir}/cups/drv"
		PPD_PATH="${datadir}/cups/ppd/Index-Braille"
		PPD_PREFIX=""
		FILTER_PATH="${libdir}/cups/filter"
		PPD_FILTER=""
		;;
esac

AC_SUBST(FILTER_PATH)
AC_SUBST(MIME_PATH)
AC_SUBST(DRV_PATH)
AC_SUBST(PPD_PATH)
AC_SUBST(PPD_PREFIX)
AC_SUBST(PPD_FILTER)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([mkdeb.sh], [chmod +x mkdeb.sh])
AC_CONFIG_FILES([filter/index-braille.sh)])
AC_CONFIG_FILES([osx-installer/printer-driver-indexbraille.xml])
AC_CONFIG_FILES([osx-installer/welcome.html])
AC_CONFIG_FILES([osx-installer/license.html])
AC_CONFIG_FILES([osx-installer/conclusion.html])
AC_OUTPUT
