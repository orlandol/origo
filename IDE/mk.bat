@echo off
if exist origoide.exe del origoide.exe
tcc origoide.c criterr.c config.c bytegen.c bytevm.c w32ui.c
