#include "generic_point.hpp"
#include <iostream>

int main(){
	point<2,int> punto1(0,2);
	std::cout << "ok" << std::endl;
	point<3,double> punto4;
		std::cout << "ok" << std::endl;
	const point<3,int> punto2(0,1,2);
		std::cout << "ok" << std::endl;
	point<2,int> punto3(0,1);
		std::cout << "ok" << std::endl;
	point<1,int> punto5(4);
		std::cout << "ok" << std::endl;
	point<3,double> punto6(punto4);
		std::cout << "ok" << std::endl;
	
	punto4.print();
	punto6.print();
	int a = punto1.y();
	std::cout << a << std::endl;
	
	punto4.set(5.03,6,7);
	punto4.print();
	std::array<double,3> aaa = {{7,8,9}};
	punto4.set(aaa);
	punto4.print();
	
	return 0;
};
