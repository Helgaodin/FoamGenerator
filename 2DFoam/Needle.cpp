#include <math.h>
#include <iostream>
#include <algorithm>
#include "Foam.h"
#include <vector>
struct Point
{
	double x;
	double y;
};

class Needle
{
public:
	double x_start;
	double y_start;
	double phi;
	double x_end;
	double y_end;
	double length;
	double lenInBox;
	int cluster;
	bool intersect_0;
	bool intersect_1;
	bool intersect_2;
	bool intersect_3;

	Needle() {}

	Needle(double x, double y, double phi_f, double l)
	{
		intersect_0 = false;
		intersect_1 = false;
		intersect_2 = false;
		intersect_3 = false;
		x_end = l * cos(phi_f) + x;
		y_end = l * sin(phi_f) + y;
		phi = phi_f;
		x_start = x;
		y_start = y;
		length = l;
		lenInBox = findLenInBox();
		length = lenInBox;
		if (length < l)
		{
			// it was an intersection
			if (x_end == 0.0)
			{
				intersect_3 = true;
			}
			if (y_end == 0.0)
			{
				intersect_0 = true;
			}
			if (x_end == L)
			{
				intersect_1 = true;
			}
			if (y_end == L)
			{
				intersect_2 = true;
			}
		}
	}

	Needle(double x, double y, double phi_f, double x_e, double y_e)
	{
		x_end = x_e;
		y_end = y_e;
		phi = phi_f;
		x_start = x;
		y_start = y;
		length = L;
	}

	Needle(double x, double y, double phi_f, double x_e, double y_e, double l)
	{
		x_end = x_e;
		y_end = y_e;
		phi = phi_f;
		x_start = x;
		y_start = y;
		length = l;
	}
	double findLenInBox()
	{
		if ((x_end < L) && (x_end > 0) && (y_end < L) && (y_end > 0))
		{
			return length;
		}
		Point res = find_intersect_in_box();
		x_end = res.x;
		y_end = res.y;
		return length_vector(res.x, res.y);
	}
	void show_needle()
	{
		std::cout << "x = " << x_start << " y = " << y_start << " phi = " << phi << " length = " << length << " x_end = " << x_end << " y_end = " << y_end << "\n";
	}

	double length_vector(double x, double y)
	{
		double len = sqrt((x - x_start) * (x - x_start) + (y - y_start) * (y - y_start));
		return len;
	}

	Point find_intersect(Needle otherNeedle)
	{
		Point p1 = {x_start, y_start};
		Point p2 = {x_end, y_end};
		Point p3 = {otherNeedle.x_start, otherNeedle.y_start};
		Point p4 = {otherNeedle.x_end, otherNeedle.y_end};

		if ((is_intersect_rectangle(p1, p2, p3, p4)) && (is_intersect_by_multi_vector(p1, p2, p3, p4)))
		{
			/* if we have intersection, lets find it */
			Point res = find_point(p1, p2, p3, p4, otherNeedle.phi);
			return res;
		}
		else
		{
			Point res = {-42, -42};
			return res;
		}
	}

private:
	bool is_intersect_rectangle(Point p1, Point p2, Point p3, Point p4)
	{
		double x1 = std::min(p1.x, p2.x);
		double x2 = std::max(p1.x, p2.x);
		double x3 = std::min(p3.x, p4.x);
		double x4 = std::max(p3.x, p4.x);
		double y1 = std::min(p1.y, p2.y);
		double y2 = std::max(p1.y, p2.y);
		double y3 = std::min(p3.y, p4.y);
		double y4 = std::max(p3.y, p4.y);

		return (x2 >= x3) && (x4 >= x1) && (y2 >= y3) && (y4 >= y1);
	}

	bool is_intersect_by_multi_vector(Point p1, Point p2, Point p3, Point p4)
	{
		Point p3Mp1 = {p3.x - p1.x, p3.y - p1.y};
		Point p2Mp1 = {p2.x - p1.x, p2.y - p1.y};
		Point p4Mp1 = {p4.x - p1.x, p4.y - p1.y};
		if (vector_multi(p3Mp1, p2Mp1) * vector_multi(p4Mp1, p2Mp1) <= 0)
		{
			Point p1Mp3 = {-p3Mp1.x, -p3Mp1.y};
			Point p4Mp3 = {p4.x - p3.x, p4.y - p3.y};
			Point p2Mp3 = {p2.x - p3.x, p2.y - p3.y};
			if (vector_multi(p1Mp3, p4Mp3) * vector_multi(p2Mp3, p4Mp3) <= 0)
			{
				return true;
			}
		}
		
		return false;
	}

	int vector_multi(Point a, Point b)
	{
		return a.y * b.x - a.x * b.y > 0 ? 1 : -1;
	}

	Point find_point(Point p1, Point p2, Point p3, Point p4, double other_phi)
	{
		Point p1p2 = {p2.x - p1.x, p2.y - p1.y};
		Point p3p4 = {p4.x - p3.x, p4.y - p3.y};
		double tan12 = tan(phi);
		double tan34 = tan(other_phi);
		double b1 = p1.y - tan12 * p1.x;
		double b2 = p3.y - tan34 * p3.x;
		double Xa = (b2 - b1) / (tan12 - tan34);
		double Ya = tan34 * Xa + b2;
		Point result = {Xa, Ya};
		return result;
	}

	Point find_intersect_in_box()
	{
		double a = (y_start - y_end) / (x_start - x_end);
		double b = y_start - a * x_start;
		if ((-b / a < L) && (-b / a > 0))
		{
			Point res = {-b / a, 0.0};
			if (insideSegment(res))
			{
				return res;
			}
		}
		if (((L - b) / a < L) && ((L - b) / a > 0))
		{
			Point res = {(L - b) / a, L};
			if (insideSegment(res))
			{
				return res;
			}
		}
		if ((b < L) && (b > 0))
		{
			Point res = {0.0, b};
			if (insideSegment(res))
			{
				return res;
			}
		}
		if ((a * L + b < L) && (a * L + b > 0))
		{
			Point res = {L, a * L + b};
			if (insideSegment(res))
			{
				return res;
			}
		}

		return {-42.0, -42.0};
	}

	bool insideSegment(Point point)
	{
		double minX = std::min(x_start, x_end);
		double maxX = std::max(x_start, x_end);
		double minY = std::min(y_start, y_end);
		double maxY = std::max(y_start, y_end);
		if ((point.x < maxX) && (point.x > minX) && (point.y < maxY) && (point.y > minY))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};