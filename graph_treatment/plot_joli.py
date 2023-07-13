import numpy as np
import matplotlib.pyplot as plt

# données à la con
data = {
    "a": np.arange(50),  # x
    "c": np.random.randint(0, 50, 50),  # couleur
    "d": np.random.randn(50),  # taille
}
data["b"] = data["a"] + 10 * np.random.randn(50)  # y
data["d"] = np.abs(data["d"]) * 100  # taille correction


# figure avec 2 axes
fig, ax = plt.subplots(2, 1, figsize=(10, 10), layout="constrained")

# on fait un truc moche sur l'axe 0
x = np.linspace(0, 70, 20)  # Sample data.
ax[0].plot(
    x, x, label="linear", color="blue", linewidth=3, linestyle="--"
)  # Plot some data on the axes.


# on fait un truc beau sur l'axe 1
pc = ax[1].scatter(
    "a", "b", c="c", s="d", data=data, marker="o", label="data", cmap="RdBu_r"
)
fig.colorbar(pc, ax=ax[1], extend="both")


# on fait une autre courbe histoire d'avoir un autre symbole
ax[1].scatter(x, x ** (1 / 2), marker="v", label="data1")

# on fixe des trucs
ax[1].set_title("title")
ax[1].set_xlabel("entry a")
ax[1].set_ylabel("entry b")
ax[1].set_ylim(0, 70)
ax[1].set_xlim(0, 70)
ax[1].legend()

# on ajoute un texte
ax[1].annotate(
    "best tactic",
    xy=(49, 52),
    xytext=(58, 58),
    arrowprops=dict(facecolor="black", shrink=0.03),
)


# on fait joli
ax[1].grid("on")
xlab = ax[1].xaxis.get_label()
ylab = ax[1].yaxis.get_label()
xlab.set_style("italic")
xlab.set_size(10)
ylab.set_style("italic")
ylab.set_size(10)
ttl = ax[1].title
ttl.set_weight("bold")
ax[1].set_yticks([0, 17, 70])
ax[1].xaxis.set_tick_params(size=0)
ax[1].yaxis.set_tick_params(size=0)
ax[1].spines["right"].set_color((0.8, 0.8, 0.8))
ax[1].spines["top"].set_color((0.8, 0.8, 0.8))


plt.show()
