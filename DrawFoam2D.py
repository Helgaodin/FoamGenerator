import matplotlib.pyplot as plot
file = r'C:\Repos\FoamGenerator\Examples\Foam2D_L10_l2.txt'
x_start = []
y_start = []
x_end = []
y_end = []
p = 0
for line in open(file):
    if (p == 0):
        p=p+1
        continue
    line = line.split('\t')
    x_start.append(float(line[0]))
    y_start.append(float(line[1]))
    x_end.append(float(line[2]))
    y_end.append(float(line[3]))

p=0
plot.figure(figsize=(4,4))
plot.ylim(0, 10)
plot.xlim(0, 10)
plot.xlabel("L")
for x in x_start:
    plot.plot([x_start[p], x_end[p]], [y_start[p], y_end[p]], color='c', alpha = 0.4)
    p=p+1
#plt.savefig(r'You_file_path.pdf')
plot.show()
