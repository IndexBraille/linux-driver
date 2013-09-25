#!/bin/sh

# Arguments, only the filename is used...
job="$1"
user="$2"
title="$3"
numcopies="$4"
options="$5"
filename="$6"

# Pass through, we only want to terminate the document with
# ASCII SUB (see below) in order to start printing immediately.
cat "$filename"

# ASCII SUB terminates document (032).
printf "\032"
