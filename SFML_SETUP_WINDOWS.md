# SFML 3.x Setup on Windows with g++

## Prerequisites
- **MinGW-w64** (GCC compiler for Windows)
  - Download: https://www.mingw-w64.org/downloads/
  - Choose: x86_64-posix-seh (64-bit, POSIX threads, SEH exception handling)
  - Installation: Extract to `C:\mingw64`

- **SFML 3.x**
  - Download: https://www.sfml-dev.org/download/sfml/3.0/
  - Choose: GCC 13.1.0 MinGW (SEH) - 64-bit
  - Extract to a known location (e.g., `C:\SFML-3.0`)

## 1. Install MinGW-w64

### Option A: Using the installer
1. Download from https://www.mingw-w64.org/downloads/
2. Run installer, select:
   - Version: Latest (e.g., 11.0.0)
   - Architecture: x86_64
   - Threads: posix
   - Exception handling: seh
3. Install to `C:\mingw64`

### Option B: Portable ZIP (Recommended for VS Code)
1. Download pre-built ZIP from https://github.com/niXman/mingw-builds-binaries/releases
2. Extract to `C:\mingw64`
3. Verify: Open PowerShell and run:
   ```powershell
   C:\mingw64\bin\g++ --version
   ```
   Should output: `g++ (MinGW-W64 x.x.x-posix-seh, built by niXman project) X.X.X`

## 2. Install SFML 3.x

1. Download SFML 3.x from https://www.sfml-dev.org/download/sfml/3.0/
2. Extract to `C:\SFML-3.0` (or your preferred location)
3. Folder structure should be:
   ```
   C:\SFML-3.0\
   ├── bin\          (DLLs: sfml-graphics-3.dll, sfml-audio-3.dll, etc.)
   ├── lib\          (Static libraries and import libs)
   ├── include\      (SFML headers)
   └── doc\
   ```

## 3. Add MinGW to PATH (Permanent)

### Using PowerShell (Admin):
```powershell
$env:Path += ";C:\mingw64\bin"
[Environment]::SetEnvironmentVariable("Path", $env:Path, "User")
```

Verify it worked:
```powershell
g++ --version
```

## 4. Copy SFML Runtime DLLs to Project

Copy these files from `C:\SFML-3.0\bin\` to your project folder:
- `sfml-graphics-3.dll`
- `sfml-audio-3.dll`
- `sfml-window-3.dll`
- `sfml-system-3.dll`
- `sfml-network-3.dll` (if needed)
- `openal32.dll` (for audio)

Your project structure should be:
```
stick-dance-cpp/
├── main.cpp
├── song.mp3
├── sfml-graphics-3.dll
├── sfml-audio-3.dll
├── sfml-window-3.dll
├── sfml-system-3.dll
└── build.bat (or Makefile)
```

## 5. Compilation Command

### Command Line (PowerShell/CMD)

**Single-line compile:**
```bash
g++ -std=c++17 -O2 -o stick-dance.exe main.cpp -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -mwindows
```

**With debug info:**
```bash
g++ -std=c++17 -g -o stick-dance.exe main.cpp -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -mwindows
```

### Build Script (Windows)

Create `build.bat` in your project folder:
```batch
@echo off
set SFML_PATH=C:\SFML-3.0
set MINGW_PATH=C:\mingw64

%MINGW_PATH%\bin\g++ -std=c++17 -O2 -o stick-dance.exe main.cpp ^
  -I"%SFML_PATH%\include" ^
  -L"%SFML_PATH%\lib" ^
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system ^
  -mwindows

echo.
if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run: stick-dance.exe
) else (
    echo Build failed with errors.
)
```

Run it:
```bash
./build.bat
```

### VS Code Configuration

Create `.vscode/tasks.json`:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build C++ (SFML)",
            "type": "shell",
            "command": "g++",
            "args": [
                "-std=c++17",
                "-O2",
                "-o", "stick-dance.exe",
                "main.cpp",
                "-I${workspaceFolder}/include",
                "-IC:/SFML-3.0/include",
                "-L${workspaceFolder}",
                "-LC:/SFML-3.0/lib",
                "-lsfml-graphics",
                "-lsfml-audio",
                "-lsfml-window",
                "-lsfml-system",
                "-mwindows"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "presentation": {
                "reveal": "always"
            },
            "problemMatcher": []
        },
        {
            "label": "Run",
            "type": "shell",
            "command": "${workspaceFolder}/stick-dance.exe",
            "group": {
                "kind": "test"
            }
        }
    ]
}
```

Then in VS Code:
- Press `Ctrl+Shift+B` to build
- Press `F5` or `Ctrl+Shift+P` → "Run Task" → "Run" to execute

## 6. Troubleshooting

### "fatal error: SFML/Graphics.hpp: No such file"
- Check SFML_PATH in your build command points to correct folder
- Verify `C:\SFML-3.0\include\SFML\` exists

### "undefined reference to `sf::Texture::loadFromFile'"
- Ensure library paths are correct (`-L"C:\SFML-3.0\lib"`)
- Ensure you have `-lsfml-graphics -lsfml-window -lsfml-system` in order

### "DLL not found" at runtime
- Copy DLL files from `C:\SFML-3.0\bin\` to your project folder
- Or add `C:\SFML-3.0\bin` to Windows PATH

### OpenAL error with audio
- Copy `openal32.dll` to project folder from SFML bin
- If still missing, download from: https://www.openal.org/

## 7. Testing Your Setup

Create a minimal test file `test.cpp`:
```cpp
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");
    std::cout << "SFML window created successfully!" << std::endl;
    window.close();
    return 0;
}
```

Compile:
```bash
g++ -std=c++17 -o test.exe test.cpp -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" -lsfml-graphics -lsfml-window -lsfml-system -mwindows
```

Run: `test.exe`

If successful, you're ready to compile the stick dance program!

## 8. API Differences (SFML 3.x vs 2.x)

If you encounter API issues with SFML 3.x:
- `sf::Vector2f` replaced with `sf::Vector2<float>`
- `sf::Color` constructor is the same: `sf::Color(r, g, b, a)`
- `sf::Clock` is the same
- `sf::Music::openFromFile()` replaced with `sf::Music::open()`
- `sf::Font::loadFromFile()` replaced with `sf::Font::openFromFile()`
- `sf::RenderWindow::draw()` is the same

The main.cpp provided uses SFML 3.x-compatible code.
