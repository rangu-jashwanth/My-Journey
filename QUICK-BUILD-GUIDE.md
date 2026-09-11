# Quick Start: Building Stick Shift C++ 

**Your system has:** ✓ MinGW compiler (g++ available)
**Still needed:** SFML 3.0 library

## Option 1: Use Online IDE (Fastest - 5 minutes)

If you want to skip local setup entirely, you can compile and run in the cloud:

1. Go to https://replit.com (create free account)
2. Create new C++ project
3. Upload your `main.cpp` and `Yeshanagula...mp3` files
4. In the shell, run:
   ```bash
   apt-get install libsfml-dev
   g++ -std=c++17 -O2 -o stick-dance main.cpp -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
   ./stick-dance
   ```

## Option 2: Local Windows Setup (Recommended - 20 minutes)

### Step 1: Download SFML 3.0 (2 minutes)

Go to https://github.com/SFML/SFML/releases and:
1. Find the **3.0** release tag (latest)
2. Scroll down to **Assets**
3. Download: `SFML-3.0-windows-gcc-13.1.0-mingw-64bit.zip`
   - Size: ~30 MB
   - This is the GCC/MinGW build you need

### Step 2: Extract SFML (1 minute)

1. Right-click the ZIP file → Extract All
2. Extract to: `C:\SFML-3.0`
   - **IMPORTANT:** Extract to the ROOT of C:\ drive, NOT inside another folder
3. Verify: You should have:
   ```
   C:\SFML-3.0\
   ├── include\
   ├── lib\
   ├── bin\
   └── ...
   ```

### Step 3: Compile (3 minutes)

Open PowerShell in your project folder and run:

```powershell
$SFML="C:\SFML-3.0"
g++ -std=c++17 -O2 -o stick-dance.exe main.cpp `
  -I"$SFML\include" `
  -L"$SFML\lib" `
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system `
  -mwindows
```

If successful, you'll see: `stick-dance.exe` appears in your folder

### Step 4: Copy DLLs (1 minute)

Copy these files from `C:\SFML-3.0\bin\` to your project folder:
- `sfml-graphics-3.dll`
- `sfml-audio-3.dll`
- `sfml-window-3.dll`
- `sfml-system-3.dll`
- `openal32.dll`

Or use PowerShell:
```powershell
Copy-Item "C:\SFML-3.0\bin\sfml-*.dll" .
Copy-Item "C:\SFML-3.0\bin\openal32.dll" .
```

### Step 5: Run (1 minute)

Double-click `stick-dance.exe` or run:
```powershell
.\stick-dance.exe
```

You should see 6 stick dancers performing!

## Option 3: Using Makefile

If you prefer `make` command:

```bash
make clean
make
make run
```

## If SFML Download Fails

**Alternative mirrors:**
1. https://github.com/SFML/SFML/releases (official)
2. https://github.com/SFML/SFML/releases/download/3.0/SFML-3.0-windows-gcc-13.1.0-mingw-64bit.zip (direct link)

**Manual installation:**
1. Create folder: `C:\SFML-3.0`
2. Download from one of the links above
3. Extract the contents into that folder

## Compilation Troubleshooting

| Error | Solution |
|-------|----------|
| `g++ not found` | MinGW not in PATH - reinstall MinGW or add to PATH |
| `SFML/Graphics.hpp not found` | Check SFML extracted to C:\SFML-3.0 |
| `sfml-graphics not found` | Check SFML lib\ folder exists and has libsfml-graphics.a |
| `undefined reference to main` | You have main.cpp in the folder |
| `.exe works but shows black screen` | Missing song.mp3 - copy it to your folder |

## Questions?

Refer to these detailed guides:
- **Setup details:** See `SFML_SETUP_WINDOWS.md`
- **Configuration:** See `CONFIGURATION.md`
- **How to customize:** See `README_QUICK_REF.md`

---

**Once compiled and running:**
- **SPACE** - Start/Stop animation
- **K** - Toggle full speed (Yeshanagula song)
- **T** - Toggle turbo (1.25x speed)
- **M** - Toggle mute
- **F** - Toggle finale mode
- **R** - Reset
- **ESC** - Quit
