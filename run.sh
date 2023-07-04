#!/bin/bash
rm *.i
rm *.s
rm *.n
rm *.os
rm *.p
rm *.asm
rm *.lst

lcpp -I../../bin/include/ -I./src -I./src/TinyTetris/ -D__COMX__ src/main.c T3tri5.i
lrcc -v -target=xr18CX -D__COMX__ -volatile T3tri5.i T3tri5.s
asl -cpu 1802 -D LCCCX=1 -D _COMX_=1 -D COMX_HEADER=1 -P -x -i ../../bin/include/ -i ./src/ -L T3tri5.s
p2bin T3tri5.p T3tri5.comx
# keyEOP T3tri5.comx T3tri5.txt
comxLoader /dev/ttyACM0 T3tri5.comx
mv T3tri5.i T3tri5.asm
