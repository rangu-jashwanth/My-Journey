# SETUP VERIFICATION CHECKLIST

Use this checklist to verify your environment is properly configured for building and running Stick Shift.

---

## Phase 1: System Preparation

- [ ] Windows 10 or 11 (64-bit)
- [ ] Administrator access to install software
- [ ] At least 2 GB free disk space
- [ ] Internet connection for downloading tools

---

## Phase 2: MinGW-w64 Installation

- [ ] Downloaded MinGW-w64 (GCC, x86_64, posix, seh)
- [ ] Extracted to `C:\mingw64` (or verified your install path)
- [ ] Verified folder structure:
  ```
  C:\mingw64\
  ├── bin\           (contains g++.exe, make.exe)
  ├── lib\
  ├── include\
  └── ...
  ```
- [ ] Tested g++ works:
  ```bash
  C:\mingw64\bin\g++ --version
  ```
  Should show version info (not "command not found")

---

## Phase 3: SFML Installation

- [ ] Downloaded SFML 3.x (GCC 13.1.0 MinGW (SEH) 64-bit)
- [ ] Extracted to `C:\SFML-3.0` (or verified your install path)
- [ ] Verified folder structure:
  ```
  C:\SFML-3.0\
  ├── bin\           (contains *.dll files)
  ├── lib\           (contains *.a and *.a.import files)
  ├── include\       (contains SFML/ subfolder)
  └── doc\
  ```
- [ ] Verified header files exist:
  ```
  C:\SFML-3.0\include\SFML\Graphics.hpp
  C:\SFML-3.0\include\SFML\Audio.hpp
  C:\SFML-3.0\include\SFML\Window.hpp
  C:\SFML-3.0\include\SFML\System.hpp
  ```
- [ ] Verified library files exist:
  ```
  C:\SFML-3.0\lib\libsfml-graphics.a
  C:\SFML-3.0\lib\libsfml-audio.a
  C:\SFML-3.0\lib\libsfml-window.a
  C:\SFML-3.0\lib\libsfml-system.a
  ```
- [ ] Verified DLL files exist:
  ```
  C:\SFML-3.0\bin\sfml-graphics-3.dll
  C:\SFML-3.0\bin\sfml-audio-3.dll
  C:\SFML-3.0\bin\sfml-window-3.dll
  C:\SFML-3.0\bin\sfml-system-3.dll
  C:\SFML-3.0\bin\openal32.dll
  ```

---

## Phase 4: Windows PATH Configuration

- [ ] Added `C:\mingw64\bin` to Windows PATH:
  - Open PowerShell as Administrator
  - Run: `[Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\mingw64\bin", "User")`
  - Close and reopen PowerShell
- [ ] Verified g++ is in PATH:
  ```bash
  g++ --version
  ```
  Should show version without full path

---

## Phase 5: Project Files

- [ ] Verified all required files exist in project folder:
  - [ ] main.cpp
  - [ ] build.bat
  - [ ] SFML_SETUP_WINDOWS.md
  - [ ] CONFIGURATION.md
  - [ ] README_QUICK_REF.md
  - [ ] README.md

---

## Phase 6: Initial Build Test

- [ ] Opened command prompt in project folder:
  ```bash
  cd /d C:\path\to\project
  ```
- [ ] Created test file (optional - for verification):
  ```bash
  echo #include ^<SFML/Graphics.hpp^> > test.cpp
  echo int main() { return 0; } >> test.cpp
  ```
- [ ] Verified compilation command:
  ```bash
  g++ -std=c++17 test.cpp -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" -lsfml-graphics -lsfml-window -lsfml-system
  ```
  Should produce `a.exe` with no errors

---

## Phase 7: Main Build

- [ ] Modified `build.bat` to match your paths (if non-standard):
  ```batch
  set SFML_PATH=C:\SFML-3.0
  set MINGW_PATH=C:\mingw64
  ```
- [ ] Ran `build.bat`:
  ```bash
  build.bat
  ```
- [ ] Verified compilation succeeded - look for:
  ```
  ========== BUILD SUCCESSFUL ==========
  Created: stick-dance.exe
  ```
- [ ] Verified `stick-dance.exe` was created (check file size > 1 MB)

---

## Phase 8: DLL Files

- [ ] Copied all required DLL files to project folder:
  - [ ] `sfml-graphics-3.dll`
  - [ ] `sfml-audio-3.dll`
  - [ ] `sfml-window-3.dll`
  - [ ] `sfml-system-3.dll`
  - [ ] `openal32.dll`
  
  Or verify `build.bat` copied them automatically

- [ ] Verified DLL files are in project folder:
  ```bash
  ls *.dll
  ```
  Should list 5 DLL files

---

