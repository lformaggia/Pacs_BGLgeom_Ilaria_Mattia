//#include "generic_point.hpp"
#include <iostream>

#include "point2D.hpp"
#include "point3D.hpp" 

int main(){
	
	point2D P1;
	std::cout << "P1: " << P1 << std::endl;
	
	point2D P2(3,5);
	std::cout << "P2: " << P2 << std::endl;
	P2.set_x(4);
	std::cout << "P2: " << P2 << std::endl;
	P2.set_y(4);
	std::cout << "P2: " << P2 << std::endl;
	P2.set(3,3);
	std::cout << "P2: " << P2 << std::endl;
	bool boh = P1 < P2;
	std::cout << "P1 < P2: " << boh << std::endl;
	
	std::cout << std::endl << std::endl;
	
	point3D P3;
	std::cout << "P3: " << P3 << std::endl;
	
	point3D P4(3,5,7);
	std::cout << "P4: " << P4 << std::endl;
	P4.set_x(4);
	std::cout << "P4: " << P4 << std::endl;
	P4.set_y(4);
	std::cout << "P4: " << P4 << std::endl;
	P4.set_z(4);
	std::cout << "P4: " << P4 << std::endl;
	P4.set(0,0,3);
	std::cout << "P4: " << P4 << std::endl;
	bool boh2 = P3 < P4;
	std::cout << "P3 < P4: " << boh2 << std::endl;
	
	return 0;
}
