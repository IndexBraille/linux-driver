AC_PREREQ([2.68])
AC_INIT([printer-driver-indexbraille], [1.1.0], [support@indexbraille.com])
AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_SRCDIR([filter/indexbraille.sh])

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
		APREFIX=/Library/Printers
		FILTER_PATH="${APREFIX}/Index-Braille/filter"
		MIME_PATH="${APREFIX}/Index-Braille/mime"
		DRV_PATH="${APREFIX}/Index-Braille/drv"
		PPD_PATH="${APREFIX}/PPDs/Contents/Resources"
		PPD_PREFIX="Index-Braille-"
		;;
	*)
		FILTER_PATH="${libdir}/cups/filter"
		MIME_PATH="${datadir}/cups/mime"
		DRV_PATH="${datadir}/cups/drv"
		PPD_PATH="${datadir}/cups/ppd/Index-Braille"
		PPD_PREFIX=""
		;;
esac

AC_SUBST(FILTER_PATH)
AC_SUBST(MIME_PATH)
AC_SUBST(DRV_PATH)
AC_SUBST(PPD_PATH)
AC_SUBST(PPD_PREFIX)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([mkdeb.sh], [chmod +x mkdeb.sh])
AC_OUTPUT
