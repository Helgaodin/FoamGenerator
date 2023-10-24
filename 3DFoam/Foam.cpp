#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "Foam.h"
#include "Plane.cpp"
#include "Cluster.cpp"
#include "_mt19937.c"
#include <ctime>
#include <vector>
#include <string>
using namespace std;

void foam_generation(string fileName);
Point generate_point();
Cluster get_intersection_bool(int intersection, Cluster cluster);
int get_num_intersect_plane(Vector &V);
int safe_get_intersect_plane(Vector &V);
PointAndCluster closiestIntersection(std::vector<PointAndCluster> PointsWhichSet, Point A0, Point A1);

mt19937_state state;
ofstream ResultFile;

int main(int argc, char *argv[])
{
	std::string fileName;
	if (argc != 2)
	{
		fileName = "Foam3D_L" + std::to_string(L) + "_l" + std::to_string(l) + ".txt";
	}
	else
	{
		fileName = argv[1];
	}
	mt19937_init_sequence_(&state, time(NULL));
	foam_generation(fileName);
	return 0;
}

void foam_generation(string fileName)
{
	Point A0, A1, A2;
	Vector A0A1;
	Plane plane;
	std::vector<Vector> Needles;
	std::vector<Cluster> ClusterNum;
	std::vector<int> FreeClusterNum;
	bool flag = true;
	int ClusterTheWinner;
	int numOfNeedle = 0;
	do
	{
		std::vector<PointAndCluster> PointsWhichSet;
		//-----generate-plane------
		A0 = generate_point();
		A1 = generate_point();
		A2 = generate_point();
		plane = Plane(A0, A1, A2);
		//-------------------------
		//-----find-intersection---
		for (Vector needle : Needles)
		{
			if (plane.isNeedleIntersectPlane(needle))
			{
				Point intersectPoint = plane.getIntersection(needle);
				if (intersectPoint.IsBad())
				{
					std::cout << "Error intersection is bad "
							  << "\n";
					return;
				}

				Vector distV = Vector(A0, intersectPoint);
				if (distV.get_distance() < l)
				{
					PointAndCluster PaC = {intersectPoint, needle.cluster};
					PointsWhichSet.push_back(PaC);
				}
			}
		}
		//-----------------------
		//-----rotate-needle-----
		int maxNumCluster = ClusterNum.size(); // get new number
		PointAndCluster resultPoint = {A1, maxNumCluster};
		if (PointsWhichSet.size() != 0)
		{
			resultPoint = closiestIntersection(PointsWhichSet, A0, A1);
			if (resultPoint.point.IsBad())
			{
				std::cout << "Error intersection is bad "
						  << "\n";
				return;
			}
		}
		//------------------------
		//-------get-right-length-
		Vector resultVector = Vector(A0, resultPoint.point);
		Point endPoint = resultVector.get_point_with_length(l);
		Vector finalVector = Vector(A0, endPoint);
		finalVector.cluster = resultPoint.ClusterNum;
		int numCluster = safe_get_intersect_plane(finalVector);
		if (numCluster != -1)
		{
			// intersect with cube was found
			// add in cluster and all others needles
			if (finalVector.cluster == maxNumCluster)
			{
				// cluster was not found for new needle
				Cluster clust = Cluster(maxNumCluster);
				clust.Needles.push_back(numOfNeedle);
				clust = get_intersection_bool(numCluster, clust);
				ClusterNum.push_back(clust);
			}
			else
			{
				// cluster was found
				ClusterNum[finalVector.cluster].Needles.push_back(numOfNeedle);
				ClusterNum[finalVector.cluster] = get_intersection_bool(numCluster, ClusterNum[finalVector.cluster]);
				if (ClusterNum[finalVector.cluster].isEnd())
				{
					ClusterTheWinner = finalVector.cluster;
					flag = false;
				}
			}
		}
		else
		{
			if (finalVector.cluster == maxNumCluster)
			{
				// cluster was not found for new needle
				Cluster clust = Cluster(maxNumCluster);
				clust.Needles.push_back(numOfNeedle);
				ClusterNum.push_back(clust);
			}
			else
			{
				// cluster was found
				ClusterNum[finalVector.cluster].Needles.push_back(numOfNeedle);
			}
		}

		numOfNeedle++;
		Needles.push_back(finalVector);
	} while (flag);
	std::vector<Vector> SurvivedNeedles;
	for (int i = 0; i < ClusterNum[ClusterTheWinner].Needles.size(); i++)
	{
		int ned = ClusterNum[ClusterTheWinner].Needles[i];
		SurvivedNeedles.push_back(Needles[ned]);
	}
	ResultFile.open(fileName, std::ios::app);
	ResultFile << SurvivedNeedles.size() << "\n";
	for (Vector needle : SurvivedNeedles)
	{
		ResultFile << needle.start.x << "\t";
		ResultFile << needle.start.y << "\t";
		ResultFile << needle.start.z << "\t";
		ResultFile << needle.end.x << "\t";
		ResultFile << needle.end.y << "\t";
		ResultFile << needle.end.z << "\n";
	}
	ResultFile.close();
}

