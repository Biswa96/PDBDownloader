@echo off
::Set Environments for X86_64 build
cd %~dp0
call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
where cl.exe link.exe

::Set Environment Variables
set NAME=PDBDownloader.exe
set BINDIR=bin
set SRCDIR=src
set CFLAGS=/c /nologo /Os /MD /W4 /Fo%BINDIR%\\
set LFLAGS=/nologo /MACHINE:X64
set LIBS=winInet.lib

::Disable warnings
set CCOPT=/D _CRT_SECURE_NO_WARNINGS

::Build
rd /s /q %BINDIR%
mkdir %BINDIR%
cl.exe %CFLAGS% %CCOPT% %SRCDIR%\*.c
link.exe %LFLAGS% %LIBS% %BINDIR%\*.obj /OUT:%BINDIR%\%NAME%

dir /B %BINDIR%\*.exe
pause
exit /b

::END#