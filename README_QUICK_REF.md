# Quick Reference Guide - Stick Shift C++

## One-Minute Setup

1. **Install MinGW-w64**: Download from https://www.mingw-w64.org/downloads/, extract to `C:\mingw64`
2. **Install SFML 3.x**: Download from https://www.sfml-dev.org/, extract to `C:\SFML-3.0`
3. **Add MinGW to PATH**:
   ```powershell
   $env:Path += ";C:\mingw64\bin"
   [Environment]::SetEnvironmentVariable("Path", $env:Path, "User")
   ```
4. **Copy files** (see SFML_SETUP_WINDOWS.md)
5. **Run build**:
   ```bash
   build.bat
   ```
6. **Place song.mp3** in the folder
7. **Run** `stick-dance.exe`

## File Reference

| File | Purpose |
|------|---------|
| `main.cpp` | Complete C++17 source code with SFML |
| `build.bat` | Windows batch build script |
| `SFML_SETUP_WINDOWS.md` | Detailed setup instructions |
| `CONFIGURATION.md` | Full customization guide |
| `stick-dance.exe` | Compiled executable (after build) |
| `song.mp3` | Your audio file (place here) |
| `*.dll` | SFML runtime libraries |

## Compilation Quick Links

### Standard Build
```bash
build.bat
```

### Manual g++ Command
```bash
g++ -std=c++17 -O2 -o stick-dance.exe main.cpp \
  -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" \
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -mwindows
```

### Debug Build
```bash
g++ -std=c++17 -g -o stick-dance-debug.exe main.cpp \
  -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" \
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

## Runtime Controls

| Key | Action |
|-----|--------|
| `SPACE` | Pause / Resume |
| `K` | Skip forward 10 seconds |
| `T` | Toggle Turbo mode (faster BPM) |
| `M` | Toggle Mirror mode (flip dancers) |
| `F` | Flash effect |
| `R` | Restart song |
| `ESC` | Quit |

## Key Configuration Values

Edit at top of `main.cpp`:

```cpp
const float BASE_BPM = 192.0f;              // Song tempo
const float TURBO_BPM = 240.0f;             // Fast mode tempo
const float SONG_START_OFFSET = 35.0f;      // Intro skip (seconds)
const int DANCER_COUNT = 6;                 // Number of dancers
const unsigned int WINDOW_WIDTH = 1280;     // Screen width
const unsigned int WINDOW_HEIGHT = 720;     // Screen height
```

## Customization Checklist

- [ ] Install MinGW and SFML (see SFML_SETUP_WINDOWS.md)
- [ ] Run `build.bat` successfully
- [ ] Copy DLL files to project folder
- [ ] Place `song.mp3` in project folder
- [ ] Run `stick-dance.exe` - see dancers move!
- [ ] Adjust `BASE_BPM` if animation is too fast/slow
- [ ] Modify `POSES` array to create custom choreography
- [ ] Change colors and sizes if desired
- [ ] Rebuild with `build.bat` after changes

## Common Customizations

### Change Song
```
1. Replace song.mp3 with your audio file
2. Run stick-dance.exe
```

### Change BPM
```cpp
// In main.cpp, top section:
const float BASE_BPM = 150.0f;  // Change to your song's BPM
// Then: build.bat
```

### Add New Dance Move
```cpp
// In main.cpp, find POSES array, add new Pose:
{Vector2D(x, y), Vector2D(x, y), ...},  // New pose
// Then: build.bat
```

### Change Number of Dancers
```cpp
const int DANCER_COUNT = 4;  // Instead of 6
// Then: build.bat
```

### Change Window Size
```cpp
const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;
// Then: build.bat
```

## Structure Overview

```
Project Folder
├── main.cpp                    ← Edit for customization
├── build.bat                   ← Run to compile
├── SFML_SETUP_WINDOWS.md       ← Setup help
├── CONFIGURATION.md            ← Detailed guide
├── README_QUICK_REF.md         ← This file
│
├── song.mp3                    ← Place your audio here
├── stick-dance.exe             ← Executable (after build)
│
├── sfml-graphics-3.dll         ← SFML libraries
├── sfml-audio-3.dll            ├─ Copy from C:\SFML-3.0\bin
├── sfml-window-3.dll           │  (Or run build.bat)
├── sfml-system-3.dll           │
└── openal32.dll                └─ For audio
```

## Features

✅ 60 FPS smooth animation  
✅ Beat-synchronized choreography  
✅ 12 dance poses with interpolation  
✅ 6 dancers with phase offsets  
✅ Multiple game modes (Normal, Turbo, Mirror, Finale)  
✅ Dynamic lighting and effects  
✅ Music playback with SFML Audio  
✅ Real-time UI with beat visualization  
✅ Keyboard controls for interactivity  
✅ Configurable BPM and choreography  
✅ Windows-optimized build process  

## Key Differences from Python Version

| Feature | Python | C++ |
|---------|--------|-----|
| Graphics | Pygame 2D | SFML 3.x |
| Performance | Interpretted | Compiled (~10x faster) |
| Audio | pygame.mixer | SFML Audio |
| Code Structure | Functional | Object-oriented |
| Setup | `pip install pygame` | Full SFML installation |
| Portability | Windows/Mac/Linux | Windows (easily ported) |
| Customization | Edit .py | Edit .cpp + rebuild |
| Extensibility | Limited | Full C++ power |

## Troubleshooting Quick Links

| Problem | Solution |
|---------|----------|
| Build fails | Check SFML_PATH in build.bat |
| DLL not found | Copy from `C:\SFML-3.0\bin\` |
| No sound | Ensure openal32.dll present |
| No song found | Place .mp3 in project folder |
| Choppy animation | Reduce DANCER_COUNT or increase optimization (-O3) |
| Text doesn't show | Verify `C:\Windows\Fonts\Arial.ttf` exists |

See `CONFIGURATION.md` for detailed troubleshooting.

## Building on Different Compilers

### MSVC (Visual Studio)
```bash
cl /std:c++17 /O2 /I"C:\SFML-3.0\include" main.cpp ^
   /link /LIBPATH:"C:\SFML-3.0\lib" sfml-graphics.lib sfml-audio.lib sfml-window.lib sfml-system.lib
```

### Clang (Windows)
```bash
clang++ -std=c++17 -O2 -o stick-dance.exe main.cpp \
  -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" \
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

### GCC (Linux - requires SFML 3.x for Linux)
```bash
g++ -std=c++17 -O2 -o stick-dance main.cpp \
  -I/usr/include/SFML -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

## Next Steps

1. **Read full guides:**
   - SFML_SETUP_WINDOWS.md (installation)
   - CONFIGURATION.md (customization)

2. **Customize your version:**
   - Edit main.cpp configuration section
   - Build with build.bat
   - Test and iterate

3. **Share your creation:**
   - Include main.cpp, build.bat, song.mp3
   - Document any custom BPM/choreography changes

## Tips & Tricks

- **Fast iteration:** Keep build.bat window open, modify main.cpp, run build.bat again
- **Backup originals:** Copy main.cpp to main.cpp.bak before major changes
- **Debug prints:** Remove `-mwindows` flag to see console output
- **Performance profiling:** Use Windows Task Manager to watch CPU usage
- **Song analysis:** Use Audacity or online BPM detector to find exact tempo

---

**For complete documentation, see CONFIGURATION.md**

**For setup help, see SFML_SETUP_WINDOWS.md**
