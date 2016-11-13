//#include "generic_point.hpp"
#include <iostream>
#include <fstream>
#include "point.hpp" 

using namespace BGLgeom;

int main(){
	
	std::ifstream infile;
	infile.open("../data/test_point.txt");
	point<3> P1;
	point<3> P2;
	
	infile >> P1 >> P2;
	std::cout << "punto 1: " << P1 << ", punto 2: " << P2 << std::endl;
	
	return 0;
}
