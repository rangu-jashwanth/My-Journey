@echo off
REM ============================================================================
REM Build script for Stick Shift (C++ with SFML 3.x)
REM Run this batch file to compile the program
REM ============================================================================

echo.
echo ========== Stick Shift Build Script ==========
echo.

REM Configuration - Change these to match your installation paths
set SFML_PATH=C:\SFML-3.0
set MINGW_PATH=C:\MinGW
set OUTPUT_NAME=stick-dance.exe
set SOURCE_FILE=main.cpp

REM Verify paths exist
if not exist "%SFML_PATH%\include\SFML\Graphics.hpp" (
    echo ERROR: SFML not found at %SFML_PATH%
    echo Please update SFML_PATH in this batch file.
    pause
    exit /b 1
)

if not exist "%MINGW_PATH%\bin\g++.exe" (
    echo ERROR: MinGW not found at %MINGW_PATH%
    echo Please update MINGW_PATH in this batch file.
    pause
    exit /b 1
)

echo SFML Path:  %SFML_PATH%
echo MinGW Path: %MINGW_PATH%
echo.

REM Compile
echo Compiling %SOURCE_FILE%...
echo.

"%MINGW_PATH%\bin\g++" -std=c++17 -O2 -o "%OUTPUT_NAME%" "%SOURCE_FILE%" ^
    -I"%SFML_PATH%\include" ^
    -L"%SFML_PATH%\lib" ^
    -lsfml-graphics ^
    -lsfml-audio ^
    -lsfml-window ^
    -lsfml-system ^
    -mwindows

REM Check if compilation was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========== BUILD SUCCESSFUL ==========
    echo.
    echo Created: %OUTPUT_NAME%
    echo.
    echo Copying required DLL files from SFML...
    echo.
    
    REM Copy DLL files
    copy "%SFML_PATH%\bin\sfml-graphics-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-audio-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-window-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-system-3.dll" . >nul 2>&1
    copy "%SFML_PATH%\bin\sfml-network-3.dll" . >nul 2>&1
    
    REM Copy OpenAL DLL for audio support
    if exist "%SFML_PATH%\bin\openal32.dll" (
        copy "%SFML_PATH%\bin\openal32.dll" . >nul 2>&1
    )
    
    echo.
    echo DLL files copied to project directory.
    echo.
    echo ========== READY TO RUN ==========
    echo.
    echo To run the program:
    echo   1. Place an MP3 file named "song.mp3" in this directory
    echo   2. Run: %OUTPUT_NAME%
    echo.
    echo Controls:
    echo   SPACE = Pause/Resume
    echo   K     = Skip forward 10 seconds
    echo   T     = Toggle Turbo mode
    echo   M     = Toggle Mirror mode
    echo   F     = Flash effect
    echo   R     = Restart song
    echo   ESC   = Quit
    echo.
    
    REM Ask if user wants to run immediately
    setlocal enabledelayedexpansion
    if exist "song.mp3" (
        echo song.mp3 found. Starting program...
        echo.
        %OUTPUT_NAME%
    ) else (
        echo WARNING: No song.mp3 found in this directory.
        echo Please add an MP3 file and run %OUTPUT_NAME%
        pause
    )
    
) else (
    echo.
    echo ========== BUILD FAILED ==========
    echo.
    echo Compilation encountered errors. See details above.
    echo.
    echo Common issues:
    echo   - SFML path incorrect (check SFML_PATH variable)
    echo   - MinGW not installed properly (check MINGW_PATH variable)
    echo   - Missing C++17 support (update MinGW compiler)
    echo.
    pause
    exit /b 1
)

exit /b 0
