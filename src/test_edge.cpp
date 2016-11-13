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
		
	std::cout<<edge;
	
	std::cout<<std::endl;
	
	//linear edge
	BGLgeom::point<3> SRC(0,0,0);
	BGLgeom::point<3> TGT(2,2,2);
	
	BGLgeom::linear_edge_geometry_srctgt<3> edge_lin(SRC,TGT);
	std::cout<<edge_lin<<std::endl;
	
	return 0;
}
