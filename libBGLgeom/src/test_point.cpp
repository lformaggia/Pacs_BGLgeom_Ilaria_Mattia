#include <iostream>
#include <fstream>
#include "point.hpp" 

using namespace BGLgeom;

int main(){
	
	std::ifstream infile;
	infile.open("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom/data/test_point.txt");
	point<3> P1;
	point<3> P2;
	point<3> P3;
	
	infile >> P1 >> P2;
	P3 = P1;
	std::cout << P1 << ", " << std::endl;
	std::cout << P2 << ", " << std::endl;
	std::cout << P3 << ", " << std::endl;
	
	std::cout << "Comparison: P1 < P2? " << operator<(P1,P2) << std::endl;
	std::cout << "Comparison: P1 > P2? " << (P1 > P2) << std::endl;
	std::cout << "Comparison: P1 == P2? " << (P1 == P2) << std::endl;
	std::cout << "Comparison: P1 == P3? " << (P1 == P3) << std::endl;
	std::cout << "Comparison: P1 != P2? " << (P1 != P2) << std::endl;
	
	return 0;
}
