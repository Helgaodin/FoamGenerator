// class Plane
#include <math.h>
#include <iostream>
#include <algorithm>
#include "Vector.cpp"

class Plane
{
public:
	float A, B, C, D;
	Vector n;
	Point P0, P1, P2;

	Plane()
	{
	}

	Plane(Point A0, Point A1, Point A2)
	{
		Vector v1 = Vector(A0, A1);
		Vector v2 = Vector(A0, A2);
		n = v1.vector_multiple(v2);
		A = n.x;
		B = n.y;
		C = n.z;
		D = -n.x * A0.x - n.y * A0.y - n.z * A0.z;
		P0 = A0;
		P1 = A1;
		P2 = A2;
	}

	bool isNeedleIntersectPlane(Vector vector)
	{
		bool result = setPointInPlane(vector.start) * setPointInPlane(vector.end) < 0;
		return result;
	}

	float setPointInPlane(Point point)
	{
		return A * point.x + B * point.y + C * point.z + D;
	}

	Point getIntersection(Vector vector)
	{
		// create two vector
		Vector v1 = Vector(P0, P1);
		Vector v2 = Vector(P0, P2);
		Vector N = v1.vector_multiple(v2);
		N.normalize_vector();
		Vector V = Vector(vector.start, P0);

		float d = N.dot_product(V);
		Vector W = Vector(vector.start, vector.end);

		float e = N.dot_product(W);
		if (e != 0)
		{
			float de = d / e;
			Point result = Point(vector.start.x + W.x * de, vector.start.y + W.y * de, vector.start.z + W.z * de);
			return result;
		}

		return Point(-1, -1, -1);
	}
};