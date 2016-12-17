#include "generic_edge.hpp"
#include "linear_edge.hpp"
#include "point.hpp"
#include <functional>
#include <algorithm>
#include <cmath>
#include <vector>

int main(){
	
  	constexpr double pi = 3.1415269;
  	constexpr double pih = pi/2;
	
	//generic edge
  	auto fun = [](double x) -> BGLgeom::point<3>{
		return BGLgeom::point<3>(std::cos(x), std::sin(x), x); 
  	};	
    
    auto fun1 = [](double x) -> BGLgeom::point<3>{
		return BGLgeom::point<3>(-std::sin(x), std::cos(x), 1);
  	};	

  	auto fun2 = [](double x) -> BGLgeom::point<3>{
		return BGLgeom::point<3>(-std::cos(x), -std::sin(x), 0);
  	};
  	
  	/*
  	auto riparam = [](double x) -> double {
  		return pi*x;
  	};
  	*/
  	
  	std::cout << "----------- GENERIC_EDGE -----------------" << std::endl << std::endl;
  	
	BGLgeom::generic_edge<3> edge(fun, fun1, fun2);	
	edge.riparam_to_curv_abs(0,pi);	
	//std::cout << "generic edge: " << edge << std::endl;
	std::cout << "Parametrized w.r.t. curvilinear abscissa: " << std::endl;
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;
	std::cout << "\tt=pi:  " << edge(pi) << std::endl;
	std::cout << "\tt=4.44279" << edge(4.44279) << std::endl;
	
	edge.riparam_original();
	std::cout << "Now coming back to original parameterization" << std::endl;
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;
	std::cout << "\tt=pi:  " << edge(pi) << std::endl;
	
	std::cout << "Some first derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge.first_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.first_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.first_der(1) << std::endl;
	std::cout << "\tt=pi:  " << edge.first_der(pi) << std::endl;
	
	std::cout << "Some curvatures: " << std::endl;
	std::cout << "\tt=0:   " << edge.curvature(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.curvature(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.curvature(1) << std::endl;
	std::cout << "\tt=pi:  " << edge.curvature(pi) << std::endl;
	
	std::cout << "Curvilinear abscissa: " << std::endl;
	std::cout << "\tt=0:   " << edge.curv_abs(0) << std::endl;
	std::cout << "\tt=1:   " << edge.curv_abs(1) << std::endl;
	std::cout << "\tt=pih: " << edge.curv_abs(pih) << std::endl;
	std::cout << "\tt=pi:  " << edge.curv_abs(pi) << std::endl;
	
	
	std::cout << std::endl << std::endl;
 	std::cout << "----------- LINEAR_EDGE -----------------" << std::endl << std::endl;
	
	BGLgeom::point<2> SRC(0,0);
	BGLgeom::point<2> TGT(2,2);
	BGLgeom::linear_edge<2> e1(SRC, TGT);
	std::cout << "Hello! " << e1 << std::endl << std::endl;
	
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << e1(0) << std::endl;
	std::cout << "\tt=0.5: " << e1(0.5) << std::endl;
	std::cout << "\tt=1:   " << e1(1) << std::endl;
	
	std::cout << "Computing a uniform mesh" << std::endl;
	e1.uniform_param_mesh(4);
	std::vector<BGLgeom::point<2>> mesh, mesh2, first_der;
	mesh = e1.evaluate_param_mesh();
	for(std::size_t i = 0; i < mesh.size(); ++i)
		std::cout << mesh[i] << std::endl;
	std::cout << std::endl;
	mesh2 = e1(e1.get_parametric_mesh());
	std::cout << "Evaluating in other way: " << std::endl;
	for(std::size_t i = 0; i < mesh.size(); ++i)
		std::cout << mesh2[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Evaluating first derivative in the parametric mesh" << std::endl;
	first_der = e1.first_der(e1.get_parametric_mesh());
	for(std::size_t i = 0; i < mesh.size(); ++i)
		std::cout << first_der[i] << std::endl;
	std::cout << std::endl;
	
	
	return 0;
}
