@echo off
if exist origo.exe del origo.exe
if exist origo.peg if exist tools\packcc\packcc.exe tools\packcc\packcc.exe origo.peg
if exist origo.c if exist tools\tcc\tcc.exe tools\tcc\tcc.exe origo.c
if exist origo.exe if exist tools\verpatch\verpatch.exe tools\verpatch\verpatch origo.exe /va 0.1.0.0 /pv 0.1.0.0 /s desc "Origo Compiler Generator" /s copyright "(C) 2022 Orlando Llanes"