Cluster get_intersection_bool(int intersection, Cluster cluster)
{
	switch (intersection)
	{
	case 0:
		cluster.intersect_0 = true;
		break;
	case 1:
		cluster.intersect_1 = true;
		break;
	case 2:
		cluster.intersect_2 = true;
		break;
	case 3:
		cluster.intersect_3 = true;
		break;
	case 4:
		cluster.intersect_4 = true;
		break;
	case 5:
		cluster.intersect_5 = true;
		break;
	default:
		std::cout << "Error! intersection is not 0..5!!!";
		break;
	}

	return cluster;
}

Point generate_point()
{
	float x = mt19937_generate_uniform_float_(&state) * L;
	float y = mt19937_generate_uniform_float_(&state) * L;
	float z = mt19937_generate_uniform_float_(&state) * L;
	return Point(x, y, z);
}

int safe_get_intersect_plane(Vector &V)
{
	int numCluster = get_num_intersect_plane(V);
	if (numCluster == -1)
	{
		return -1;
	}

	int anotherNumCluster = get_num_intersect_plane(V);
	if (anotherNumCluster == -1)
	{
		return numCluster;
	}

	int thirdNumCluster = get_num_intersect_plane(V);
	if (thirdNumCluster == -1)
	{
		return anotherNumCluster;
	}

	return thirdNumCluster;
}

int get_num_intersect_plane(Vector &V)
{
	Point basePoint = Point(0, 0, 0);
	Point xPoint = Point(L, 0, 0);
	Point yPoint = Point(0, L, 0);
	Point zPoint = Point(0, 0, L);
	Plane plane = Plane(basePoint, xPoint, yPoint);
	if (plane.isNeedleIntersectPlane(V))
	{
		Point end = plane.getIntersection(V);
		V.end = end;
		return 0;
	}
	plane = Plane(basePoint, xPoint, zPoint);
	if (plane.isNeedleIntersectPlane(V))
	{
		Point end = plane.getIntersection(V);
		V.end = end;
		return 2;
	}
	plane = Plane(basePoint, zPoint, yPoint);
	if (plane.isNeedleIntersectPlane(V))
	{
		Point end = plane.getIntersection(V);
		V.end = end;
		return 4;
	}
	basePoint = Point(L, L, L);
	xPoint = Point(L, 0, L);
	yPoint = Point(0, L, L);
	zPoint = Point(L, L, 0);
	plane = Plane(basePoint, xPoint, yPoint);
	if (plane.isNeedleIntersectPlane(V))
	{
		Point end = plane.getIntersection(V);
		V.end = end;
		return 1;
	}
	plane = Plane(basePoint, zPoint, yPoint);
	if (plane.isNeedleIntersectPlane(V))
	{
		Point end = plane.getIntersection(V);
		V.end = end;
		return 3;
	}
	plane = Plane(basePoint, xPoint, zPoint);
	if (plane.isNeedleIntersectPlane(V))
	{
		Point end = plane.getIntersection(V);
		V.end = end;
		return 5;
	}

	return -1;
}

PointAndCluster closiestIntersection(std::vector<PointAndCluster> PointsWhichSet, Point A0, Point A1)
{
	float minCos = 1;
	int clusterNum = -1;
	Point resultPoint = Point(-1, -1, -1);
	Vector A0A1 = Vector(A0, A1);
	for (PointAndCluster point : PointsWhichSet)
	{
		Vector v = Vector(A0, point.point);
		float cos = A0A1.get_cos(v);
		if (cos < minCos)
		{
			minCos = cos;
			resultPoint = point.point;
			clusterNum = point.ClusterNum;
		}
	}
	PointAndCluster result = {resultPoint, clusterNum};
	return result;
}