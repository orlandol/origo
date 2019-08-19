@echo off
nasm -wno-other -fobj %1.asm
alink -c -oPE -subsys console %1.obj
