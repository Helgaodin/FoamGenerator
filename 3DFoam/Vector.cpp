// class Vector
#include <math.h>
#include <iostream>
#include <algorithm>
#include "Point.cpp"

class Vector
{
public:
	Point start;
	Point end;
	float x, y, z;
	int cluster;

	Vector()
	{
	}

	Vector(float x_in, float y_in, float z_in)
	{
		x = x_in;
		y = y_in;
		z = z_in;
	}

	Vector(Point start_in, Point end_in)
	{
		start = start_in;
		end = end_in;
		x = end_in.x - start_in.x;
		y = end_in.y - start_in.y;
		z = end_in.z - start_in.z;
	}

	Vector vector_multiple(Vector another)
	{
		float res_x = y * another.z - z * another.y;
		float res_y = z * another.x - x * another.z;
		float res_z = x * another.y - y * another.x;
		Vector vector = Vector(res_x, res_y, res_z);
		return vector;
	}

	float get_distance()
	{
		return sqrt(x * x + y * y + z * z);
	}

	void normalize_vector()
	{
		float n = get_distance();
		x = x / n;
		y = y / n;
		z = z / n;
	}

	Point get_point_with_length(float l)
	{
		normalize_vector();
		x = x * l;
		y = y * l;
		z = z * l;
		end = Point(start.x + x, start.y + y, start.z + z);
		return end;
	}

	float dot_product(Vector another)
	{
		float p = x * another.x + y * another.y + z * another.z;
		return p;
	}

	float get_cos(Vector another)
	{
		float scal = dot_product(another);
		float cos = scal / (get_distance() * another.get_distance());
		return cos;
	}
};