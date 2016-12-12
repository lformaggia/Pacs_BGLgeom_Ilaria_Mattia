#include"generic_edge.hpp"
#include"linear_edge.hpp"
#include"point.hpp"
#include<functional>
#include<algorithm>

int main(){
	
	//generic edge
  	auto fun = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(x*x, 2*x); 
  	};	
    
    auto fun1 = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(2*x, 2);
  	};	

  	auto fun2 = [](double x) -> BGLgeom::point<2>{
		return BGLgeom::point<2>(2, 0);
  	};	
  	
	BGLgeom::generic_edge<2> edge(fun, fun1, fun2);		
	std::cout << "generic edge: " << edge << std::endl;
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;
	std::cout << "\tt=10:  " << edge(10) << std::endl;
	
	std::cout << "Some first derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge.first_derivatives(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.first_derivatives(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.first_derivatives(1) << std::endl;
	
	std::cout << "Some curvatures: " << std::endl;
	std::cout << "\tt=0:   " << edge.curvature(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.curvature(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.curvature(1) << std::endl;
	
	
	
	/*
	//linear edge
	BGLgeom::point<3> SRC(0,0,0);
	BGLgeom::point<3> TGT(2,2,2);	
	BGLgeom::linear_edge<3> edge_lin(SRC,TGT);
	std::cout << edge_lin << std::endl;
	
	//tring to break an existing edge into two new ones:
	std::cout << "Creating another edge with same extremes" << std::endl;
	BGLgeom::linear_edge<3> edge_lin2(SRC,TGT);
	std::cout << "Resizing edges" << std::endl;
	edge_lin.set_target(BGLgeom::point<3>(1,1,1));
	edge_lin2.set_source(BGLgeom::point<3>(1,1,1));
	std::cout << "Now edges are:" << std::endl;
	std::cout << edge_lin << std::endl << edge_lin2 << std::endl;
	*/
	
	
	
	return 0;
}
