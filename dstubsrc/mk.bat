@echo off
nasm -wno-other -fobj %1.asm
alink -c -o %1. -oEXE %1.obj
