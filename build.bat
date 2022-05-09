@echo off
if exist origo.exe del origo.exe
if exist origo.c if exist tools\tcc\tcc.exe tools\tcc\tcc.exe origo.c
if exist origo.exe if exist tools\verpatch\verpatch.exe tools\verpatch\verpatch origo.exe /va 0.0.1.1 /pv 0.0.1.1 /s desc "Origo Compiler Generator" /s copyright "(C) 2022 Orlando Llanes"
