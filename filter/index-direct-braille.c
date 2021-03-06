/**
 * CUPS filter for printers supporting Index-direct-Braille.
 * Copyright (C) 2014 Index Braille AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

/**
 * We only buffer reading, output (to stdout) may or may not be buffered.
 */
static char buf[8096];
static int bytes_written;

/**
 * Send a single character to stdout, also keep track of number of bytes sent.
 * @param c Character value to send.
 */
static void output(int c) {
	/**
	 * Keep track of number of bytes written, we wish to avoid a annoying
	 * bug in the printer usb-driver (or hardware?).
	 */
	bytes_written++;
	if (fputc(c, stdout) == EOF) {
		fprintf(stderr, "ERROR: Unable to write to stdout: %s.\n", strerror(errno));
		exit(1);
	}
}

/**
 * Character codes with special meaning.
 */
enum {
	SUB=0x1a,
	ESC=0x1b
} escape_characters_t;

/**
 * Entry point for filter, parameters for filter in argv. Only copies (#4) and
 * input file (#6) are used. It should be noted that (#6) is required atm.
 *
 * Signal SIGPIPE is ignored as suggested by the CUPS-filter programming
 * manual. It should be noted that SIGTERM is _also_ ignored, since we
 * currently have no way to abort the printout w/o corrupting the document.
 *
 * @param  argc Number of arguments.
 * @param  argv Array or arguments.
 * @return      0 upon success, 1 upon failure (see CUPS filter programming).
 */
int main(int argc, char **argv) {
	int i;
	int input = STDIN_FILENO;
	char *tmp;
	const char * const filename = argv[6];
	long int copies = 1;

	/**
	 * Ignore pipe, if another filter in the chain terminates unexpectedly,
	 * this signal will be raised.
	 */
	signal(SIGPIPE, SIG_IGN);

	/**
	 * Ignore SIGTERM, this is sent by the scheduler when a job is canceled
	 * or held. When this signal is raised we need to abort the job and
	 * place the printer in a valid state. Since there is no way to abort
	 * w/o leaving the printer in an invalid state we ignore this signal.
	 */
	signal(SIGTERM, SIG_IGN);

	/**
	 * Update status (usually never seen, transfer is to short).
	 */
	fprintf(stderr, "INFO: Sending data to printer\n");

	/**
	 * Debug info, usually not visible.
	 */
	fprintf(stderr, "DEBUG: ARGV(%d): %s ", argc, argv[0]);
	for (i=1;i<argc;i++) {
		fprintf(stderr, " %s ", argv[i]);
	}
	fprintf(stderr, "\n");

	/** Ensure we get at least 5 and no more than 6 arguments (see CUPS filter programming API). */
	if (argc < 6 || argc > 7) {
		fprintf(stderr, "ERROR: Invalid number of arguments %d, expected at least 5 and less than 6.\n", argc);
		exit(1);
	}

	/** Open file if filename given. */
	if (argc == 7) {
		input = open(argv[6], O_RDONLY);
		if (input == -1) {
			fprintf(stderr, "ERROR: Unable to open '%s': %s\n", argv[6], strerror(errno));
			exit(1);
		}
	}

	/** Check if we should produce more than one copy. */
	if (input != STDIN_FILENO) {
		copies = strtol(argv[4], &tmp, 10);
		if (tmp == argv[4]) {
			fprintf(stderr, "WARNING: Unable to parse number of copies '%s', assuming 1 copy.\n", argv[4]);
			copies = 1;
		} else if (copies <= 0) {
			fprintf(stderr, "WARNING: Number of copies was negative or zero '%s', assuming 1 copy.\n", argv[4]);
			copies = 1;
		}
	}

	/**
	 * Prefix with escape (\x1b) + I, which indicates that this is an
	 * Index-direct-Braille file.
	 */
	output('\x1b');
	output('I');

	/**
	 * Escape file, otherwise we might send the printer invalid escape-
	 * senquences and premature EOFs.
	 */
	for (;;) {
		ssize_t bytes = read(input, buf, sizeof(buf));
		if (bytes == 0) {
			/** EOF. */
			break;
		} else if (bytes == -1) {
			fprintf(stderr, "ERROR: Unable to read from file '%s': %s\n", filename, strerror(errno));
		} else {
			size_t i;
			for (i=0;i<bytes;i++) {
				char c = buf[i];

				/**
				 * ESC-characters and SUB-characters must
				 * be escaped, i.e. prefix with '\1xb' and
				 * invert their value.
				 */
				switch (c) {
				case SUB:
				case ESC:
					output(ESC);
					output(~c);
					break;
				default:
					output(c);
					break;
				}
			}
		}
	}

	/** Indicate EOF to printer. */
	output(SUB);

	/**
	 * Try to avoid the multiple of 64 bug in the printer. The if so,
	 * output an extra SUB-character, since that will result in an an
	 * empty document that is discarded.
	 */
	if ((bytes_written % 64) == 0) {
		output(SUB);
	}

	/**
	 * Close the input file, unless it's stdin.
	 */
	if (input != STDIN_FILENO) {
		close(input);
	}

	fprintf(stderr, "INFO: Ready\n");

	return 0;
}
