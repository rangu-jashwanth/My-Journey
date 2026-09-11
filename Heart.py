import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.colors as mcolors

# ---------------------------------------------------------
# 1. Heart outline (classic parametric heart curve)
# ---------------------------------------------------------
n_points = 500
t = np.linspace(0, 2 * np.pi, n_points)
base_x = 16 * np.sin(t) ** 3
base_y = 13 * np.cos(t) - 5 * np.cos(2 * t) - 2 * np.cos(3 * t) - np.cos(4 * t)

cx, cy = 0, 0

# Vivid custom gradient: hot pink -> red -> orange -> gold -> magenta -> hot pink
vivid_colors = ['#ff1493', '#ff0044', '#ff4500', '#ffd700', '#ff00ff', '#ff1493']
cmap = mcolors.LinearSegmentedColormap.from_list('vivid_heart', vivid_colors, N=n_points)
ray_colors = cmap(np.linspace(0, 1, n_points))

# ---------------------------------------------------------
# 2. Figure setup
# ---------------------------------------------------------
fig, ax = plt.subplots(figsize=(7, 7))
ax.set_xlim(-22, 22)
ax.set_ylim(-22, 18)
ax.set_facecolor('black')
fig.patch.set_facecolor('black')
ax.axis('off')
ax.set_aspect('equal')

BUILD_FRAMES = n_points          # frames to draw the heart, ray by ray
PULSE_FRAMES = 240                # frames for the beating/glow phase
N_PARTICLES = 60

rng = np.random.default_rng(7)
particle_angle = rng.uniform(0, 2 * np.pi, N_PARTICLES)
particle_radius0 = rng.uniform(2, 16, N_PARTICLES)
particle_speed = rng.uniform(0.15, 0.5, N_PARTICLES)
particle_size = rng.uniform(8, 40, N_PARTICLES)
particle_color = cmap(rng.uniform(0, 1, N_PARTICLES))

ray_lines = []          # persistent line objects for the build phase
glow_lines = []         # extra glow layers drawn on top of the filled heart
particle_scatter = ax.scatter([], [], s=[], c=[], alpha=0)
title_text = ax.text(0, 16.5, 'LOVE', color='white', fontsize=18,
                      ha='center', family='sans-serif', alpha=0)


def heart_points(scale=1.0):
    """Return heart boundary scaled (used for the beating effect)."""
    return base_x * scale, base_y * scale


def animate(frame):
    artists = []

    # ---- Phase 1: build the heart with radiating colored rays ----
    if frame < BUILD_FRAMES:
        if frame == 0:
            for line in ray_lines:
                line.remove()
            ray_lines.clear()
        i = frame
        line, = ax.plot([cx, base_x[i]], [cy, base_y[i]],
                         color=ray_colors[i], linewidth=1.4, alpha=0.9,
                         solid_capstyle='round')
        ray_lines.append(line)
        artists.extend(ray_lines)

    # ---- Phase 2: heartbeat / pulse / glow / sparkle ----
    else:
        p = frame - BUILD_FRAMES
        # heartbeat waveform: two quick pulses then rest, like a real heartbeat
        beat = 1.0 + 0.06 * (np.sin(p * 0.35) ** 6) + 0.03 * np.sin(p * 0.7)
        gx, gy = heart_points(beat)

        # clear previous glow layers
        for gl in glow_lines:
            gl.remove()
        glow_lines.clear()

        # layered glow: several outlines with decreasing alpha/increasing width
        for width, alpha in [(10, 0.05), (6, 0.10), (3, 0.25), (1.4, 0.9)]:
            gl, = ax.plot(gx, gy, color='#ff1493', linewidth=width, alpha=alpha)
            glow_lines.append(gl)
        artists.extend(glow_lines)

        # fade in the title once the heart starts beating
        title_text.set_alpha(min(1.0, p / 30))
        artists.append(title_text)

        # floating sparkle particles drifting outward and fading
        life = (p * particle_speed) % 40
        radius = particle_radius0 + life
        px = radius * np.cos(particle_angle) * 0.9
        py = radius * np.sin(particle_angle) * 0.9 - 3
        alpha_p = np.clip(1 - life / 40, 0, 1)
        particle_scatter.set_offsets(np.column_stack([px, py]))
        particle_scatter.set_sizes(particle_size)
        particle_scatter.set_color(particle_color)
        particle_scatter.set_alpha(0.8)
        particle_scatter.set_edgecolor('none')
        artists.append(particle_scatter)

    return artists


total_frames = BUILD_FRAMES + PULSE_FRAMES
ani = animation.FuncAnimation(fig, animate, frames=total_frames,
                               interval=12, blit=False, repeat=True)

plt.show()

# To save as a GIF instead of showing it live, comment out plt.show() above and use:
# ani.save('advanced_heart.gif', writer='pillow', fps=60)