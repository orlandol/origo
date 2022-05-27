@echo off
if "%1" == "origo" goto SkipDeleteOrigo
if exist %1.c del %1.c
if exist %1.def del %1.def
if exist %1.exe del %1.exe
:SkipDeleteOrigo

if exist cgen.c del cgen.c
if exist cgen.def del cgen.def
if exist cgen.exe del cgen.exe

if exist retg1.c del retg1.c
if exist retg1.def del retg1.def
if exist retg1.exe del retg1.exe

if exist origo.def del origo.def
if exist origo.exe del origo.exe
