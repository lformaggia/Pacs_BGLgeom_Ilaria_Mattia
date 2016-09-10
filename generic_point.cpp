#include "generic_point.hpp"

int main() {
	point<double,2> punto1(0,2);
	const point<double,3> punto2(0,1,2);
	point<double,2> punto3(0,1,2);
	
	punto3.print();
	
	return 0;
};
