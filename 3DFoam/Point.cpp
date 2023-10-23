// class Point
#include <math.h>
#include <iostream>
#include <algorithm>

class Point
{
public:
	float x, y, z;

	Point()
	{
	}

	Point(float x_in, float y_in, float z_in)
	{
		x = x_in;
		y = y_in;
		z = z_in;
	}

	void show_point()
	{
		std::cout << "x = " << x << " y = " << y << " z = " << z << "\n";
	}

	bool IsBad()
	{
		return x == -1 && y == -1 && z == -1;
	}
};

struct PointAndCluster
{
	Point point;
	int ClusterNum;
};