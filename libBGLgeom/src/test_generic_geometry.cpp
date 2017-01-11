/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	test_generic_geometry.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Testing class generic_geometry and creation of mesh on an edge
			with this geometry
	
	@detail	We perfom these different test: \n
			- Creation of a straigth line using the generic_geometry class,
				to test consistency with the linear_geometry one. Evaluation
				of the geometric characteristics for different values of the 
				parameter. Creation of a uniform and a variable size mesh on it. \n
			- Creation of a half-circumference. Evaluation of the geometric 
				characteristics for different values of the parameter, using 
				both methods that accept one single parameter and methods
				accepting vectors of paramters; \n
			- Creation of a graph with one single edge, representing a spiral.
				Creation of a uniform mesh on it. Production of a pts and vtp
				output
*/

#include "generic_geometry.hpp"
#include "point.hpp"
#include "data_structure.hpp"
#include "graph_builder.hpp"
#include "mesh.hpp"
#include "writer_vtp.hpp"
#include "writer_pts.hpp"
#include <functional>
#include <cmath>
#include <vector>
#include <tuple>

using namespace BGLgeom;
using namespace boost;

int main(){
	
  	constexpr double pi = std::atan(1.0)*4.0;
	
	//generic edge
  	auto fun = [](double x) -> point<2>{
		return point<2>(10*x, 10*x); 
  	};	
    
    auto fun1 = [](double x) -> point<2>{
		return point<2>(10, 10);
  	};	

  	auto fun2 = [](double x) -> point<2>{
		return point<2>(0, 0);
  	};
  	  	
  	std::cout << "================== GENERIC_GEOMETRY ================" << std::endl;
  	std::cout << "Representing a straigth line in the plane with a generic_geometry" << std::endl << std::endl;
  	
	generic_geometry<2> edge(fun, fun1, fun2);
	std::cout << edge << std::endl << std::endl;
	
	std::cout << "Evaluation: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;
	
	std::cout << "First derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge.first_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.first_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.first_der(1) << std::endl;

	std::cout << "Sedond derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge.second_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.second_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.second_der(1) << std::endl;
	
	std::cout << "Curvatures: " << std::endl;
	std::cout << "\tt=0:   " << edge.curvature(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.curvature(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.curvature(1) << std::endl;
	
	std::cout << "Curvilinear abscissa: " << std::endl;
	std::cout << "\tt=0:   " << edge.curv_abs(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.curv_abs(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.curv_abs(1) << std::endl;
	
	std::cout << std::endl;
	std::cout << "Computing a uniform mesh: " << std::endl;
	std::cout << std::endl;
	mesh<2> M1;
	M1.uniform_mesh(10, edge);
	for(std::size_t i = 0; i < M1.real.size(); ++i)
		std::cout << M1.real[i] << std::endl;
	std::cout << std::endl;	
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < M1.parametric.size(); ++i)
		std::cout << M1.parametric[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Now variable size mesh: " << std::endl;
	mesh<2> M2;
	M2.variable_mesh(1000, [pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.)); }, edge);
	for(std::size_t i = 0; i < M2.real.size(); ++i)
		std::cout << M2.real[i] << std::endl;
	std::cout << std::endl;
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < M2.parametric.size(); ++i)
		std::cout << M2.parametric[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "=================== ANOTHER GENERIC EDGE =======================" << std::endl;
	std::cout << "Representing a half-circumference" << std::endl << std::endl;
	
	auto gamma = [](double x) -> point<2>{
		return point<2>(std::cos(pi*x), 2*std::sin(pi*x)); 
  	};	
    
    auto gamma1 = [](double x) -> point<2>{
		return point<2>(-pi*std::sin(pi*x), 2*pi*std::cos(pi*x));
  	};
  	
	auto gamma2 = [](double x) -> point<2>{
		return point<2>(-pi*pi*std::cos(pi*x), -2*pi*pi*std::sin(pi*x)); 
  	};
  	
  	BGLgeom::generic_geometry<2> edge2(gamma,gamma1,gamma2);
	std::cout << "Values: " << std::endl;
	std::cout << "\tt=0:   " << edge2(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2(1) << std::endl;
	
	std::cout << "First derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge2.first_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2.first_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2.first_der(1) << std::endl;
	
	std::cout << "Second derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge2.second_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2.second_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2.second_der(1) << std::endl;
	
	std::cout << "Curvatures: " << std::endl;
	std::cout << "\tt=0:   " << edge2.curvature(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2.curvature(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2.curvature(1) << std::endl;
	
	std::cout << "Curvilinear abscissa: " << std::endl;
	std::cout << "\tt=0:   " << edge2.curv_abs(0) << std::endl;
	std::cout << "\tt=0.5: " << edge2.curv_abs(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge2.curv_abs(1) << std::endl;
	
	// Now evaluation with vectors
	std::cout << std::endl << "--------- Now evaluation with vector ----------" << std::endl << std::endl;
	std::vector<double> t{0,0.5,1};
	
	std::cout << "Evaluation:" << std::endl;
	std::vector<point<2>> Eval = edge2(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << Eval[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "First derivative:" << std::endl;
	std::vector<point<2>> F = edge2.first_der(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << F[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Second derivative:" << std::endl;
	std::vector<point<2>> S = edge2.second_der(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << S[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Curvilinear abscissa:" << std::endl;
	std::vector<double> A = edge2.curv_abs(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << A[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Curvature:" << std::endl;
	std::vector<double> C = edge2.curvature(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << C[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << std::endl;
	std::cout << "Computing a uniform mesh: " << std::endl;
	mesh<2> M3;
	M3.uniform_mesh(10, edge2);
	for(std::size_t i = 0; i < M3.real.size(); ++i)
		std::cout << M3.real[i] << std::endl;
	std::cout << std::endl;	
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < M3.parametric.size(); ++i)
		std::cout << M3.parametric[i] << std::endl;
	std::cout << std::endl;
  	
  	// BUilding a simple Graph
  	std::cout << "==================== ON GRAPH ======================" << std::endl;
	std::cout << std::endl << "Creating a graph with a generic edge" << std::endl;
	using Graph = adjacency_list<vecS, vecS, directedS, Vertex_base_property<3>, Edge_base_property<generic_geometry<3>,3> >;
	Graph G;
	
	auto alfa = [](double x) -> point<3>{
		return point<3>(std::cos(pi*x), 2*std::sin(pi*x), x);
	};
	
	auto alfa1 = [](double x) -> point<3>{
		return point<3>(-pi*std::sin(pi*x), 2*pi*std::cos(pi*x), 1);
  	};
	
	auto alfa2 = [](double x) -> point<3>{
		return point<3>(-pi*pi*std::cos(pi*x), -2*pi*pi*std::sin(pi*x), 0); 
  	};
  	
  	Vertex_desc<Graph> a,b;
  	Vertex_base_property<3> src_prop(alfa(0));
  	Vertex_base_property<3> tgt_prop(alfa(1));
  	a = new_vertex(src_prop,G); 	//BGLgeom
  	b = new_vertex(tgt_prop,G);		//BGLgeom
  	
  	Edge_desc<Graph> e;
  	e = new_generic_edge<Graph,3>(a, b, G, alfa, alfa1, alfa2);	//BGLgeom
  	
  	//Creating a uniform mesh
  	G[e].make_uniform_mesh(100);
  	
  	// Output
  	writer_pts<Graph,3> Wpts("../data/out_test_generic_geometry.pts");
  	writer_vtp<Graph,3> Wvtp("../data/out_test_generic_geometry.vtp");
  	Wpts.export_pts(G);
  	Wvtp.export_vtp(G);
	
	return 0;
}
