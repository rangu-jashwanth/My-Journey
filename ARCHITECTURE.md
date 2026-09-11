# Architecture & System Overview

## Program Flow Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                     stick-dance.exe                             │
│                    (SFML 3.x Application)                       │
└────────────────────────────┬────────────────────────────────────┘
                             │
         ┌───────────────────┼───────────────────┐
         │                   │                   │
    ┌────▼──────┐    ┌──────▼──────┐   ┌───────▼────┐
    │   AUDIO   │    │  RENDERING  │   │   INPUT    │
    │ PLAYBACK  │    │   ENGINE    │   │  HANDLING  │
    │           │    │             │   │            │
    │ - Music   │    │ - Graphics  │   │ - Keyboard │
    │ - Sync    │    │ - Effects   │   │ - Controls │
    │ - Timing  │    │ - UI        │   │ - Modes    │
    └────┬──────┘    └──────┬──────┘   └───────┬────┘
         │                  │                   │
         └──────────────────┼───────────────────┘
                            │
              ┌─────────────▼─────────────┐
              │  Main Application Loop    │
              │  (60 FPS @ 16ms/frame)    │
              └────────────┬──────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
   ┌────▼────┐        ┌────▼────┐      ┌────▼────┐
   │ Process │        │ Calculate│      │  Render │
   │ Events  │        │Animation │      │ Frame   │
   └────┬────┘        └────┬────┘      └────┬────┘
        │                  │                 │
        ▼                  ▼                 ▼
   ┌──────────┐      ┌──────────┐    ┌─────────────┐
   │ Keyboard │      │ Beat & │     │ Draw to GPU  │
   │ Input    │      │ Pose  │      │ Display     │
   │ Handler  │      │ Inter-│      │ Window      │
   │          │      │polate │      │             │
   └──────────┘      └──────────┘   └─────────────┘
```

---

## Class Architecture

```
StickDanceApp (Main Controller)
├── sf::RenderWindow
├── sf::Music (Audio playback)
├── StickRenderer (Draws dancers)
│   ├── draw_line() - Limbs
│   ├── draw_circle() - Joints & head
│   └── draw_stick_person() - Full character
├── BackgroundRenderer (Draws effects)
│   ├── draw_background() - Stage & grid
│   ├── Grid rendering
│   ├── Stage lighting
│   ├── Crowd silhouettes
│   ├── Dynamic light beams
│   └── Finale mode effects
└── UIRenderer (Draws UI)
    ├── draw_ui() - Text & HUD
    ├── Time display
    ├── BPM indicator
    ├── Move counter
    ├── Beat progress bar
    └── Control hints
```

---

## Data Flow: Song to Animation

```
song.mp3
   │
   ├─▶ sf::Music::play()
   │      │
   │      ├─▶ Audio system (OpenAL)
   │      │
   │      └─▶ Playback position (seconds)
   │
   ▼
song_time (current playback position)
   │
   ├─▶ beat = song_time * BPM / 60
   │
   ▼
beat (float - continuous beat number)
   │
   ├─▶ pose_index = int(beat / POSE_BEATS) % num_poses
   │
   ├─▶ progress = (beat % POSE_BEATS) / POSE_BEATS
   │
   ▼
Pose at current beat (with interpolation)
   │
   ├─▶ per-dancer variations (phase offsets)
   │
   ├─▶ dynamic movement (wiggles, lean, bob)
   │
   ▼
Draw stick figure at that pose
   │
   ├─▶ All 6 dancers rendered
   │
   ▼
Display to window @ 60 FPS
```

---

## Animation Interpolation Pipeline

```
POSES Array [0...11]
    │
    ├─▶ Frame 0
    │     ├─ Head: (0, -174)
    │     ├─ Neck: (0, -142)
    │     ├─ Left elbow: (-48, -100)
    │     ├─ ... (all joints)
    │     └─ Bob, lean, facing
    │
    ├─▶ Frame 1
    │     ├─ Head: (0, -178)
    │     ├─ Neck: (0, -145)
    │     ├─ Left elbow: (-61, -108)
    │     └─ ...
    │
    ▼
For current beat position:
    │
    ├─▶ Find enclosing keyframes (frame N and N+1)
    │
    ├─▶ Calculate progress: 0.0 - 1.0
    │
    ├─▶ Apply smoothstep easing
    │
    ├─▶ Linearly interpolate each joint:
    │     joint_current = joint_N + (joint_N+1 - joint_N) * progress
    │
    ▼
Interpolated Pose for current beat
```

---

## Rendering Pipeline (Per Frame)

```
┌─ Clear Screen ────────────────────────────┐
│                                            │
│  window.clear(BACKGROUND_COLOR)           │
└────────────────────────────────────────────┘
                    │
                    ▼
┌─ Draw Background ─────────────────────────┐
│                                            │
│  ├─ Grid background                       │
│  ├─ Stage horizon line                    │
│  ├─ Stage glow ellipse                    │
│  ├─ Crowd silhouettes                     │
│  ├─ Dynamic lighting beams                │
│  └─ Finale mode effects (if active)       │
│                                            │
└────────────────────────────────────────────┘
                    │
                    ▼
