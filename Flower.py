import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.colors as mcolors

# Rose curve (polar rose - k petals, k odd = k petals, k even = 2k petals)
n_points = 600
k = 7 # petal count (try 4, 5, 7)
t = np.linspace(0, 2 * np.pi, n_points)
r = 15 * np.cos(k * t)

x = r * np.cos(t)
y = r * np.sin(t)

cx, cy = 0, 0

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_xlim(-18, 18)
ax.set_ylim(-18, 18)
ax.set_facecolor('black')
fig.patch.set_facecolor('black')
ax.axis('off')

# Custom vivid colormap: hot pink -> magenta -> purple -> gold -> crimson
vivid_colors = ["#fe07d1", '#ff00ff', "#c9ff55", "#f7cf08", '#ff4500', '#ff1493']
custom_cmap = mcolors.LinearSegmentedColormap.from_list('vivid_rose', vivid_colors, N=n_points)
colors = custom_cmap(np.linspace(0, 1, n_points))

def animate(i):
    if i < n_points:
        ax.plot([cx, x[i]], [cy, y[i]],
                color=colors[i], linewidth=1.6, alpha=0.95)
    return []

ani = animation.FuncAnimation(fig, animate, frames=n_points,
                               interval=8, blit=False, repeat=False)

plt.show()
# To save as GIF instead of live display:
# ani.save('vivid_rose.gif', writer='pillow', fps=60)