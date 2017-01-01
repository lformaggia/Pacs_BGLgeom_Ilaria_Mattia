#include "data_structure.hpp"
#include "graph_builder.hpp"
#include "writer_pts.hpp"
#include "writer_vtk.hpp"
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
/*
	Vertex_base_property<2> AA(A);
	Vertex_base_property<2> BB(B);
	Vertex_base_property<2> CC(C);
	a = new_vertex(AA,G);
	b = new_vertex(BB,G);
	c = new_vertex(CC,G);
*/
	a = new_vertex<Graph,2>(A,G);
	b = new_vertex<Graph,2>(B,G);
	c = new_vertex<Graph,2>(C,G);
		
	Edge_desc<Graph> e1, e2;
	e1 = new_linear_edge(a, b, G);
	e2 = new_linear_edge(b, c, G);
	
	G[e1].mesh.uniform_mesh(10, G[e1].geometry);
	G[e2].mesh.variable_mesh(1000,
							[pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.)); },
							G[e2].geometry );
	
	std::string filename("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/libBGLgeom/data/out_test_mesh.pts");
	writer_pts<Graph,2> W(filename);
	W.export_pts(G);
	
	
	
	std::cout<<"------- Example with B-spline edge -------"<< std::endl;
	using Graph3D = adjacency_list<vecS, vecS, directedS, Vertex_base_property<3>, Edge_base_property_static<bspline_geometry<3>,3> >;
	Graph3D G1;
	
	Vertex_desc<Graph3D> d,e,f;
	point<3> D(0,0,0);
	point<3> E(4,8,0);
	point<3> F(-1,-1,-1);
	
	d = new_vertex<Graph3D,3>(D,G1);
	e = new_vertex<Graph3D,3>(E,G1);
	f = new_vertex<Graph3D,3>(F,G1);
	
	std::vector<point<3>> CPs1 = {point<3>(0,      0,   0),	// 1st c.p.
							      point<3>(2./3.,  1,   0),	// 2nd c.p.
							      point<3>(2,      2, 8.0),	// 3rd c.p.
							      point<3>(10./3., 4,   0),	// 4th c.p.
							      point<3>(11./3., 4,   0),	// 5th c.p.
							      point<3>(4,      8,   0)};	// 6th c.p.
	
	std::vector<point<3>> CPs2 = {point<3>(0,      0,   0),	// 1st c.p.
							      point<3>(-1./6.,  -1./7.,   -1./8.),	// 2nd c.p.
							      point<3>(-2./6.,   -2./7., -3./8.),	// 3rd c.p.
							      point<3>(-.5, -3./7.,   -5./8.),	// 4th c.p.
							      point<3>(-5./6., -5./7.,   -7./8.),	// 5th c.p.
							      point<3>(-1,      -1,   -1)};	// 6th c.p.

	Edge_desc<Graph3D> e3, e4;
	e3 = new_bspline_edge<Graph3D,3>(d, e, CPs1, G1);
	e4 = new_bspline_edge<Graph3D,3>(d, f, CPs2, G1);
	
	G1[e3].mesh.uniform_mesh(10, G1[e3].geometry);
	G1[e4].mesh.uniform_mesh(1000, G1[e4].geometry );
	
	std::string filename1("/home/pacs_student/Desktop/pacs/Project/Pacs_project_Ilaria_Mattia/libBGLgeom/data/out_test_mesh3D_bspline.vtp");
	writer_vtk<Graph3D,3> W1(filename1);
	W1.export_vtp(G1);
		
	return 0;
}
