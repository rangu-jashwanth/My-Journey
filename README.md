# My-Journey

## Stick Shift dance animation

This project uses Python 3 and Pygame to animate a stick person in time with a song.
The default timing is set for **Yeshanagula (From The Paradise) (Telugu)** at 168 BPM.

1. Install Pygame: `py -m pip install pygame`
2. Put a music file named `song.mp3` in this folder, beside `stick dance.py`.
3. Run it from PowerShell: `py "stick dance.py"`

The `BASE_BPM` and `TURBO_BPM` values near the top of `stick dance.py` control the choreography clock.
The animation changes movement every beat and smoothly blends between a six-pose choreography.
Use Space to pause, `K` to skip forward 10 seconds, `T` to toggle turbo mode, `R` to restart the song, and Escape to quit.
