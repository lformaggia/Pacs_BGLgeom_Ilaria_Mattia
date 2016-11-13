#include <iostream>
#include <fstream>
#include "point.hpp" 

using namespace BGLgeom;

int main(){
	
	std::ifstream infile;
	infile.open("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/data/test_point.txt");
	point<3> P1;
	point<3> P2;
	/*
	while(infile.peek() != std::ifstream::traits_type::eof()){
		std::cout << infile.peek() << std::endl;
		infile >> P1;
		std::cout << "punto 1: " << P1 << std::endl;
	}
	*/
	read_point<3>(infile,P1);
	read_point<3>(infile,P2);
	std::cout << P1 << ", " << P2 << std::endl;
	return 0;
}
