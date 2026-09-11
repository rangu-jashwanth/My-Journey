# Stick Shift C++ - Configuration and Customization Guide

## Table of Contents
1. [Quick Start](#quick-start)
2. [Project Structure](#project-structure)
3. [Compilation Commands](#compilation-commands)
4. [Configuration](#configuration)
5. [Adding/Changing Songs](#addingchanging-songs)
6. [Modifying BPM](#modifying-bpm)
7. [Creating New Choreography](#creating-new-choreography)
8. [Performance Tuning](#performance-tuning)
9. [Troubleshooting](#troubleshooting)

---

## Quick Start

### Prerequisites
- MinGW-w64 installed at `C:\mingw64`
- SFML 3.x installed at `C:\SFML-3.0`
- An MP3 audio file (`song.mp3` or any `.mp3`/`.ogg` in the folder)

### Step 1: Prepare Your Environment
Follow the setup guide in [SFML_SETUP_WINDOWS.md](SFML_SETUP_WINDOWS.md)

### Step 2: Compile
```bash
# Use the batch script:
build.bat

# OR compile manually:
g++ -std=c++17 -O2 -o stick-dance.exe main.cpp \
  -I"C:\SFML-3.0\include" \
  -L"C:\SFML-3.0\lib" \
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -mwindows
```

### Step 3: Copy Audio and DLLs
The build script does this automatically. Manually:
```bash
# Copy DLLs from SFML to your project folder:
xcopy "C:\SFML-3.0\bin\*.dll" . /Y

# Place your song in the folder:
copy "C:\path\to\your\song.mp3" song.mp3
```

### Step 4: Run
```bash
stick-dance.exe
```

---

## Project Structure

Expected folder layout:
```
stick-dance-cpp/
├── main.cpp                      (Source code)
├── build.bat                     (Build script)
├── song.mp3                      (Your audio file - required)
├── SFML_SETUP_WINDOWS.md         (Setup instructions)
├── CONFIGURATION.md              (This file)
│
├── sfml-graphics-3.dll           (SFML runtime DLLs)
├── sfml-audio-3.dll              (Copy from SFML\bin)
├── sfml-window-3.dll
├── sfml-system-3.dll
├── openal32.dll                  (For audio support)
│
└── stick-dance.exe               (Compiled executable)
```

---

## Compilation Commands

### Option 1: Windows Batch Script (Recommended)
```bash
build.bat
```
This automatically:
- Compiles with C++17
- Links SFML libraries
- Copies required DLLs
- Runs the executable if song.mp3 exists

### Option 2: Direct g++ Command
```bash
g++ -std=c++17 -O2 -o stick-dance.exe main.cpp \
  -I"C:\SFML-3.0\include" \
  -L"C:\SFML-3.0\lib" \
  -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system \
  -mwindows
```

**Flags explained:**
- `-std=c++17`: Use C++17 standard
- `-O2`: Enable optimizations for better performance
- `-o stick-dance.exe`: Output filename
- `-I"..."`: Include path for SFML headers
- `-L"..."`: Library path for SFML binaries
- `-lsfml-*`: Link SFML libraries
- `-mwindows`: Hide console window (remove for debug output)

### Option 3: VS Code Tasks
Create `.vscode/tasks.json`:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Stick Shift",
            "type": "shell",
            "command": "build.bat",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "presentation": {
                "reveal": "always"
            }
        }
    ]
}
```

Then press `Ctrl+Shift+B` to build.

### Option 4: Makefile
Create `Makefile`:
```makefile
CXX = g++
CXXFLAGS = -std=c++17 -O2
SFML_PATH = C:\SFML-3.0
SFML_INCLUDE = -I"$(SFML_PATH)\include"
SFML_LIB = -L"$(SFML_PATH)\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
OUTPUT = stick-dance.exe
SOURCE = main.cpp

all: $(OUTPUT)

$(OUTPUT): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_INCLUDE) $(SFML_LIB) -mwindows
	@echo Build complete!

clean:
	del $(OUTPUT)

run: $(OUTPUT)
	$(OUTPUT)

.PHONY: all clean run
```

Then use:
```bash
make              # Compile
make run          # Compile and run
make clean        # Remove executable
```

---

## Configuration

All configuration values are at the top of `main.cpp` in the "CONFIGURATION" section:

### Timing & Music
```cpp
const float BASE_BPM = 192.0f;           // Base beats per minute
const float TURBO_BPM = 240.0f;          // BPM for turbo mode (press T)
const float SKIP_SECONDS = 10.0f;        // Seconds to skip (press K)
const float SONG_START_OFFSET = 35.0f;   // Seconds before choreography starts
const std::string SONG_FILENAME = "song.mp3";  // Audio file to load
```

### Display
```cpp
const unsigned int WINDOW_WIDTH = 1280;  // Window width in pixels
const unsigned int WINDOW_HEIGHT = 720;  // Window height in pixels
const unsigned int FPS = 60;             // Target frame rate
```

### Dancers
```cpp
const int DANCER_COUNT = 6;              // Number of dancers on stage
const float STAGE_Y = 438.0f;            // Vertical position of dancers
```

### Dance Poses
```cpp
const float POSE_BEATS = 1.0f;           // Duration of each pose in beats
const int BEATS_PER_MEASURE = 4;         // Beats in a musical measure
```

### Visual Elements
```cpp
const int HEAD_RADIUS = 29;              // Size of head circle
const int JOINT_RADIUS = 7;              // Size of joint circles
const int LINE_WIDTH = 9;                // Thickness of limb lines
```

### Colors (RGB format)
```cpp
const sf::Color BACKGROUND = sf::Color(11, 16, 28);     // Dark blue background
const sf::Color MINT = sf::Color(98, 231, 190);         // Cyan color
const sf::Color PINK = sf::Color(255, 111, 166);        // Pink color
const sf::Color YELLOW = sf::Color(255, 210, 92);       // Yellow color
const sf::Color BLUE = sf::Color(100, 177, 255);        // Light blue color
```

---

## Adding/Changing Songs

### Method 1: Automatic Detection (Recommended)
The program automatically finds the first `.mp3` or `.ogg` file in the folder:

1. Place your audio file in the project folder
2. Name it anything (e.g., `my_song.mp3`)
3. Run the program - it will find it automatically

### Method 2: Specific Filename
Edit the configuration in `main.cpp`:
```cpp
const std::string SONG_FILENAME = "my_custom_song.mp3";
```

Then recompile and place your file with that exact name.

### Supported Formats
- MP3 (`.mp3`) - Recommended
- OGG Vorbis (`.ogg`) - Also supported
- FLAC and WAV may work depending on SFML build

### Important Notes
- Audio file must be in the same folder as the executable
- If no file is found, the program shows "No MP3 song found" and exits
- You can press K to skip forward 10 seconds while playing

### Finding Good Songs
Your Python version uses "Yeshanagula" at 192 BPM. For other songs:
1. Find BPM (try https://www.tunebat.com)
2. Adjust `BASE_BPM` in the code
3. Try TURBO mode (press T) for faster songs

---

## Modifying BPM

### Change Base BPM
Edit in `main.cpp`:
```cpp
const float BASE_BPM = 192.0f;  // Change this value
```

**Common BPMs:**
- 120 BPM: Standard pop/dance
- 140 BPM: Upbeat pop
- 160 BPM: Fast EDM
- 180-200 BPM: Very fast EDM/drum & bass

### Change Turbo BPM
For the turbo mode (press T during playback):
```cpp
const float TURBO_BPM = 240.0f;  // Faster version
```

### How BPM Affects Animation
- `beat = song_time * effective_bpm / 60.0`
- Each "beat" moves to the next pose in the `POSES` array
- Higher BPM = faster animation
- Lower BPM = slower, more emphasized movements

### Finding the Right BPM
1. Play your song in Python version (if you have tempo info)
2. Use an online BPM detector: https://www.tunebat.com
3. Count beats: "1-2-3-4" to 10 seconds, multiply by 6
4. Start with estimated BPM, adjust in turbo mode with T key

### Example: Matching Song Tempo
```cpp
// For a song at 160 BPM:
const float BASE_BPM = 160.0f;
const float TURBO_BPM = 200.0f;  // 160 * 1.25 for energetic mode
```

Recompile and test. Use K key to skip around and find optimal BPM.

---

## Creating New Choreography

### Understanding Poses

Each pose is a keyframe defining joint positions. The animation interpolates smoothly between consecutive poses.

Pose structure (in `main.cpp`):
```cpp
struct Pose {
    Vector2D head, neck;                    // Head position
    Vector2D left_elbow, left_hand;         // Left arm joints
    Vector2D right_elbow, right_hand;       // Right arm joints
    Vector2D left_knee, left_foot;          // Left leg joints
    Vector2D right_knee, right_foot;        // Right leg joints
    float bob;                              // Vertical bounce
    float lean;                             // Left/right lean
    float facing;                           // Mirror/facing direction
};
```

All positions are **offsets from the hip** (center of character).

### Coordinate System
```
        Head (0, -174)
          |
        Neck (0, -142)
       /    \
    Left    Right
   Elbow   Elbow
   /         \
 Hand       Hand
   
   Hip (0, 0)  <- Center
   /       \
Knee     Knee
 |         |
Foot     Foot
```

- X: Negative = left, Positive = right
- Y: Negative = above hip, Positive = below hip
- Typical standing pose: Head at (0, -180), Feet at (±40, 100)

### Creating New Poses

Example: Adding a jumping pose

1. Open `main.cpp` in your editor
2. Find the `POSES` array:
```cpp
const std::vector<Pose> POSES = {
    // Existing poses...
    {Vector2D(0, -174), Vector2D(0, -142), ...},
    ...
};
```

3. Add a new pose:
```cpp
const std::vector<Pose> POSES = {
    // Existing poses...
    
    // NEW: Jump pose with arms up
    {Vector2D(0, -200), Vector2D(0, -160),  // Head higher (jumping)
     Vector2D(-70, -150), Vector2D(-110, -240),  // Left arm up
     Vector2D(70, -150), Vector2D(110, -240),    // Right arm up
     Vector2D(-20, 20), Vector2D(-30, 90),       // Left leg bent
     Vector2D(20, 20), Vector2D(30, 90),         // Right leg bent
     20, 0, 1.0f},  // High bob value, no lean, normal facing
};
```

4. Recompile:
```bash
build.bat
```

5. Test the new pose by pressing R to restart - the animation will cycle through your new pose

### Editing Existing Poses

Modify any pose in the array:
```cpp
// Original pose 0:
{Vector2D(0, -174), Vector2D(0, -142), ...}

// Modified to lean more to the left:
{Vector2D(0, -174), Vector2D(-10, -142), ...}  // Neck moved left
```

### Tips for Creating Good Poses

1. **Keep proportions realistic:**
   - Head is small (radius 29)
   - Arms are 100-150 pixels long
   - Legs are 130-150 pixels long

2. **Use symmetry for stable poses:**
   ```cpp
   Vector2D(-50, 100)   // Left
   Vector2D(50, 100)    // Right (mirror)
   ```

3. **Add variation to avoid repetition:**
   - Vary lean: -20 to +20
   - Vary bob: 0 to 20 (vertical bounce)
   - Vary facing: 0.85 to 1.15 (mirror effect)

4. **Test gradually:**
   - Add one pose at a time
   - Use R key to restart and see changes
   - Adjust positions if limbs intersect awkwardly

### Advanced: Pose Interpolation

The animation automatically creates smooth transitions between poses using `smoothstep()`:
```cpp
float smoothstep(float t) {
    return t * t * (3.0f - 2.0f * t);  // Smooth easing function
}
```

This ensures movement isn't jerky. Poses are interpolated over `POSE_BEATS` duration (default 1 beat).

### Complete Custom Choreography Example

Replace entire `POSES` array:
```cpp
const std::vector<Pose> POSES = {
    // Frame 1: Standing neutral
    {Vector2D(0, -174), Vector2D(0, -142), 
     Vector2D(-40, -110), Vector2D(-70, -60), 
     Vector2D(40, -110), Vector2D(70, -60),
     Vector2D(-30, 25), Vector2D(-40, 100), 
     Vector2D(30, 25), Vector2D(40, 100), 
     0, 0, 1.0f},
    
    // Frame 2: Lean left
    {Vector2D(0, -174), Vector2D(-5, -142), 
     Vector2D(-60, -100), Vector2D(-100, -50), 
     Vector2D(30, -120), Vector2D(50, -140),
     Vector2D(-35, 25), Vector2D(-50, 100), 
     Vector2D(45, 20), Vector2D(70, 95), 
     5, -15, 1.0f},
    
    // Frame 3: Lean right
    {Vector2D(0, -174), Vector2D(5, -142), 
     Vector2D(-30, -120), Vector2D(-50, -140),
     Vector2D(60, -100), Vector2D(100, -50),
     Vector2D(-45, 20), Vector2D(-70, 95), 
     Vector2D(35, 25), Vector2D(50, 100), 
     5, 15, 1.0f},
};
```

---

## Performance Tuning

### Monitor Performance
The console shows debug output. Look for any dropped frames.

### Optimization Flags
In `build.bat` or g++ command, ensure `-O2` is included:
```batch
g++ -std=c++17 -O2 ...
```

For maximum speed, use `-O3`:
```batch
g++ -std=c++17 -O3 ...
```

### Reduce Visual Complexity
If performance is poor, simplify:

1. **Reduce DANCER_COUNT** (fewer dancers to draw):
```cpp
const int DANCER_COUNT = 3;  // Default: 6
```

2. **Lower WINDOW_HEIGHT/WIDTH**:
```cpp
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 576;
```

3. **Reduce FPS** (less responsive, but faster):
```cpp
const unsigned int FPS = 30;  // Default: 60
```

4. **Simplify backgrounds** by reducing number of particles/effects in `BackgroundRenderer`

### Profile the Code
Compile with debug info:
```bash
g++ -std=c++17 -g -o stick-dance-debug.exe main.cpp ...
```

Use a profiler to identify bottlenecks.

---

## Troubleshooting

### "Cannot find SFML headers"
**Error:** `fatal error: SFML/Graphics.hpp: No such file or directory`

**Solution:**
1. Verify SFML path: `C:\SFML-3.0\include\SFML` should exist
2. In `build.bat`, update:
```batch
set SFML_PATH=C:\your\sfml\path
```
3. Recompile

### "Undefined reference to SFML functions"
**Error:** `undefined reference to 'sf::RenderWindow::RenderWindow(...)'`

**Solution:**
1. Check library files exist: `C:\SFML-3.0\lib\` should have `.a` files
2. Ensure `-L` path points to correct location
3. Link libraries in correct order:
```bash
-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

### "DLL not found" at runtime
**Error:** `The code execution cannot proceed because sfml-graphics-3.dll was not found`

**Solution:**
1. Copy DLLs from `C:\SFML-3.0\bin\` to project folder
2. Or add to Windows PATH: `C:\SFML-3.0\bin`

### "No MP3 song found"
**Error:** Program starts but shows "No MP3 song found"

**Solution:**
1. Place audio file in same folder as `.exe`
2. Name must end with `.mp3` or `.ogg`
3. File must be readable (check permissions)

### "OpenAL error" or "No sound"
**Error:** Audio plays silently or crashes

**Solution:**
1. Ensure `openal32.dll` is in project folder
2. Update SFML - download latest 3.x build
3. Test with different audio format (OGG instead of MP3)

### Choppy Animation
**Cause:** Frame rate drops below 60 FPS

**Solutions:**
1. Reduce number of dancers: `DANCER_COUNT = 3`
2. Disable some background effects
3. Close other programs
4. Compile with `-O3` optimization
5. Reduce window size

### Controls Not Responding
**Cause:** Program frozen

**Solution:**
1. Check audio file is valid (try different MP3)
2. Ensure FPS isn't capped too low
3. Recompile with latest code

### Garbled Text in UI
**Cause:** Font not found

**Solution:**
1. Ensure `C:\Windows\Fonts\Arial.ttf` exists (standard on Windows)
2. Or modify font path in `UIRenderer` constructor:
```cpp
if (!large_font.openFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
    // Change to a font that exists on your system
}
```

---

## Summary

To customize your animation:

1. **Change song:** Place `.mp3` file in folder
2. **Change BPM:** Modify `BASE_BPM` value
3. **Add dances:** Modify `POSES` array
4. **Change visuals:** Adjust color constants or `HEAD_RADIUS`, `LINE_WIDTH`
5. **Recompile:** Run `build.bat`

For production use:
- Test on target Windows version
- Include all DLL files with executable
- Document any custom BPM/choreography changes
- Keep `main.cpp` commented for future modifications

Enjoy your customized stick-man dance animation!
