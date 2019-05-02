import matplotlib.pyplot as plt
y = []
f = open('./results.txt', 'r')
for line in f:
    y.append(float(line))
print(y)
f.close()
x = [i for i in range(10,1000, 10)]
plt.plot(x, y)
plt.ylabel('Average Access Time (ms)')
plt.xlabel('Length of string')
plt.show()
