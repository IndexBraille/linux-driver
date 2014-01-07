#!/bin/sh

# Arguments, only the filename ($6) is used atm.
FILENAME="$6"

# Pass the document through unmolested. We only want to terminate
# the document with ASCII SUB (see below) in order to start printing
# immediately (printer should print after 5 seconds anyway, but
# let's do things "right").
cat "$FILENAME"

# ASCII SUB terminates document (032).
printf "\032"

# Check the length of the file, if the lenght of the file + the terminating
# ASCII SUB character is a multiple of 64, add an extra SUB character. THis is
# due to the kernel module (at91_udc.c) uses the length of a transfer to
# determine if the transfer was completed or more packets are coming, i.e. if
# the total lenght is a multiple of 64, then the driver will keep waiting for
# more data (that isn't coming!). An alternative would be to send a zero-length
# packet at the end of each transfer, however, this would require distrubution
# of a special usb-driver or incorporating this into the lib-usb driver of cups.
# This is the quick, dirty, and working fix (we can always send extra SUB
# characters).

# Check for required utilities, otherwise disable file-length patching (since
# if till fail).
WC=`which wc`
AWK=`which awk`
EXPR=`which expr`
if test -z "${WC}" -o -z "${AWK}" -o -z "${EXPR}"
then
	# Some utility is missing, inform user.
	echo "INFO: Utilities missing, file-length patching is disabled."
	echo "INFO: WC='${WC}', AWK='${AWK}', EXPR='${EXPR}'."
	exit 0
fi

# Patch the file lenght, if required.
FILE_LENGTH=`wc -c $FILENAME|awk '{print $1}'`
if test ! -z $FILE_LENGTH
then
	FILE_LENGTH_MOD64=`expr $FILE_LENGTH % 64`
	if test $FILE_LENGTH_MOD64 -eq 63
	then
		# Extra ASCII sub to ensure that the transfer length module 64
		# is not 0.
		echo "INFO: Patching file to avoid transfer length modulo-64 bug (length was $FILE_LENGTH)." >> /dev/stderr
		printf "\032"
	fi
fi
