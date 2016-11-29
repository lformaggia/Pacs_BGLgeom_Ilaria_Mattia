#include "intersections2D.hpp"
#include "point.hpp"
#include "linear_edge_geometry_srctgt.hpp"
#include <iostream>

using namespace BGLgeom;

int main()
{
	point<2> p1(0.0,3.0);
	point<2> p2(0.0,0.0);
	linear_edge_geometry_srctgt<2> edge1(p1,p2);

	point<2> p3(0.0,4.0);
	point<2> p4(0.0,0.0);
	linear_edge_geometry_srctgt<2> edge2(p3,p4);

	Intersection I = compute_intersection(edge1,edge2);
	std::cout << I << std::endl;
	
	return 0;
}
