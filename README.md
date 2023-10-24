# FoamGenerator
Foam generation for 2D and 3D.

###Usage

In Foam.h you can set L - size of the box and l - size of the fiber.

g++ Foam.cpp -o3 -o a.out
a.out {file name}
If file name is not provided file will be saved as Foam{2/3}D_L{L size}_l{l size}.txt

File consists of: Number of fibers, and rows with coordinates of start and end points of fibers.
You can draw it with DrawFoam.py script, where you need to provide fileName and size of L.
