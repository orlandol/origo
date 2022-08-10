@echo off
if exist retg1.exe del retg1.exe
if exist retg1.peg if exist tools\packcc\packcc.exe tools\packcc\packcc.exe retg1.peg
if exist retg1.c tools\tcc\tcc.exe retg1.c
if exist retg1.exe if exist tools\verpatch\verpatch.exe tools\verpatch\verpatch retg1.exe /va 0.1.0.0 /pv 0.1.0.0 /s desc "retg1 Compiler Generator" /s copyright "(C) 2022 Orlando Llanes"

if exist sdbasics.exe del sdbasics.exe
if exist sdbasics.c tools\tcc\tcc.exe sdbasics.c

if exist skbasics.exe del skbasics.exe
if exist skbasics.c tools\tcc\tcc.exe skbasics.c

if exist stbasics.exe del stbasics.exe
if exist stbasics.c tools\tcc\tcc.exe stbasics.c
