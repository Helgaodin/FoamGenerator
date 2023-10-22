#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Needle.cpp"

class Cluster
{
public:
	int numberCluster;
	bool intersect_0;
	bool intersect_1;
	bool intersect_2;
	bool intersect_3;
	std::vector<int> Needles;

	Cluster(int num)
	{
		numberCluster = num;
		intersect_0 = false;
		intersect_1 = false;
		intersect_2 = false;
		intersect_3 = false;
	}

	void setIntersect(Needle needle)
	{
		intersect_0 = needle.intersect_0;
		intersect_1 = needle.intersect_1;
		intersect_2 = needle.intersect_2;
		intersect_3 = needle.intersect_3;
	}

	Cluster union_cluster(Cluster otherCluster)
	{
		int num;
		if (otherCluster.numberCluster < numberCluster)
		{
			num = otherCluster.numberCluster;
		}
		else
		{
			num = numberCluster;
		}

		Cluster newCluster = Cluster(num);

		if (otherCluster.intersect_0 || intersect_0)
		{
			newCluster.intersect_0 = true;
		}

		if (otherCluster.intersect_1 || intersect_1)
		{
			newCluster.intersect_1 = true;
		}

		if (otherCluster.intersect_2 || intersect_2)
		{
			newCluster.intersect_2 = true;
		}

		if (otherCluster.intersect_3 || intersect_3)
		{
			newCluster.intersect_3 = true;
		}
		for (int i = 0; i < Needles.size(); i++)
		{
			newCluster.Needles.push_back(Needles[i]);
		}

		if (otherCluster.numberCluster != numberCluster)
		{
			for (int i = 0; i < otherCluster.Needles.size(); i++)
			{
				newCluster.Needles.push_back(otherCluster.Needles[i]);
			}
		}

		return newCluster;
	}

	bool isItEnd()
	{
		if (intersect_0 && intersect_1 && intersect_2 && intersect_3)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};