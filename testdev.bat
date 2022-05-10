@echo off
if exist dev.exe del dev.exe
if exist dev.c del dev.c
if exist origo.exe if exist dev.ocg origo dev.ocg dev.c
