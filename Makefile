all: src/main.c src/PPDStrings.h src/datatypes.h
	gcc src/main.c -o ppd-generator -O2 -Wall -Werror

clean:
	rm -f ppd-generator
