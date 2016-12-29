#include "data_structure.hpp"
#include "graph_builder.hpp"
#include "writer_pts.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <iostream>

using namespace boost;
using namespace BGLgeom;

int main(){

	constexpr double pi = std::atan(1.0)*4.0;
	
	std::cout << "Creating a mesh on a simple graph" << std::endl;
	
	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_base_property<2>, Edge_base_property_static<linear_geometry<2>,2> >;
	Graph G;
	
	point<2> A(0,0);
	point<2> B(10,10);
	point<2> C(15,5);
	
	Vertex_desc<Graph> a, b, c;
	Vertex_base_property<2> AA(A);
	Vertex_base_property<2> BB(B);
	Vertex_base_property<2> CC(C);
	a = new_vertex(AA,G);
	b = new_vertex(BB,G);
	c = new_vertex(CC,G);
	
	Edge_desc<Graph> e1, e2;
	e1 = new_linear_edge(a, b, G);
	e2 = new_linear_edge(b, c, G);
	
	G[e1].mesh.uniform_mesh(10, G[e1].geometry);
	G[e2].mesh.variable_mesh(1000,
							[pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.)); },
							G[e2].geometry );
	
	std::string filename("/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom/data/out_test_mesh.pts");
	writer_pts<Graph,2> W(filename);
	W.export_pts(G);
		
	return 0;
}
