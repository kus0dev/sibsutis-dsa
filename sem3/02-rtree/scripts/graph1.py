import matplotlib.pyplot as plt
import numpy as np

# handle data
data = np.genfromtxt("../out/table1.csv", delimiter=',', skip_header=1,
                     dtype=(int, int, float))
sizes = []
rtree_lookup = []
for line in data:
    sizes.append(line[1]/1000)
    rtree_lookup.append(line[2]*10**6)

plt.figure(figsize=(10, 6))

# curves
plt.plot(sizes, rtree_lookup, "s-", color="green", label="rtree_lookup", markersize=4,
         linewidth=1.5)

# Oy
y_ticks = [0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45]
plt.yticks(y_ticks, [str(y) for y in y_ticks])

# Ox
plt.xticks(sizes, [str(int(x)) for x in sizes])
plt.xlim(sizes[0], sizes[-1])

# label
plt.xlabel("Количество элементов в массиве, тыс")
plt.ylabel("Время выполнения, мкс")

# grid
plt.grid(True, which='both', linestyle='--', linewidth=0.5, color='grey', alpha=0.7)

# legend
plt.legend()

# save
plt.savefig("../out/graph1.png", dpi=175)

# display
plt.tight_layout()
plt.show()
