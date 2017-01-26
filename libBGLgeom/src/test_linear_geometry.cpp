/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	test_linear_geometry.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Testing class linear_geometry and creation of mesh on an edge
			with this geometry
	
	We perfom these different tests: \n
	- Creation of a straigth line to test consistency with the 
		generic_geometry class. Evaluation of the geometric characteristics 
		for different values of the parameter, using both methods 
		that accept one single parameter and methods accepting vectors 
		of paramters. Creation of a uniform and a variable size mesh on it. \n
	- Creation of a graph with three linear edges. Creation of a uniform 
		and variable size mesh on them. Production of a pts and vtp
		output
*/

#include "linear_geometry.hpp"
#include "point.hpp"
#include "base_properties.hpp"
#include "graph_builder.hpp"
#include "graph_access.hpp"
#include "mesh.hpp"
#include "writer_vtp.hpp"
#include "writer_pts.hpp"
#include <vector>
#include <cmath>

using namespace BGLgeom;
using namespace boost;

int main(){

	constexpr double pi = std::atan(1.0)*4.0;

	std::cout << "====================== LINEAR_GEOMETRY ======================" << std::endl << std::endl;
	
	point<2> SRC(0,0);
	point<2> TGT(10,10);
	linear_geometry<2> edge(SRC, TGT);
	std::cout << edge << std::endl << std::endl;
	
	std::cout << "Evaluation: " << std::endl;
	std::cout << "\tt=0:   " << edge(0) << std::endl;
	std::cout << "\tt=0.5: " << edge(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge(1) << std::endl;
	
	std::cout << "First derivatives: " << std::endl;
	std::cout << "\tt=0:   " << edge.first_der(0) << std::endl;
	std::cout << "\tt=0.5: " << edge.first_der(0.5) << std::endl;
	std::cout << "\tt=1:   " << edge.first_der(1) << std::endl;
	
	std::cout << "Second derivatives: " << std::endl;
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
	
	// Now evaluation with vectors
	std::cout << std::endl << "--------- Now evaluation with vector ----------" << std::endl << std::endl;
	std::vector<double> t{0,0.5,1};
	
	std::cout << "Evaluation:" << std::endl;
	std::vector<point<2>> Eval = edge(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << Eval[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "First derivative:" << std::endl;
	std::vector<point<2>> F = edge.first_der(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << F[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Second derivative:" << std::endl;
	std::vector<point<2>> S = edge.second_der(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << S[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Curvilinear abscissa:" << std::endl;
	std::vector<double> Abs = edge.curv_abs(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << Abs[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "Curvature:" << std::endl;
	std::vector<double> Curv = edge.curvature(t);
	for(std::size_t i=0; i<t.size(); ++i)
		std::cout << "\t" << t[i] << "\t: " << Curv[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "---------- Computing a uniform mesh -----------" << std::endl << std::endl;
	mesh<2> M;
	M.uniform_mesh(10, edge);
	for(std::size_t i = 0; i < M.real.size(); ++i)
		std::cout << M.real[i] << std::endl;
	std::cout << std::endl;	
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < M.parametric.size(); ++i)
		std::cout << M.parametric[i] << std::endl;
	std::cout << std::endl;
	
	std::cout << "-------- Evaluating first derivative in the parametric mesh -------" << std::endl;
	std::vector<point<2>> first_der;
	first_der = edge.first_der(M.parametric);
	for(std::size_t i = 0; i < first_der.size(); ++i)
		std::cout << first_der[i] << std::endl;
	std::cout << std::endl;
	
	std::cout <<"-------------- Variable size mesh -----------------" << std::endl;
	mesh<2> var_M;
	var_M.variable_mesh(1000, [pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.));}, edge);
	for(std::size_t i = 0; i < var_M.real.size(); ++i)
		std::cout << var_M.real[i] << std::endl;
	std::cout << std::endl;
	std::cout << "The corresponding parametric mesh is: " << std::endl;
	for(std::size_t i = 0; i < var_M.parametric.size(); ++i)
		std::cout << var_M.parametric[i] << std::endl;
	std::cout << std::endl;
	
	
	
	// BUilding a simple Graph
	std::cout << "========================= ON GRAPH ========================" << std::endl;
	std::cout << "Creating a graph" << std::endl << std::endl;
	using Graph = boost::adjacency_list< boost::vecS, 
										 boost::vecS, 
										 boost::directedS, 
										 Vertex_base_property<3>, 
										 Edge_base_property<linear_geometry<3>,3> >;
	Graph G;
	
	point<3> A(0,0,0);
	point<3> B(3,4,5);
	point<3> C(3,2,1);
	point<3> D(5,6,7);
	
	Vertex_desc<Graph> a,b,c,d;
	Vertex_base_property<3> a_prop(A);
	Vertex_base_property<3> b_prop(B);
	Vertex_base_property<3> c_prop(C);
	Vertex_base_property<3> d_prop(D);
	a = new_vertex(a_prop, G);
	b = new_vertex(b_prop, G);
	c = new_vertex(c_prop, G);
	d = new_vertex(d_prop, G);
	
	Edge_desc<Graph> e1, e2, e3;
	e1 = new_linear_edge(a, b, G);
	e2 = new_linear_edge(b, c, G);
	e3 = new_linear_edge(c, d, G);
	
	std::cout << G[e1] << std::endl;
	std::cout << G[e2] << std::endl;
	std::cout << G[e3] << std::endl;
	
	// Computing a mesh on the edges
	G[e1].make_uniform_mesh(100);
	G[e2].make_uniform_mesh(50);
	G[e3].make_variable_mesh(1000, [pi](double const & x)->double{ return (0.05+ 0.1*std::sin(x*pi/10.)); });
	
	// Produving output
	writer_pts<Graph,3> Wpts("../data/out_test_linear_geometry.pts");
	writer_vtp<Graph,3> Wvtp("../data/out_test_linear_geometry.vtp");
	Wpts.export_pts(G);
	Wvtp.export_vtp(G);
	
	return 0;
}
