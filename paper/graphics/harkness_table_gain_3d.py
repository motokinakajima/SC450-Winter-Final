import numpy as np
import matplotlib as mpl

# --- PGFとLaTeXフォントの設定（おまじない） ---
# 注意: 実行環境にLaTeXがインストールされている必要があります
mpl.use("pgf")
mpl.rcParams.update({
    "pgf.texsystem": "pdflatex",
    "font.family": "serif",  # 論文用のセリフ体
    "text.usetex": True,     # テキストにLaTeXを使用
    "pgf.rcfonts": False,    # Matplotlibのフォント設定を無視
    "axes.unicode_minus": False
})

import matplotlib.pyplot as plt

# --- データの作成 ---
# thetaを0から2piまで細かく分割
theta = np.linspace(0, 2 * np.pi, 1000)

# r=1 なので、x, y は以下のようになります
x = np.cos(theta)
y = np.sin(theta)

# 1. 指定された複雑な曲線
z1 = (np.sin(theta))**16 + 0.3 * (np.cos(theta))**8 - 4.5 * (np.cos(theta/2))**64

# 2. z=0 の線 (r=1の円)
z2 = np.zeros_like(theta)

# --- グラフの描画 ---
fig = plt.figure(figsize=(7, 6))
ax = fig.add_subplot(111, projection='3d')

# 線1: メインの曲線
ax.plot(x, y, z1, label=r'Curve', color='blue', linewidth=1.5)

# 線2: z=0 の線
ax.plot(x, y, z2, label=r'$z=0$', color='gray', linestyle='--')

# 点: (1, 0, 0)
ax.scatter([1], [0], [0], color='red', s=40, label=r'Point $(1,0,0)$', zorder=5)

# 軸ラベルの設定（LaTeXの数式フォントになります）
ax.set_xlabel(r'$x$')
ax.set_ylabel(r'$y$')
ax.set_zlabel(r'$z$')

# グラフの視点を調整（見やすい角度に変更可能）
ax.view_init(elev=20, azim=45)

ax.legend()
plt.tight_layout()

# --- 保存 ---
# 論文には、このPDFを読み込むのが一番軽くて綺麗です
plt.savefig('my_3d_plot.pdf', bbox_inches='tight', transparent=True)
# 直接LaTeXコードとして埋め込みたい場合はこちら（コンパイルが少し遅くなります）
plt.savefig('my_3d_plot.pgf', bbox_inches='tight')

print("グラフの保存が完了しました。")