## Phase 9: Audio File

- [ ] Obtained an MP3 or OGG audio file (10+ seconds recommended)
- [ ] Placed audio file as `song.mp3` in project folder:
  ```bash
  copy C:\path\to\music.mp3 song.mp3
  ```
- [ ] Verified file exists:
  ```bash
  ls song.mp3
  ```
  Should show file with size > 1 MB

---

## Phase 10: First Run

- [ ] Ran the program from command prompt:
  ```bash
  stick-dance.exe
  ```
- [ ] Verified window opened with:
  - [ ] "STICK SHIFT" title at top
  - [ ] Grid background visible
  - [ ] 6 stick figures on stage
  - [ ] Animation synchronized to music
  - [ ] UI showing time, BPM, controls

- [ ] Tested controls:
  - [ ] SPACE key pauses/resumes music
  - [ ] K key skips forward 10 seconds
  - [ ] T key toggles turbo mode
  - [ ] M key toggles mirror mode
  - [ ] F key shows flash effect
  - [ ] R key restarts song
  - [ ] ESC key closes program

---

## Phase 11: Troubleshooting (if needed)

### If build fails:
- [ ] Checked error message in console
- [ ] Verified SFML_PATH in build.bat is correct
- [ ] Verified SFML headers exist at path
- [ ] Tried manual compile command (see SFML_SETUP_WINDOWS.md)

### If program won't start:
- [ ] Verified all 5 DLL files are in project folder
- [ ] Checked DLL file sizes (should be > 100 KB each)
- [ ] Verified `song.mp3` exists and is readable
- [ ] Tried copying DLLs manually from SFML\bin

### If audio doesn't play:
- [ ] Verified `openal32.dll` is in project folder
- [ ] Tried different audio format (OGG instead of MP3)
- [ ] Checked system audio volume is not muted
- [ ] Tried different audio file

### If UI text doesn't show:
- [ ] Verified `C:\Windows\Fonts\Arial.ttf` exists
- [ ] Checked font path in `UIRenderer` constructor is correct

### If animation is choppy:
- [ ] Reduced `DANCER_COUNT` in main.cpp to 3
- [ ] Closed other running programs
- [ ] Checked CPU usage (Task Manager)
- [ ] Recompiled with `-O3` instead of `-O2`

---

## Phase 12: Verification Summary

### Success Indicators ✅
- [ ] `g++ --version` works without full path
- [ ] SFML headers and libraries found at paths
- [ ] `build.bat` completes without errors
- [ ] `stick-dance.exe` file created (> 1 MB)
- [ ] All 5 DLL files in project folder
- [ ] `song.mp3` present and readable
- [ ] Program window opens and displays dancers
- [ ] Animation synchronized to music
- [ ] All keyboard controls respond
- [ ] No console error messages

### System Ready ✅
- If all boxes above are checked, your system is properly configured
- You can now:
  - [ ] Customize choreography (edit POSES)
  - [ ] Change BPM to match different songs
  - [ ] Modify colors and visual effects
  - [ ] Add custom dance moves

---

## Quick Verification Commands

Copy and paste these commands to verify setup:

### Check compiler
```bash
g++ --version
```
Expected: GCC version info

### Check SFML headers
```bash
dir "C:\SFML-3.0\include\SFML\Graphics.hpp"
```
Expected: File found

### Check SFML libraries
```bash
dir "C:\SFML-3.0\lib\libsfml-graphics.a"
```
Expected: File found

### Check project files
```bash
dir main.cpp build.bat stick-dance.exe
```
Expected: All three files listed

### Test compilation
```bash
g++ -std=c++17 -o test.exe main.cpp -I"C:\SFML-3.0\include" -L"C:\SFML-3.0\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -mwindows
```
Expected: Compiles without errors

---

## Next Steps After Verification

Once all checkboxes are complete:

1. **Read customization guide**: Open CONFIGURATION.md
2. **Create new choreography**: Edit POSES array in main.cpp
3. **Try different songs**: Place new MP3, adjust BPM
4. **Experiment with effects**: Modify colors, dancer count, window size
5. **Build and test**: Run build.bat, test with stick-dance.exe

---

## Common Path Issues

If you installed in different locations, update these variables in `build.bat`:

```batch
set SFML_PATH=C:\path\to\your\sfml
set MINGW_PATH=C:\path\to\your\mingw
```

Then re-run build.bat.

---

## Support Resources

- **SFML Setup**: See SFML_SETUP_WINDOWS.md
- **Troubleshooting**: See CONFIGURATION.md
- **Quick Reference**: See README_QUICK_REF.md

---

**Mark completion date:** _______________

**When all checks above are complete, your system is ready to build and run Stick Shift!** 🎉
