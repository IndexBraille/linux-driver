#!/bin/sh

# Arguments, only filename ($6) and copies are used atm.
COPIES="${4}"
FILENAME="${6}"

# The extra number of bytes we send.
TRANSFERED_BYTES=0

# File lenght patching is enabled by default.
PATCH_FILE_LENGTH=1

# This filter must be the first in the chain, i.e. FILENAME must NOT be empty.
if test -z "${FILENAME}"
then
	echo "ERROR: `basename $0` is not at the head of the filter-chain." >> /dev/stderr
	exit 1
fi

# Check for required utilities, otherwise disable file-length patching (since
# if till fail).
WC=`which wc`
AWK=`which awk`
EXPR=`which expr`
if test -z "${WC}" -o -z "${AWK}" -o -z "${EXPR}"
then
	# Some utility is missing, inform user.
	echo "INFO: Utilities missing, file-length patching is disabled." >> /dev/stderr
	echo "INFO: WC='${WC}', AWK='${AWK}', EXPR='${EXPR}'." >> /dev/stderr
	PATCH_FILE_LENGTH=0
fi

# Grab the lenght of the file, if we are patching the length.
if test $PATCH_FILE_LENGTH -ne 0
then
	# Patch the file lenght, if required.
	FILE_LENGTH=`wc -c $FILENAME|awk '{print $1}'`
	if test ! -z $FILE_LENGTH
	then
		echo "INFO: Unable to read file-length, file-length patching is disabled." >> /dev/stderr
		PATCH_FILE_LENGTH=0
	fi
fi

# Produce $COPIES number of documents.
while test $COPIES -gt 0
do

	# Pass the document through unmolested. We only want to terminate
	# the document with ASCII SUB (see below) in order to start printing
	# immediately (printer should print after 5 seconds anyway, but
	# let's do things "right").
	cat "$FILENAME"

	# ASCII SUB terminates document (032).
	printf "\032"

	if test $PATCH_FILE_LENGTH -ne 0
	then
		# We sent FILE_LENGTH + SUB-character.
		TRANSFERED_BYTES="${TRANSFERED_BYTES} + ${FILE_LENGTH} + 1"
	fi

	# Decrease COPIES by one, if expr doesn't exists, we don't support
	# multiple copies.
	test -z "${EXPR}" && break
	COPIES=`$EXPR $COPIES - 1`
done

# Check the length of the transfer, if the lenght of the transfer is a
# multiple of 64, add an extra SUB character. This is due to the kernel
# module (at91_udc.c) uses the length of a transfer to determine if the
# transfer was completed or more packets are coming, i.e. if the total
# lenght is a multiple of 64, then the driver will keep waiting for
# more data (that isn't coming!). An alternative would be to send a
# zero-length packet at the end of each transfer, however, this would
# require distrubution of a special usb-driver or incorporating this
# into the lib-usb driver of cups. This is the quick, dirty, and
# working fix (we can always send extra SUB characters).

# Patch the transfer, if enabled and required.
if test $PATCH_FILE_LENGTH -ne 0
then
	TRANSFERED_BYTES=`$EXPR $TRANSFERED_BYTES`
	TRANSFERED_BYTES_MOD64=`$EXPR $FILE_LENGTH % 64`
	if test $TRANSFERED_BYTES_MOD64 -eq 0
	then
		# Extra ASCII sub to ensure that the transfer length module 64
		# is not 0.
		echo "INFO: Patching file to avoid transfer length modulo-64 bug (length was $FILE_LENGTH)." >> /dev/stderr
		printf "\032"
	fi
fi
