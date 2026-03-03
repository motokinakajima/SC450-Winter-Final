import numpy as np
import matplotlib as mpl

mpl.use("pgf")
mpl.rcParams.update({
    "pgf.texsystem": "pdflatex",
    "font.family": "serif",
    "text.usetex": True,
    "pgf.rcfonts": False,
    "axes.unicode_minus": False
})

import matplotlib.pyplot as plt

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'

theta = np.linspace(0, 2 * np.pi, 1000)

x = np.cos(theta)
y = np.sin(theta)

z1 = (np.sin(theta))**16 + 0.3 * (np.cos(theta))**8 - 4.5 * (np.cos(theta/2))**64

z2 = np.zeros_like(theta)

fig = plt.figure(figsize=(7, 6))
ax = fig.add_subplot(111, projection='3d')

ax.plot(x, y, z1, label=r'$\frac{k}{d} = \sin^{16}(\theta_{ij}) + 0.3\cos^8(\theta_{ij}) - 4.5\cos^{64}\left(\frac{\theta_{ij}}{2}\right)$', color='blue', linewidth=1.5)

ax.plot(x, y, z2, label=r'$z=0$', color='gray', linestyle='--')

ax.scatter([0],[0],[0], color='black', s=40, label='Table Center', zorder=5)

ax.scatter([1], [0], [0], color='red', s=40, label=r'Student $i$', zorder=5)

ax.scatter([np.sqrt(2)/2], [np.sqrt(2)/2], [0], color='green', s=40, label=r'Student $j$', zorder=5)

o_i = np.array([1, 0, 0])
o_j = np.array([np.sqrt(2)/2, np.sqrt(2)/2, 0])
ax.quiver(0, 0, 0, o_i[0], o_i[1], o_i[2], color='red', length=1.0, normalize=True, arrow_length_ratio=0.0)
ax.quiver(0, 0, 0, o_j[0], o_j[1], o_j[2], color='green', length=1.0, normalize=True, arrow_length_ratio=0.0)

arc_theta = np.linspace(0, np.pi/4, 100)
arc_r = 0.2
arc_x = arc_r * np.cos(arc_theta)
arc_y = arc_r * np.sin(arc_theta)
arc_z = np.zeros_like(arc_theta)
ax.plot(arc_x, arc_y, arc_z, color='orange', label=r'$\theta_{ij}$', linewidth=2)

ax.set_xlabel(r'$x$')
ax.set_ylabel(r'$y$')
ax.set_zlabel(r'$z$')

ax.view_init(elev=20, azim=45)

ax.legend(loc='upper right', bbox_to_anchor=(1.02, 1.05), borderaxespad=0.0)
plt.tight_layout()

plt.savefig('cross_talk_coefficient.pdf', bbox_inches='tight', transparent=True)
plt.savefig('cross_talk_coefficient.pgf', bbox_inches='tight')

print("グラフの保存が完了しました。")