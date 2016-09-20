#include "generic_point.hpp"
#include <iostream>

int main(){
	point<2,int> punto1{0,2};
	std::cout << "ok" << std::endl;
	point<3,double> punto4;
		std::cout << "ok" << std::endl;
	const point<3,int> punto2{0,1,2};
		std::cout << "ok" << std::endl;
	point<2,int> punto3{0,1};
	std::cout << "ok" << std::endl;
	point<1,int> punto5{4};
		std::cout << "ok" << std::endl;
	point<3,double> punto6(punto4);
		std::cout << "ok" << std::endl;

	int a = punto1.y();
	std::cout << a << std::endl;
	
	punto4.set({5.03,6,7});
	
	std::cout << punto4 << std::endl;
	
	point<2> punto7{1,2};
	point<2> punto8{1,1};
	
	bool prova = (punto7 < punto8);
	std::cout << prova << std::endl;

	return 0;
}
