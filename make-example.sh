#!/bin/bash

# Nothing exciting in here, simple static linking to our PitParser
# Ex. http://tldp.org/HOWTO/Program-Library-HOWTO/more-examples.html

gcc -Wall -g -c -o lib/libpit-parser.o lib/PitParser.c
ar rcs lib/libpit-parser.a lib/libpit-parser.o
mv lib/libpit-parser.a .
gcc -Wall -g -c example.c -o example.o
gcc -g -o example example.o -L. -lpit-parser

# Do some cleaning up
rm example.o libpit-parser.a lib/libpit-parser.o