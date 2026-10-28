import matplotlib.pyplot as plt
import numpy as np

data = np.genfromtxt("../out/table2.csv", delimiter=',', skip_header=1,
                     dtype=(int, int, float, float, float, float))
sizes = []
minheap_dec = []
fibheap_dec = []
for line in data:
    sizes.append(line[1]/1000)
    minheap_dec.append(line[4]*10**6)
    fibheap_dec.append(line[5]*10**6)

plt.figure(figsize=(10,6))
plt.plot(sizes, minheap_dec, "s-", label="minheap_decreasekey", markersize=4)
plt.plot(sizes, fibheap_dec, "x-", label="fibheap_decreasekey", markersize=4)
plt.xticks(sizes)
plt.xlabel("Количество элементов, тыс")
plt.ylabel("Время, мкс")
plt.grid(True, linestyle='--', linewidth=0.5)
plt.legend()
plt.savefig("../out/graph2-2.png", dpi=175)
plt.close()
