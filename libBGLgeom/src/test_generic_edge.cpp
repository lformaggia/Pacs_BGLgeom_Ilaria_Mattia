#include "generic_edge.hpp"
#include "linear_edge.hpp"
#include "point.hpp"
#include <functional>
#include <algorithm>
#include <cmath>
#include <vector>
#include <tuple>

int main(){
	
  	constexpr double pi = std::atan(1.0)*4.0;
  	constexpr double pih = pi/2;
	
	//generic edge
  	auto fun = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(10*x, 10*x); 
  	};	
    
    auto fun1 = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(10, 10);
  	};	

  	auto fun2 = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(0, 0);
  	};
  	
  	/*
  	auto riparam = [](double x) -> double {
  		return pi*x;
  	};
  	*/
  	
	std::vector<BGLgeom::point<2>> mesh1, mesh2, mesh3, mesh4, mesh5, first_der;
	std::vector<double> param_mesh;
  	
  	std::cout << "----------- GENERIC_EDGE -----------------" << std::endl << std::endl;
  	std::cout << "Representing a straigth line in the plane with a generic_edge" << std::endl;
  	
	BGLgeom::generic_edge<2> edge(fun, fun1, fun2);
	
	//std::cout << "generic edge: " << edge << std::endl;
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;
//	std::cout << "\tt=pi:  " << edge(pi) << std::endl;
//	std::cout << "\tt=10:  " << edge(4.44288) << std::endl;
	
	std::cout << "Some first derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge.first_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.first_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.first_der(1) << std::endl;
//	std::cout << "\tt=pi:  " << edge.first_der(pi) << std::endl;
	
	std::cout << "Some curvatures: " << std::endl;
	std::cout << "\tt=0:   " << edge.curvature(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.curvature(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.curvature(1) << std::endl;
//	std::cout << "\tt=pi:  " << edge.curvature(pi) << std::endl;
	
	std::cout << "Curvilinear abscissa: " << std::endl;
	std::cout << "\tt=0:   " << edge.curv_abs(0) << std::endl;
	std::cout << "\tt=1:   " << edge.curv_abs(1) << std::endl;
//	std::cout << "\tt=pih: " << edge.curv_abs(pih) << std::endl;
//	std::cout << "\tt=pi:  " << edge.curv_abs(pi) << std::endl;
	
	std::cout << std::endl;
	std::cout << "Computing a uniform mesh: " << std::endl;
	std::cout << std::endl;
	std::tie(mesh1,param_mesh) = edge.uniform_mesh(10);
	for(std::size_t i = 0; i < mesh1.size(); ++i)
		std::cout << mesh1[i] << std::endl;
	std::cout << std::endl;	
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < param_mesh.size(); ++i)
		std::cout << param_mesh[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Now variable size mesh: " << std::endl;
	std::tie(mesh2, param_mesh) = edge.variable_mesh(1000, [pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.)); });
	for(std::size_t i = 0; i < mesh2.size(); ++i)
		std::cout << mesh2[i] << std::endl;
	std::cout << std::endl;
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < param_mesh.size(); ++i)
		std::cout << param_mesh[i] << std::endl;
	std::cout << std::endl;
	
	//===================== ANOTHER GENERIC EDGE =================================
	std::cout << std::endl << std::endl;
	std::cout << "Another generic edge" << std::endl;
	
	auto gamma = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(std::cos(pi*x), 2*std::sin(pi*x)); 
  	};	
    
    auto gamma1 = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(-pi*std::sin(pi*x), 2*pi*std::cos(pi*x));
  	};
  	
	auto gamma2 = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(-pi*pi*std::cos(pi*x), -2*pi*pi*std::sin(pi*x)); 
  	};
  	
  	BGLgeom::generic_edge<2> edge2(gamma,gamma1,gamma2);
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << edge2(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2(1) << std::endl;
	
	std::cout << "Some first derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge2.first_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2.first_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2.first_der(1) << std::endl;
	
	std::cout << "Some curvatures: " << std::endl;
	std::cout << "\tt=0:   " << edge2.curvature(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2.curvature(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2.curvature(1) << std::endl;
	
	std::cout << "Curvilinear abscissa: " << std::endl;
	std::cout << "\tt=0:   " << edge2.curv_abs(0) << std::endl;
	std::cout << "\tt=1:   " << edge2.curv_abs(1) << std::endl;
	
	std::cout << std::endl;
	std::cout << "Computing a uniform mesh: " << std::endl;
	param_mesh.clear();
	std::tie(mesh5,param_mesh) = edge2.uniform_mesh(2);
	for(std::size_t i = 0; i < mesh5.size(); ++i)
		std::cout << mesh5[i] << std::endl;
	std::cout << std::endl;	
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < param_mesh.size(); ++i)
		std::cout << param_mesh[i] << std::endl;
	std::cout << std::endl;
  	
	return 0;
}
