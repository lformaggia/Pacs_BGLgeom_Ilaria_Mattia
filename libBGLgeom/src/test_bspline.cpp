#include "bspline_edge.hpp"
#include <vector>
#include <iostream>

using namespace BGLgeom;

int main(){

	std::vector<BGLgeom::point<2>> control_pts;
	control_pts.push_back(point<2>(0,0));
	control_pts.push_back(point<2>(0,1));
	control_pts.push_back(point<2>(1,1));
	control_pts.push_back(point<2>(1,0));
	
	BGLgeom::bspline_edge<2,2> B(control_pts);
	
	std::cout << "Evaluation: " << std::endl;
	std::cout << "\tt=0.5 : " << B(0.5) << std::endl;
	
	
	return 0;
}
