import mpl_toolkits
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

 
def show(fileBase, L):
    so = []
    should_start = True
    for line in open(fileBase):
        if (should_start):
            should_start = False
            continue
        line = line.split('\t')
        s = []
        for coord in line:
            s.append(float(coord))
        so.append(s)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlim([0, L])
    ax.set_ylim([0, L])
    ax.set_zlim([0, L])
    for elem in so:
        ax.plot([elem[0], elem[3]], [elem[1],elem[4]],zs=[elem[2],elem[5]])
    plt.show()
    
L = 10
fileBase = r'C:\Repos\FoamGenerator\Examples\Foam3D_L10_l5.txt'
show(fileBase, L)