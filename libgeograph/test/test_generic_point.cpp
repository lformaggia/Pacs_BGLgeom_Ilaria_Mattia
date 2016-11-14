#include <iostream>
#include <fstream>
#include "point.hpp" 

using namespace BGLgeom;

int main(){
	
	std::ifstream infile;
	infile.open("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/data/test_point.txt");
	point<3> P1;
	point<3> P2;
	
	infile >> P1 >> P2;	//tutto in fila non va. Perché?
	//std::cout << "Hello" << std::endl;
	std::cout << P1 << ", " << std::endl;
	//infile >> P2;
	std::cout << P2 << ", " << std::endl;
	
	/*
	read_point<3>(infile,P1);
	read_point<3>(infile,P2);
	std::cout << P1 << ", " << P2 << std::endl;
	*/
	
	return 0;
}
