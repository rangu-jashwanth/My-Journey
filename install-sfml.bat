@echo off
REM ============================================================================
REM MANUAL SFML 3.0 INSTALLATION GUIDE FOR WINDOWS
REM This script checks for SFML and guides manual installation if needed
REM ============================================================================

echo.
echo ========== SFML Installation Guide ==========
echo.

set SFML_PATH=C:\SFML-3.0

REM Check if SFML already exists
if exist "%SFML_PATH%\include\SFML\Graphics.hpp" (
    echo ✓ SFML already installed at %SFML_PATH%
    echo.
    goto :compile
)

echo SFML is not found at %SFML_PATH%
echo.
echo ==== MANUAL INSTALLATION STEPS ====
echo.
echo 1. Go to: https://github.com/SFML/SFML/releases/
echo.
echo 2. Look for the latest 3.x release
echo.
echo 3. Download the file:
echo    "SFML-3.0-windows-gcc-13.1.0-mingw-64bit.zip"
echo    (or the equivalent for your version)
echo.
echo 4. Extract the ZIP file to C:\ 
echo    This will create C:\SFML-3.0
echo.
echo 5. Verify the folder structure:
echo    C:\SFML-3.0\include\SFML\
echo    C:\SFML-3.0\lib\
echo    C:\SFML-3.0\bin\
echo.
echo 6. Once extracted, run this script again.
echo.
echo Press any key to open the download page...
pause

REM Try to open browser
start https://github.com/SFML/RELEASES
timeout /t 2 /nobreak

echo.
echo After downloading and extracting, press any key to continue compilation...
pause

REM Re-check if SFML was installed
if not exist "%SFML_PATH%\include\SFML\Graphics.hpp" (
    echo.
    echo ERROR: SFML still not found at %SFML_PATH%
    echo Please manually install SFML following the steps above.
    echo.
    pause
    exit /b 1
)

:compile
echo.
echo ========== Compiling Stick Shift ==========
echo.

REM Change to script directory
cd /d "%~dp0"

if not exist "main.cpp" (
    echo ERROR: main.cpp not found in %~dp0
    pause
    exit /b 1
)

echo Compiling stick-dance.exe...
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
    copy "%SFML_PATH%\bin\sfml-graphics-3.dll" . 2>nul
    copy "%SFML_PATH%\bin\sfml-audio-3.dll" . 2>nul
    copy "%SFML_PATH%\bin\sfml-window-3.dll" . 2>nul
    copy "%SFML_PATH%\bin\sfml-system-3.dll" . 2>nul
    copy "%SFML_PATH%\bin\openal32.dll" . 2>nul
    
    echo.
    echo ========== READY TO RUN ==========
    echo.
    echo Your executable is ready: stick-dance.exe
    echo.
    echo To run the program:
    echo   stick-dance.exe
    echo.
    pause
) else (
    echo.
    echo ========== BUILD FAILED ==========
    echo.
    echo Please check:
    echo   1. SFML is installed at C:\SFML-3.0
    echo   2. main.cpp exists in this folder
    echo   3. MinGW g++ is in your PATH
    echo.
    pause
    exit /b 1
)