┌─ Draw All 6 Dancers ──────────────────────┐
│                                            │
│ For each dancer (0...5):                  │
│   ├─ Calculate beat with phase offset     │
│   ├─ Get interpolated pose                │
│   ├─ Apply finale mode mods (if active)   │
│   └─ Draw stick figure:                   │
│       ├─ Body lines (9px thick)           │
│       ├─ Joint circles (7px radius)       │
│       ├─ Head circle (29px radius)        │
│       ├─ Eyes                             │
│       └─ Feet                             │
│                                            │
└────────────────────────────────────────────┘
                    │
                    ▼
┌─ Draw UI Overlay ─────────────────────────┐
│                                            │
│  ├─ Title: "STICK SHIFT"                  │
│  ├─ Subtitle: "SIX-DANCER PERFORMANCE"    │
│  ├─ Mode info: BPM, Mode, Mirror, etc     │
│  ├─ Time display: MM:SS                   │
│  ├─ Beat progress bar                     │
│  ├─ Control hints                         │
│  ├─ Flash indicator (if active)           │
│  └─ Finale indicator (if active)          │
│                                            │
└────────────────────────────────────────────┘
                    │
                    ▼
┌─ Display to Window ───────────────────────┐
│                                            │
│  window.display()                         │
│  ✓ Frame sent to GPU                      │
│  ✓ Appears on screen                      │
│  ✓ Next frame begins                      │
│                                            │
└────────────────────────────────────────────┘
```

---

## Control Input Handling

```
Main Loop
    │
    ├─▶ pollEvent()
    │     │
    │     ├─▶ KeyPressed Event
    │     │     │
    │     │     ├─ SPACE ─▶ toggle_pause()
    │     │     │
    │     │     ├─ K ─▶ skip forward 10s
    │     │     │
    │     │     ├─ T ─▶ turbo_mode = !turbo_mode
    │     │     │           (switches BASE_BPM to TURBO_BPM)
    │     │     │
    │     │     ├─ M ─▶ mirror_mode = !mirror_mode
    │     │     │           (flips dancers horizontally)
    │     │     │
    │     │     ├─ F ─▶ flash = 1.0
    │     │     │           (shows aura effect)
    │     │     │
    │     │     ├─ R ─▶ restart()
    │     │     │           (rewind to start, replay)
    │     │     │
    │     │     └─ ESC ─▶ window.close()
    │     │
    │     └─▶ WindowClosed Event
    │           └─▶ window.close()
    │
    ▼
Update Application State
    │
    ├─▶ Calculate frame timing
    │
    ├─▶ Update song_time from music
    │
    ├─▶ Calculate beat
    │
    ├─▶ Decay effects (flash, flash duration)
    │
    └─▶ Check if song finished
```

---

## Configuration Customization Path

```
main.cpp (Configuration Section)
    │
    ├─▶ Timing Configuration
    │     ├─ BASE_BPM = 192.0f
    │     ├─ TURBO_BPM = 240.0f
    │     ├─ SONG_START_OFFSET = 35.0f
    │     └─ SONG_FILENAME = "song.mp3"
    │
    ├─▶ Display Configuration
    │     ├─ WINDOW_WIDTH = 1280
    │     ├─ WINDOW_HEIGHT = 720
    │     └─ FPS = 60
    │
    ├─▶ Stage Configuration
    │     ├─ DANCER_COUNT = 6
    │     └─ STAGE_Y = 438.0f
    │
    ├─▶ Visual Configuration
    │     ├─ HEAD_RADIUS = 29
    │     ├─ JOINT_RADIUS = 7
    │     └─ LINE_WIDTH = 9
    │
    └─▶ Color Palette
          ├─ BACKGROUND
          ├─ MINT
          ├─ PINK
          ├─ YELLOW
          ├─ BLUE
          └─ WHITE

            │
            ▼
        main() reads these
            │
            ▼
        Creates StickDanceApp with settings
            │
            ▼
        Builds animation based on config
            │
            ▼
        stick-dance.exe with custom behavior
```

---

## Choreography Customization Path

```
POSES Array (12 keyframes)
    │
    ├─▶ Pose 0: Head at (0,-174), bob=8, etc.
    ├─▶ Pose 1: Head at (0,-178), bob=11, etc.
    ├─▶ Pose 2: Head at (0,-180), bob=10, etc.
    ├─▶ ... (9 more poses)
    └─▶ Pose 11: Head at (0,-170), bob=11, etc.

    Edit pose:
    └─▶ Modify Vector2D values for joint positions
    └─▶ Modify float bob, lean, facing values

    Add new pose:
    └─▶ Insert into array (max recommended: 16)

    Rebuild:
    └─▶ build.bat
    └─▶ stick-dance.exe with new choreography
```

---

## Performance Characteristics

```
Per-Frame Costs (@ 60 FPS = 16.67 ms per frame)

