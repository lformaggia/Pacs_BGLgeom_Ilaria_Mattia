#include "linear_edge.hpp"
#include "point.hpp"
#include "data_structure.hpp"
#include "graph_builder.hpp"
#include <vector>
#include <tuple>
#include <cmath>

using namespace BGLgeom;
using namespace boost;

int main(){

	constexpr double pi = std::atan(1.0)*4.0;

	std::cout << "----------- LINEAR_EDGE -----------------" << std::endl << std::endl;
	
	point<2> SRC(0,0);
	point<2> TGT(10,10);
	linear_edge<2> edge(SRC, TGT);
	std::cout << edge << std::endl << std::endl;
	
	std::cout << "Some values: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;	
	
	std::cout << "Computing a uniform mesh: " << std::endl;
	std::vector<point<2>> mesh1, mesh2, first_der;
	std::vector<double> param_mesh;
	std::cout << std::endl;
	std::tie(mesh1,param_mesh) = edge.uniform_mesh(10);
	for(std::size_t i = 0; i < mesh1.size(); ++i)
		std::cout << mesh1[i] << std::endl;
	std::cout << std::endl;	
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < param_mesh.size(); ++i)
		std::cout << param_mesh[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Evaluating first derivative in the parametric mesh" << std::endl;
	first_der = edge.first_der(param_mesh);
	for(std::size_t i = 0; i < first_der.size(); ++i)
		std::cout << first_der[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Now variable size mesh: " << std::endl;
	param_mesh.clear();
	std::tie(mesh2, param_mesh) = edge.variable_mesh(1000, [pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.)); });
	for(std::size_t i = 0; i < mesh2.size(); ++i)
		std::cout << mesh2[i] << std::endl;
	std::cout << std::endl;
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < param_mesh.size(); ++i)
		std::cout << param_mesh[i] << std::endl;
	std::cout << std::endl;
	
	// BUilding a simple Graph
	std::cout << std::endl <<  "Creating a graph" << std::endl;
	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_base_property<3>, Edge_base_property_static<linear_edge<3>,3> >;
	Graph G;
	
	point<3> A(0,0,0);
	point<3> B(3,4,5);
	point<3> C(3,2,1);
	point<3> D(5,6,7);
	
	Vertex_desc<Graph> a,b,c,d;
	a = add_vertex(G);	//boost
	b = add_vertex(G);	//boost
	c = add_vertex(G);	//boost
	d = add_vertex(G);	//boost
	G[a].coordinates = A;
	G[b].coordinates = B;
	G[d].coordinates = D;
	
	Edge_desc<Graph> e1, e2, e3;
	e1 = new_linear_edge(a, b, G);					//BGLgeom
	e2 = new_linear_edge<Graph,3>(b, c, G, B, C);	//BGLgeom
	e3 = add_edge(c, d, G).first;					//boost
	G[e3].geometry.set_source(G[e2].geometry(1));
	G[e3].geometry.set_target(G[d].coordinates);	
	
	std::cout << "Hello! I'm vertex A:" << G[e1].geometry(0) << std::endl;
	std::cout << "Hello! I'm vertex B:" << G[e1].geometry(1) << std::endl;
	std::cout << "Hello! I'm vertex B:" << G[e2].geometry(0) << std::endl;
	std::cout << "Hello! I'm vertex C:" << G[e2].geometry(1) << std::endl;
	std::cout << "Hello! I'm vertex C:" << G[e3].geometry(0) << std::endl;
	std::cout << "Hello! I'm vertex D:" << G[e3].geometry(1) << std::endl;
	
	return 0;
}
