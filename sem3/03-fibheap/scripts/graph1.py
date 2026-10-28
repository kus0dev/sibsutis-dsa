import matplotlib.pyplot as plt
import numpy as np

data = np.genfromtxt("../out/table1.csv", delimiter=',', skip_header=1,
                     dtype=(int, int, float, float, float, float))
sizes = []
minheap_ex = []
fibheap_ex = []
for line in data:
    sizes.append(line[1]/1000)
    minheap_ex.append(line[2]*10**6)
    fibheap_ex.append(line[3]*10**6)

plt.figure(figsize=(10,6))
plt.plot(sizes, minheap_ex, "s-", label="minheap_extractmin", markersize=4)
plt.plot(sizes, fibheap_ex, "x-", label="fibheap_extractmin", markersize=4)
plt.xticks(sizes)
plt.xlabel("Количество элементов, тыс")
plt.ylabel("Время, мкс")
plt.grid(True, linestyle='--', linewidth=0.5)
plt.legend()
plt.savefig("../out/graph1.png", dpi=175)
plt.close()