┌──────────────────────────┐
│ Audio Update             │  ~0.1 ms
│ Event Processing         │  ~0.2 ms
│ Animation Calculation    │  ~1.0 ms
│  ├─ Beat calculation     │
│  ├─ 6 dancers × pose at beat
│  └─ Interpolation        │
│ Rendering Pipeline       │  ~10-15 ms (GPU dependent)
│  ├─ Clear screen         │
│  ├─ Draw background      │
│  ├─ Draw 6 dancers × joints
│  ├─ Draw UI text         │
│  └─ Flip/present         │
│ SFML/Window Management   │  ~2-3 ms
├──────────────────────────┤
│ Total per frame          │  ~13-19 ms ✓
│ @ 60 FPS target          │
└──────────────────────────┘

Typical CPU Usage: 5-15% (on modern processor)
Typical Memory: 50-100 MB
GPU Memory: ~10-30 MB (depends on resolution)
```

---

## File Dependencies

```
stick-dance.exe
    │
    ├─▶ Requires at runtime:
    │     ├─ sfml-graphics-3.dll (drawing)
    │     ├─ sfml-audio-3.dll (music playback)
    │     ├─ sfml-window-3.dll (window management)
    │     ├─ sfml-system-3.dll (timing, math)
    │     └─ openal32.dll (audio mixing)
    │
    ├─▶ Loads at startup:
    │     ├─ song.mp3 (or first .mp3/.ogg found)
    │     └─ C:\Windows\Fonts\Arial.ttf (UI text)
    │
    └─▶ No other files needed
```

---

## Build Dependency Chain

```
main.cpp
    │
    ├─ #include <SFML/Graphics.hpp>
    │     ├─ Requires: C:\SFML-3.0\include\
    │     └─ Links to: C:\SFML-3.0\lib\libsfml-graphics.a
    │
    ├─ #include <SFML/Audio.hpp>
    │     └─ Requires: libsfml-audio.a
    │
    ├─ #include <SFML/Window.hpp>
    │     └─ Requires: libsfml-window.a
    │
    └─ #include <SFML/System.hpp>
          └─ Requires: libsfml-system.a

            │
            ▼
        g++ compiler
            │
            ├─ -I"C:\SFML-3.0\include"
            ├─ -L"C:\SFML-3.0\lib"
            ├─ -lsfml-graphics
            ├─ -lsfml-audio
            ├─ -lsfml-window
            ├─ -lsfml-system
            └─ -std=c++17 -O2

            │
            ▼
        Linker
            │
            ├─ Combines object files
            ├─ Links SFML libraries
            ├─ Resolves dependencies
            └─ Creates stick-dance.exe
```

---

## Execution Timeline (Per Frame)

```
Frame N (16.67 ms budget @ 60 FPS)
├─ 0.0 ms   : Frame begins
│
├─ 0.5 ms   : Handle input events
│             ├─ Check keyboard
│             ├─ Update game state
│             └─ Pause/resume/seek if needed
│
├─ 1.0 ms   : Update timing
│             ├─ Get current song position
│             ├─ Calculate beat
│             └─ Update effects decay
│
├─ 2.0 ms   : Calculate poses
│             ├─ For each dancer:
│             │   ├─ Calculate beat offset
│             │   ├─ Interpolate pose
│             │   └─ Apply dynamics
│
├─ 3.0 ms   : Clear/prepare rendering
│             └─ Clear screen buffer
│
├─ 4.0 ms   : Render background
│             ├─ Draw grid
│             ├─ Draw stage
│             ├─ Draw lights
│             └─ ~2-5 ms (GPU)
│
├─ 10.0 ms  : Render dancers
│             ├─ For each dancer:
│             │   ├─ Draw limbs (lines)
│             │   ├─ Draw joints (circles)
│             │   └─ Draw head (circle)
│             └─ ~3-8 ms (GPU)
│
├─ 15.0 ms  : Render UI
│             ├─ Draw text
│             ├─ Draw progress bar
│             └─ ~1-2 ms
│
├─ 16.2 ms  : Flip/present
│             └─ Send frame to screen
│
└─ 16.67 ms : Frame complete ✓
              Next frame begins
```

---

## Memory Layout

```
Approximate Memory Usage
┌─────────────────────────────────────┐
│ Program Code                        │ ~2 MB
│  - main() and functions             │
│  - SFML linked libraries            │
├─────────────────────────────────────┤
│ Data Section                        │ ~48 MB
│  - POSES array (12 poses)           │ < 1 KB
│  - Texture/buffer allocations       │
│  - SFML objects                     │
├─────────────────────────────────────┤
│ Runtime Memory                      │ varies
│  - Window buffer (1280×720×4B)      │ ~3.5 MB
│  - Depth/stencil buffers            │ ~3.5 MB
│  - Music streaming                  │ ~5-10 MB
│  - SFML internal caches             │ ~10-20 MB
├─────────────────────────────────────┤
│ Total ~50-100 MB                    │
└─────────────────────────────────────┘
```

---

This architecture provides:
- ✅ High performance (compiled C++17)
- ✅ Clean separation of concerns
- ✅ Easy extensibility
- ✅ Efficient rendering pipeline
- ✅ Precise audio synchronization
- ✅ Responsive input handling
