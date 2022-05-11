@echo off
if exist %1.ocg goto Proceed
echo.
echo test.bat BaseFileName
goto Exit

:Proceed
if exist %1.exe del %1.exe
if exist %1.c del %1.c
if exist origo.exe if exist %1.ocg origo %1.ocg %1.c

:Exit
