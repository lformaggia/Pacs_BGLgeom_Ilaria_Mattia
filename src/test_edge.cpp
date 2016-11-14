#include"generic_edge_geometry_srctgt.hpp"
#include"linear_edge_geometry_srctgt.hpp"
#include"point.hpp"
#include<cmath>
#include<functional>
#include<algorithm>

int main(){
	//generic edge
  	auto fun = [](double x) -> BGLgeom::point<3>{
		BGLgeom::point<3> P(pow(x,3),pow(x,2),x);
		return P;
  	};	
    
    auto fun1 = [](double x) -> BGLgeom::point<3>{
		BGLgeom::point<3> P(3*pow(x,2), 2*x, x);
		return P;
  	};	

  	auto fun2 = [](double x) -> BGLgeom::point<3>{
		BGLgeom::point<3> P(6*x, 2, 0);
		return P;
  	};	
  	
	BGLgeom::generic_edge_geometry_srctgt<3> edge(fun, fun1, fun2);		
	std::cout << edge << std::endl;
	
	//linear edge
	BGLgeom::point<3> SRC(0,0,0);
	BGLgeom::point<3> TGT(2,2,2);	
	BGLgeom::linear_edge_geometry_srctgt<3> edge_lin(SRC,TGT);
	std::cout << edge_lin << std::endl;
	
	//tring to break an existing edge into two new ones:
	std::cout << "Creating another edge with same extremes" << std::endl;
	BGLgeom::linear_edge_geometry_srctgt<3> edge_lin2(SRC,TGT);
	std::cout << "Resizing edges" << std::endl;
	edge_lin.set_target(BGLgeom::point<3>(1,1,1));
	edge_lin2.set_source(BGLgeom::point<3>(1,1,1));
	std::cout << "Now edges are:" << std::endl;
	std::cout << edge_lin << std::endl << edge_lin2 << std::endl;
	
	
	std::cout << "Trying with generic edge rapresenting a semicirconferenza:" << std::endl;
	auto circ = [](double teta) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(cos(teta), sin(teta));
	};
	
	auto d_circ = [](double teta) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(-sin(teta), cos(teta));
	};
	
	auto dd_circ = [](double teta) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(-cos(teta), -sin(teta));
	};
	
	constexpr double pi = 3.141526;
	constexpr double half_pi = 3.141526/2;
	
	BGLgeom::generic_edge_geometry_srctgt<2> semi_circ(circ, d_circ, dd_circ);
	std::cout << "teta = 0: " << semi_circ.value(0) << std::endl;
	std::cout << "teta = pi/2: " << semi_circ.value(half_pi) << std::endl;
	std::cout << "teta = pi: " << semi_circ.value(pi) << std::endl;
	
	std::cout << "D'(0): " << semi_circ.first_derivatives(0) << std::endl;
	std::cout << "D'(pi/2): " << semi_circ.first_derivatives(half_pi) << std::endl;
	
	std::cout << "D''(0): " << semi_circ.second_derivatives(0) << std::endl;
	std::cout << "D''(pi/2): " << semi_circ.second_derivatives(half_pi) << std::endl;
	
	return 0;
}
