#!/bin/sh

# Arguments, only the filename ($6) is used atm.
filename="$6"

# Pass the document through unmolested. We only want to terminate
# the document with ASCII SUB (see below) in order to start printing
# immediately (printer should print after 5 seconds anyway, but
# let's do things "right").
cat "$filename"

# ASCII SUB terminates document (032).
printf "\032"
