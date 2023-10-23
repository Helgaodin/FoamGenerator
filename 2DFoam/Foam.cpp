#include <stdio.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "_mt19937.c"
#include "Cluster.cpp"
#include <vector>
#include <string>
using namespace std;

void spill_needles(std::string address);
bool isIntersect(Point res);
std::vector<Cluster> GenerateSpecialClusters(std::vector<Needle>& Needles);
std::vector<Needle> Washout(std::vector<Needle> Needles);
mt19937_state state;
ofstream ResultFile;

int main(int argc, char *argv[])
{
	std::string address;
	if (argc != 2)
	{
		address = "Foam2D_L_l.txt";
		address.insert(strlen("Foam2D_L_l"), std::to_string(l));
		address.insert(strlen("Foam2D_L"), std::to_string(L));
	}
	else
	{
		address = argv[1];
	}
	mt19937_init_sequence_(&state, time(NULL));
	spill_needles(address);
	return 0;
}

void spill_needles(std::string address)
{
	std::vector<Needle> Needles;
	std::vector<Cluster> ClusterNum = GenerateSpecialClusters(Needles);
	std::vector<int> FreeClusterNum;

	/* previous 4 needles are already added */
	int index = 4;
	bool flag = true;
	int ClusterTheWinner;
	do
	{
		double x, y, phi;
		x = mt19937_generate_uniform_float_(&state) * L;
		y = mt19937_generate_uniform_float_(&state) * L;
		phi = mt19937_generate_uniform_float_(&state) * 2 * M_PI;
		Needle needle = Needle(x, y, phi, l);

		std::vector<int> numberNeedlesIntersect;
		for (int i = 4; i < index; i++)
		{
			Point res = needle.find_intersect(Needles[i]);
			if (isIntersect(res))
			{
				numberNeedlesIntersect.push_back(i);
			}
		}

		if (numberNeedlesIntersect.size() > 0)
		{
			/* create temp-cluster */
			Cluster clust = Cluster(ClusterNum.size());
			clust.Needles.push_back(index);
			clust.setIntersect(needle);
			needle.cluster = clust.numberCluster;

			/* check intersection with other clusters */
			for (int i = 0; i < numberNeedlesIntersect.size(); i++)
			{
				int nedCluster = Needles[numberNeedlesIntersect[i]].cluster;
				if (nedCluster < clust.numberCluster)
				{
					if (clust.numberCluster != ClusterNum.size())
					{
						FreeClusterNum.push_back(clust.numberCluster);
					}
				}
				else if (nedCluster != clust.numberCluster)
				{
					FreeClusterNum.push_back(nedCluster);
				}
				clust = clust.union_cluster(ClusterNum[nedCluster]);
				needle.cluster = clust.numberCluster;
				for (int j = 0; j < clust.Needles.size(); j++)
				{
					int numNed = clust.Needles[j];
					Needles[numNed].cluster = clust.numberCluster;
				}
			}

			if (clust.isItEnd())
			{
				ClusterTheWinner = clust.numberCluster;
				flag = false;
			}
			ClusterNum[clust.numberCluster] = clust;
		}
		else
		{
			/* receiving cluster */
			if (FreeClusterNum.size() > 0)
			{
				/* reuse old cluster cell */
				int clustnum = FreeClusterNum[FreeClusterNum.size() - 1];
				FreeClusterNum.pop_back();
				Cluster clust = Cluster(clustnum);
				clust.Needles.push_back(index);
				clust.setIntersect(needle);
				ClusterNum[clustnum] = clust;
				needle.cluster = clustnum;
			}
			else
			{
				/* generate completely new cluster */
				Cluster clust = Cluster(ClusterNum.size());
				clust.Needles.push_back(index);
				clust.setIntersect(needle);
				ClusterNum.push_back(clust);
				needle.cluster = ClusterNum.size() - 1;
			}
		}
		Needles.push_back(needle);
		index++;
	} while (flag);

	std::vector<Needle> SurvivedNeedles;
	for (int i = 0; i < ClusterNum[ClusterTheWinner].Needles.size(); i++)
	{
		int needle = ClusterNum[ClusterTheWinner].Needles[i];
		SurvivedNeedles.push_back(Needles[needle]);
	}

	int previous = SurvivedNeedles.size();
	SurvivedNeedles = Washout(SurvivedNeedles);
	while (SurvivedNeedles.size() != previous)
	{
		previous = SurvivedNeedles.size();
		SurvivedNeedles = Washout(SurvivedNeedles);
	}

	ResultFile.open(address, std::ios::app);
	ResultFile << SurvivedNeedles.size() << "\n";
	for (int i = 0; i < SurvivedNeedles.size(); i++)
	{
		ResultFile << SurvivedNeedles[i].x_start << "\t";
		ResultFile << SurvivedNeedles[i].y_start << "\t";
		ResultFile << SurvivedNeedles[i].x_end << "\t";
		ResultFile << SurvivedNeedles[i].y_end << "\n";
	}

	ResultFile.close();
}

bool isIntersect(Point res)
{
	if ((res.x != -42) && (res.y != -42))
	{
		return true;
	}
	
	return false;
}

std::vector<Cluster> GenerateSpecialClusters(std::vector<Needle>& Needles)
{
	std::vector<Cluster> clusterNum;

	Needle needle = Needle(0.0, 0.0, 0.0, L, 0.0);
	needle.cluster = 0;
	Needles.push_back(needle);
	Cluster cluster = Cluster(0);
	cluster.intersect_0 = true;
	cluster.Needles.push_back(0);
	clusterNum.push_back(cluster);

	needle = Needle(L, 0.0, M_PI / 2.0, L, L);
	needle.cluster = 1;
	Needles.push_back(needle);
	cluster = Cluster(1);
	cluster.intersect_1 = true;
	cluster.Needles.push_back(1);
	clusterNum.push_back(cluster);

	needle = Needle(L, L, -M_PI, 0.0, L);
	needle.cluster = 2;
	Needles.push_back(needle);
	cluster = Cluster(2);
	cluster.intersect_2 = true;
	cluster.Needles.push_back(2);
	clusterNum.push_back(cluster);

	needle = Needle(0.0, L, -M_PI / 2.0, 0.0, 0.0);
	needle.cluster = 3;
	Needles.push_back(needle);
	cluster = Cluster(3);
	cluster.intersect_3 = true;
	cluster.Needles.push_back(3);
	clusterNum.push_back(cluster);

	return clusterNum;
}

std::vector<Needle> Washout(std::vector<Needle> Needles)
{
	std::vector<int> NumbersToDelete;
	for (int i = 0; i < Needles.size(); i++)
	{
		int numberOfIntersect = 0;
		for (int j = 0; j < Needles.size(); j++)
		{
			Point res = Needles[i].find_intersect(Needles[j]);
			if (isIntersect(res))
			{
				numberOfIntersect++;
			}
		}
		if ((numberOfIntersect == 1) && (Needles[i].lenInBox == Needles[i].length))
		{
			NumbersToDelete.push_back(i);
		}
	}
	if (NumbersToDelete.size() == 0)
	{
		return Needles;
	}

	std::vector<Needle> SurvivedNeedles;
	int j=0;
	for (int i = 0; i < Needles.size(); i++)
	{
		int index = NumbersToDelete[j];
		if (i != index)
		{
			SurvivedNeedles.push_back(Needles[i]);
		}
		else
		{
			j++;
		}
	}

	return SurvivedNeedles;
}