$SFML_PATH = "C:\SFML-3.0"
$PROJECT_DIR = Get-Location

Write-Host "============================================" -ForegroundColor Cyan
Write-Host "  Stick Shift C++ Compiler - PowerShell" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""

# Check if SFML exists
if (-not (Test-Path "$SFML_PATH\include\SFML\Graphics.hpp")) {
    Write-Host "ERROR: SFML not found at $SFML_PATH" -ForegroundColor Red
    Write-Host ""
    Write-Host "Steps to fix:" -ForegroundColor Yellow
    Write-Host "1. Download SFML from: https://github.com/SFML/SFML/releases"
    Write-Host "2. Extract to: C:\SFML-3.0"
    Write-Host "3. Run this script again"
    Write-Host ""
    Read-Host "Press Enter to continue"
    exit 1
}

# Check if main.cpp exists
if (-not (Test-Path "main.cpp")) {
    Write-Host "ERROR: main.cpp not found in $PROJECT_DIR" -ForegroundColor Red
    exit 1
}

Write-Host "Compiling stick-dance.exe..." -ForegroundColor Green
Write-Host "SFML Path: $SFML_PATH" -ForegroundColor Gray
Write-Host ""

# Compile
g++ -std=c++17 -O2 -o stick-dance.exe main.cpp `
    -I"$SFML_PATH\include" `
    -L"$SFML_PATH\lib" `
    -lsfml-graphics `
    -lsfml-audio `
    -lsfml-window `
    -lsfml-system `
    -mwindows

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "============================================" -ForegroundColor Green
    Write-Host "  ✓ BUILD SUCCESSFUL" -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    
    # Copy DLLs
    Write-Host "Copying DLL files..." -ForegroundColor Gray
    
    $dlls = @(
        "sfml-graphics-3.dll",
        "sfml-audio-3.dll",
        "sfml-window-3.dll",
        "sfml-system-3.dll",
        "openal32.dll"
    )
    
    foreach ($dll in $dlls) {
        $source = "$SFML_PATH\bin\$dll"
        if (Test-Path $source) {
            Copy-Item $source . -ErrorAction SilentlyContinue
            Write-Host "  ✓ $dll" -ForegroundColor Gray
        }
    }
    
    Write-Host ""
    Write-Host "============================================" -ForegroundColor Green
    Write-Host "  READY TO RUN" -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Your executable: stick-dance.exe" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "To run: .\stick-dance.exe" -ForegroundColor White
    Write-Host ""
    Write-Host "Controls:" -ForegroundColor Yellow
    Write-Host "  SPACE - Start/Stop"
    Write-Host "  K     - Full Speed Mode"
    Write-Host "  T     - Turbo (1.25x)"
    Write-Host "  M     - Mute"
    Write-Host "  F     - Finale"
    Write-Host "  R     - Reset"
    Write-Host "  ESC   - Quit"
    Write-Host ""
    
    # Ask if user wants to run
    $run = Read-Host "Run now? (y/n)"
    if ($run -eq "y") {
        .\stick-dance.exe
    }
} else {
    Write-Host ""
    Write-Host "============================================" -ForegroundColor Red
    Write-Host "  ✗ BUILD FAILED" -ForegroundColor Red
    Write-Host "============================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please check:" -ForegroundColor Yellow
    Write-Host "  1. SFML is at C:\SFML-3.0"
    Write-Host "  2. main.cpp exists"
    Write-Host "  3. g++ compiler is in your PATH"
    Write-Host ""
    Read-Host "Press Enter to exit"
    exit 1
}
