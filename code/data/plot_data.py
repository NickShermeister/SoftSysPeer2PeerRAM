import matplotlib.pyplot as plt
y = []
y2 = []
disk = []
f = open('./results.txt', 'r')
f2 = open('./results_local.txt', 'r')
for line in f:
    y.append(float(line))
    disk.append(4)
for line in f2:
    y2.append(float(line))
f.close()
x = [i for i in range(10,1410, 50)]
plt.semilogy(x, disk)
plt.semilogy(x, y)
plt.semilogy(x, y2)
plt.ylabel('Average Access Time (ms)')
plt.xlabel('Length of string')
plt.legend(["(Estimated) Hard Drive Access","Peer2Peer","RAM Access"])
plt.title("Comparison of different storage methods")
plt.show()
