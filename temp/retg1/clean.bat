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

if exist testst.exe del testst.exe

if exist retg1.c del retg1.c
if exist retg1.h del retg1.h
if exist retg1.def del retg1.def
if exist retg1.exe del retg1.exe

if exist sdbasics.exe del sdbasics.exe

if exist skbasics.exe del skbasics.exe

if exist stbasics.exe del stbasics.exe
