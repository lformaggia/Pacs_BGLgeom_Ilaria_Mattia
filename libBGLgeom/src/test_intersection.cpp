#include "intersections2D.hpp"
#include "point.hpp"
#include "linear_edge.hpp"
#include <iostream>
#include <array>

using namespace BGLgeom;

int main()
{
	point<2> p1(0.0,3.0);
	point<2> p2(0.0,0.0);
	linear_edge<2> edge1(p1,p2);

	point<2> p3(0.0,4.0);
	point<2> p4(0.0,0.0);
	linear_edge<2> edge2(p3,p4);
	
	/*	//Giusta!
	std::array<std::array<double,2>,2> P;
	P[0][0] = 1;
	P[0][1] = 2;
	P[1][0] = 3;
	P[1][1] = 4;
	std::array<BGLgeom::point<2>,2> PP;
	PP = translate_array_to_eigen(P,2);
	for(std::size_t i=0; i<2; ++i)
		for(std::size_t j=0; j<2; ++j)
			std::cout << PP[i](0,j) << std::endl;
	*/

	Intersection I = compute_intersection(edge1,edge2);
	std::cout << I << std::endl;
	
	return 0;
}
