@echo off
if not "%1" == "" goto DELETEGENERAL
if exist %1.c del %1.c
if exist %1.h del %1.h
if exist %1.def del %1.def
if exist %1.exe del %1.exe
:DELETEGENERAL

if exist test.c del test.c
if exist test.h del test.h
if exist test.def del test.def
if exist test.exe del test.exe

if exist origo.c del origo.c
if exist origo.h del origo.h
if exist origo.def del origo.def
if exist origo.exe del origo.exe
