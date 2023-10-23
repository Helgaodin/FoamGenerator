#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>

class Cluster
{
public:
	int numberCluster;
	bool intersect_0;
	bool intersect_1;
	bool intersect_2;
	bool intersect_3;
	bool intersect_4;
	bool intersect_5;
	std::vector<int> Needles;

	Cluster(int num)
	{
		numberCluster = num;
		intersect_0 = false;
		intersect_1 = false;
		intersect_2 = false;
		intersect_3 = false;
		intersect_4 = false;
		intersect_5 = false;
	}

	bool isEnd()
	{
		if (intersect_0 && intersect_1 && intersect_2 && intersect_3 && intersect_4 && intersect_5)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};