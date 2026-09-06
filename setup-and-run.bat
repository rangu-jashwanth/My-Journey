@echo off
REM ============================================================================
REM SFML 3.x Automated Installation for Windows
REM This script downloads and sets up SFML for Stick Shift project
REM ============================================================================

echo.
echo ========== SFML 3.x Installation Script ==========
echo.
echo This will download SFML 3.0 GCC build and extract it to C:\SFML-3.0
echo.

setlocal enabledelayedexpansion

set SFML_URL=https://www.sfml-dev.org/files/SFML-3.0-windows-gcc-13.1.0-mingw-64bit.zip
set SFML_ZIP=%TEMP%\SFML-3.0.zip
set SFML_PATH=C:\SFML-3.0

REM Check if SFML already exists
if exist "%SFML_PATH%\include\SFML\Graphics.hpp" (
    echo SFML is already installed at %SFML_PATH%
    goto :compile
)

REM Download SFML using PowerShell
echo Downloading SFML 3.0...
echo.
powershell -Command ^
    "$ProgressPreference = 'Continue'; " ^
    "$client = New-Object System.Net.ServicePointManager; " ^
    "$client.SecurityProtocol = 'Tls12'; " ^
    "(New-Object System.Net.WebClient).DownloadFile('%SFML_URL%', '%SFML_ZIP%')" ^
    2>nul

if not exist "%SFML_ZIP%" (
    echo ERROR: Failed to download SFML
    echo.
    echo Please download manually from: https://www.sfml-dev.org/download/sfml/3.0/
    echo Choose: GCC 13.1.0 MinGW (SEH) - 64-bit
    echo Extract to: C:\SFML-3.0
    echo.
    pause
    exit /b 1
)

REM Extract SFML
echo Extracting SFML to %SFML_PATH%...
echo.

powershell -Command "Expand-Archive -Path '%SFML_ZIP%' -DestinationPath C:\ -Force" 2>nul

if exist "%SFML_PATH%\include\SFML\Graphics.hpp" (
    echo SUCCESS: SFML installed at %SFML_PATH%
    del "%SFML_ZIP%"
) else (
    echo ERROR: SFML extraction failed
    pause
    exit /b 1
)

echo.
echo ========== Next: Compiling Stick Shift ==========
echo.

:compile
REM Now compile the program
cd /d "%~dp0"

if not exist "main.cpp" (
    echo ERROR: main.cpp not found in %~dp0
    pause
    exit /b 1
)

echo Compiling stick-dance.exe...
echo.

set SFML_PATH=C:\SFML-3.0
set MINGW_PATH=%ProgramFiles%\mingw-w64
if not exist "%MINGW_PATH%" set MINGW_PATH=C:\mingw64
if not exist "%MINGW_PATH%" set MINGW_PATH=C:\mingw-w64

echo SFML Path:  %SFML_PATH%
echo MinGW Path: %MINGW_PATH%
echo.

g++ -std=c++17 -O2 -o stick-dance.exe main.cpp ^
    -I"%SFML_PATH%\include" ^
    -L"%SFML_PATH%\lib" ^
    -lsfml-graphics ^
    -lsfml-audio ^
    -lsfml-window ^
    -lsfml-system ^
    -mwindows

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========== BUILD SUCCESSFUL ==========
    echo.
    echo Copying DLL files...
    copy "%SFML_PATH%\bin\sfml-graphics-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-audio-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-window-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-system-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\openal32.dll" . >nul 2>&1
    echo.
    echo ========== READY TO RUN ==========
    echo.
    echo Your executable is ready: stick-dance.exe
    echo.
    echo To run:
    echo   1. Make sure song.mp3 is in this folder
    echo   2. Run: stick-dance.exe
    echo.
    pause
) else (
    echo.
    echo ========== BUILD FAILED ==========
    echo.
    echo See error messages above
    pause
    exit /b 1
)

endlocal
