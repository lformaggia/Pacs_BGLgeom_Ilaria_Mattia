#include "intersections2D.hpp"
#include "intersections2D_utilities.hpp"
#include "point.hpp"
#include "linear_edge.hpp"
#include "data_structure.hpp"
#include <iostream>
#include <array>

#include <boost/graph/adjacency_list.hpp>

using namespace BGLgeom;
using namespace boost;
using Graph = adjacency_list<vecS,vecS,directedS,Vertex_base_property<2>,Edge_base_property_static<linear_edge<2>,2>>;

int main()
{
	point<2> p1(0.0,0.0);
	point<2> p2(0.0,4.0);
	linear_edge<2> edge1(p1,p2);

	point<2> p3(0.0,0.0);
	point<2> p4(1.0,2.0);
	linear_edge<2> edge2(p3,p4);

	Intersection I = compute_intersection(edge1,edge2);
	std::cout << I << std::endl;
	std::cout << std::endl;
	Intersection_more<Graph> I2(I);
	std::cout << I2 << std::endl;	
	
	return 0;
}
